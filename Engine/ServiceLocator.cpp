#include "ServiceLocator.h"

std::unique_ptr<SoundSystem> ServiceLocator::m_SoundSystemInstance = nullptr;

void ServiceLocator::RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss)
{
	m_SoundSystemInstance = ss == nullptr ? std::make_unique<NullSoundSystem>() : std::move(ss);
};

SoundSystem& ServiceLocator::GetSoundSystem()
{ 
	return *m_SoundSystemInstance;
}