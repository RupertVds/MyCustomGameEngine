#pragma once
#include <memory>
#include "Singleton.h"
#include "SoundSystem.h"
#include <memory>

class ServiceLocator final
{
public:
	static SoundSystem& GetSoundSystem();
	static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss);

private:
	static std::unique_ptr<SoundSystem> m_SoundSystemInstance;
};