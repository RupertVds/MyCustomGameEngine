#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>
#include "SDL_mixer.h"

// Define the type for play sound request
using PlaySoundRequest = std::function<void()>;

class SoundEventQueue 
{
public:
	// Function to add a play sound request to the queue
	void AddPlaySoundRequest(PlaySoundRequest request)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Requests.push(std::move(request));
		m_Condition.notify_one();
	}

	// Function to get the next play sound request from the queue
	PlaySoundRequest GetNextRequest() 
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Condition.wait(lock, [this] { return !m_Requests.empty(); });
		PlaySoundRequest request = std::move(m_Requests.front());
		m_Requests.pop();
		return request;
	}

	// Function to check if the queue is empty
	bool IsEmpty() const
	{
		//std::unique_lock<std::mutex> lock(m_Mutex);
		return m_Requests.empty();
	}
private:
	std::mutex m_Mutex;
	std::queue<PlaySoundRequest> m_Requests;
	std::condition_variable m_Condition;
};

class SoundSystem
{
public:
	virtual ~SoundSystem();

	virtual void Play(const std::string& id, const float volume) = 0;
};

class NullSoundSystem final : public SoundSystem
{
	void Play([[maybe_unused]] const std::string& id, const float) override {}
};

class SDLSoundSystem final : public SoundSystem 
{
public:
	SDLSoundSystem() : running_(true)
	{
		// Initialize SDL_mixer and other necessary resources
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			return;
		}

		// Increase the number of channels to 32
		Mix_AllocateChannels(64);

		eventThread_ = std::thread([this]() { EventLoop(); });
	}
	virtual ~SDLSoundSystem();

	void Play(const std::string& id, const float volume) override
	{
		auto playEvent = [this, id, volume]() {
			std::string dataPath{ "../Data/BubbleBobbleSFX/" + id };

			// Check if the sound chunk is already loaded
			auto it = loadedChunks_.find(id);
			if (it != loadedChunks_.end()) {
				// If the sound chunk is cached, use it directly
				PlayChunk(it->second, volume);
				return;
			}

			// If the sound chunk is not cached, load it
			Mix_Chunk* sound = Mix_LoadWAV(dataPath.c_str());
			if (!sound) {
				// handle loading failure
				std::cerr << "Mix_LoadWAV Error: " << SDL_GetError() << std::endl;
				return;
			}

			// Cache the loaded sound chunk
			loadedChunks_[id] = sound;

			// Play the sound chunk
			PlayChunk(sound, volume);
			};

		eventQueue_.AddPlaySoundRequest(playEvent);
	}

private:
	void PlayChunk(Mix_Chunk* sound, const float volume) {
		// Set the volume
		Mix_VolumeChunk(sound, static_cast<int>(MIX_MAX_VOLUME * volume));

		// Play the sound
		int channel = Mix_PlayChannel(-1, sound, 0);
		if (channel == -1) {
			// handle playback failure
			std::cerr << "Mix_PlayChannel Error: " << SDL_GetError() << std::endl;
			// No need to free the loaded sound, as it's cached
			return;
		}
	}

	void EventLoop() {
		while (running_) 
		{
			if (!eventQueue_.IsEmpty()) {
				auto request = eventQueue_.GetNextRequest();
				request();
			}
			// Sleep or wait for new events if queue is empty
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

	SoundEventQueue eventQueue_;
	std::thread eventThread_;
	bool running_;
	std::unordered_map<std::string, Mix_Chunk*> loadedChunks_;
};

class LoggingSoundSystem final : public SoundSystem
{
	std::unique_ptr<SoundSystem> _real_ss;
public:
	LoggingSoundSystem(std::unique_ptr<SoundSystem>&& ss) : _real_ss(std::move(ss)) {}

	virtual ~LoggingSoundSystem() = default;

	void Play(const std::string& id, const float volume) override
	{
		_real_ss->Play(id, volume);
		std::cout << "playing " << id << " at volume " << volume << std::endl;
	}
};