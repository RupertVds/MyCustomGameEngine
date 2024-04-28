#pragma once
#include <iostream>
#include <memory>

using Sound_Id = unsigned short;

class SoundSystem
{
public:
	virtual ~SoundSystem();

	virtual void Play(const Sound_Id id, const float volume) = 0;
};

class NullSoundSystem final : public SoundSystem
{
	void Play(const Sound_Id, const float) override {}
};


class SDLSoundSystem final : public SoundSystem
{
public:
	virtual ~SDLSoundSystem();
	void Play(const Sound_Id id, const float volume) override
	{
		volume;
		id;
		// lots of sdl_mixer code
	}
};

class LoggingSoundSystem final : public SoundSystem
{
	std::unique_ptr<SoundSystem> _real_ss;
public:
	LoggingSoundSystem(std::unique_ptr<SoundSystem>&& ss) : _real_ss(std::move(ss)) {}

	virtual ~LoggingSoundSystem() = default;

	void Play(const Sound_Id id, const float volume) override 
	{
		_real_ss->Play(id, volume);
		std::cout << "playing " << id << " at volume " << volume << std::endl;
	}
};