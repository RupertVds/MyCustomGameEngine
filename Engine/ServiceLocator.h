#pragma once
#include <memory>
#include "Singleton.h"
#include "SoundSystem.h"
#include <memory>

class ServiceLocator final
{
public:
	static SoundSystem& get_sound_system();
	static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss);

private:
	static std::unique_ptr<SoundSystem> _ss_instance;

};