#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>

class SoundSystem
{
public:
	virtual ~SoundSystem();

	virtual void Play(const std::string& id, const float volume) = 0;
};

class NullSoundSystem final : public SoundSystem
{
	void Play(const std::string&, const float) override {}
};

class SDLSoundSystem final : public SoundSystem 
{
public:
	SDLSoundSystem();
	virtual ~SDLSoundSystem();

	void Play(const std::string& id, const float volume) override;
private:
	class SDLSoundSystemImpl;
	std::unique_ptr<SDLSoundSystemImpl> m_pImpl;
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