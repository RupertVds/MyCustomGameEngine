#include "SoundSystem.h"
#include "SDL_mixer.h"

SoundSystem::~SoundSystem()
{
}

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

		// UNLOCK here mistake.. ??
		//m_Mutex.unlock();

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

class SDLSoundSystem::SDLSoundSystemImpl final
{
public:
	SDLSoundSystemImpl() : m_IsRunning(true)
	{
		// Initialize SDL_mixer and other necessary resources
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			return;
		}

		// Increase the number of channels to 64 so
		// we can play a lot at the same time
		Mix_AllocateChannels(64);

		m_EventThread = std::thread([this]() { EventLoop(); });
	}

	virtual ~SDLSoundSystemImpl()
	{
		m_IsRunning = false;
		if (m_EventThread.joinable()) {
			m_EventThread.join();
		}

		// Free all loaded sound chunks
		for (auto& pair : m_LoadedChunks) {
			Mix_FreeChunk(pair.second);
		}

		// Clear the map
		m_LoadedChunks.clear();

		// Shutdown SDL_mixer
		Mix_CloseAudio();
	}

	void Play(const std::string& id, const float volume)
	{
		auto playEvent = [this, id, volume]() {
			std::string dataPath{ "../Data/BubbleBobbleSFX/" + id };

			// Check if the sound chunk is already loaded
			auto it = m_LoadedChunks.find(id);
			if (it != m_LoadedChunks.end()) {
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
			m_LoadedChunks[id] = sound;

			// Play the sound chunk
			PlayChunk(sound, volume);
			};

		m_EventQueue.AddPlaySoundRequest(playEvent);
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
		while (m_IsRunning)
		{
			if (!m_EventQueue.IsEmpty()) {
				auto request = m_EventQueue.GetNextRequest();
				request();
				
			}
			// Sleep or wait for new events if queue is empty
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

	SoundEventQueue m_EventQueue;
	std::thread m_EventThread;
	bool m_IsRunning;
	std::unordered_map<std::string, Mix_Chunk*> m_LoadedChunks;
};

SDLSoundSystem::SDLSoundSystem()
	:
	m_pImpl{ std::make_unique<SDLSoundSystemImpl>() }
{
}

SDLSoundSystem::~SDLSoundSystem()
{
}

void SDLSoundSystem::Play(const std::string& id, const float volume)
{
	m_pImpl->Play(id, volume);
}

