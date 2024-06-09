#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <SDL.h>
#include "Engine.h"
#include "SceneManager.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "GameCommands.h"
#include "InputManager.h"
#include "XInputController.h"
#include "HealthComponent.h"
#include "PlayerLivesObserver.h"
#include "PlayerPointsObserver.h"
#include "ServiceLocator.h"

#include "TilemapComponent.h"
#include "PlayerComponent.h"
#include "BoxColliderComponent.h"
#include "CircleColliderComponent.h"
#include <Renderer.h>
#include <AnimatorComponent.h>
#include "ZenChanComponent.h"

void load() {
#if _DEBUG
	ServiceLocator::RegisterSoundSystem(std::make_unique<LoggingSoundSystem>(std::make_unique<SDLSoundSystem>()));
#else
	ServiceLocator::RegisterSoundSystem(std::make_unique<SDLSoundSystem>());
#endif

	GameManager::GetInstance().SetGameState(GameManager::GameState::TITLESCREEN);
	GameManager::GetInstance().LoadScene();
}

int main(int, char* []) 
{
	Engine engine{"../Data/"};
	engine.Run(load);
	return 0;
}