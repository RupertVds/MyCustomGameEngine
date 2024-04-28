#include "ServiceLocator.h"

std::unique_ptr<SoundSystem> ServiceLocator::_ss_instance = nullptr;

void ServiceLocator::RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss)
{
	_ss_instance = ss == nullptr ? std::make_unique<NullSoundSystem>() : std::move(ss);
};

SoundSystem& ServiceLocator::get_sound_system()
{ 
	return *_ss_instance;
}