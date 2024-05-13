#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <SDL.h>
#include "Engine.h"
#include "SceneManager.h"
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
#include <BoxColliderComponent.h>

void load() {
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	auto& inputManager = InputManager::GetInstance();
	
#if _DEBUG
	ServiceLocator::RegisterSoundSystem(std::make_unique<LoggingSoundSystem>(std::make_unique<SDLSoundSystem>()));
#else
	ServiceLocator::RegisterSoundSystem(std::make_unique<SDLSoundSystem>());
#endif

	//std::shared_ptr<GameObject> levelObject = std::make_shared<GameObject>();
	//levelObject->AddComponent<RenderComponent>();
	//levelObject->AddComponent<TilemapComponent>(16.f);

	//auto levelTilemap = levelObject->GetComponent<TilemapComponent>();

	//for (int col = 0; col < 32; ++col)
	//{
	//	levelTilemap->CreateTile(24, col);
	//}
	//levelTilemap->CreateTile(5, 6);
	//levelTilemap->CreateTile(5, 7);
	
	std::shared_ptr<GameObject> fpsObject = std::make_shared<GameObject>();
	fpsObject->AddComponent<RenderComponent>();
	fpsObject->AddComponent<TextComponent>(" ", font);
	fpsObject->AddComponent<FPSComponent>();
	fpsObject->SetLocalPosition(glm::vec3{ 20, 0, 0 });	
	fpsObject->SetParent(SceneManager::GetInstance().GetRootObject());
	
	// Player one
	std::shared_ptr<GameObject> playerOneObject = std::make_shared<GameObject>();
	playerOneObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("player_1.png"));
	playerOneObject->SetLocalPosition(glm::vec3{ Renderer::WIDTH / 2 - 50, 0, 0 });
	playerOneObject->AddComponent<BoxColliderComponent>(32.f, 32.f, CollisionComponent::ColliderType::DYNAMIC);
	playerOneObject->AddComponent<PlayerComponent>();
	playerOneObject->SetScale({ 2.f, 2.f, 2.f });

	std::shared_ptr<GameObject> groundCollision = std::make_shared<GameObject>();
	groundCollision->SetLocalPosition(glm::vec3{ 200, Renderer::HEIGHT - 200, 0 });
	groundCollision->AddComponent<BoxColliderComponent>(3000.f, 50.f, CollisionComponent::ColliderType::STATIC);

	std::shared_ptr<GameObject> groundCollision2 = std::make_shared<GameObject>();
	groundCollision2->SetLocalPosition(glm::vec3{ 50, Renderer::HEIGHT - 100, 0 });
	groundCollision2->AddComponent<BoxColliderComponent>(3000.f, 50.f, CollisionComponent::ColliderType::STATIC);

	//inputManager.BindInput(SDL_SCANCODE_W, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, -1 }), InputMode::Hold });
	//inputManager.BindInput(SDL_SCANCODE_A, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ -1, 0 }), InputMode::Hold });
	//inputManager.BindInput(SDL_SCANCODE_S, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, 1 }), InputMode::Hold });
	//inputManager.BindInput(SDL_SCANCODE_D, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 1, 0 }), InputMode::Hold });
	inputManager.BindInput(SDL_SCANCODE_A, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{-1.f, 0.f}), InputMode::Hold });
	inputManager.BindInput(SDL_SCANCODE_D, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{1.f, 0.f}), InputMode::Hold });
	inputManager.BindInput(SDL_SCANCODE_SPACE, InputBinding{ playerOneObject->AddCommand<JumpCommand>(), InputMode::Press });
	inputManager.BindInput(SDL_SCANCODE_0, InputBinding{ playerOneObject->AddCommand<PlaySFX>("Mega_Man_3_The_Passing_of_the_Blue_Crown_OC_ReMix.mp3", 0.1f), InputMode::Press});
	inputManager.BindInput(SDL_SCANCODE_1, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_1.wav", 0.2f), InputMode::Press});
	inputManager.BindInput(SDL_SCANCODE_2, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_2.wav", 0.2f), InputMode::Press});

	inputManager.AddController();
	//inputManager.BindInput(0, GAMEPAD_DPAD_UP, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, -1 }), InputMode::Hold });
	//inputManager.BindInput(0, GAMEPAD_DPAD_LEFT, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ -1, 0 }), InputMode::Hold });
	//inputManager.BindInput(0, GAMEPAD_DPAD_DOWN, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 0, 1 }), InputMode::Hold });
	//inputManager.BindInput(0, GAMEPAD_DPAD_RIGHT, InputBinding{ playerOneObject->AddCommand<MoveCommand>(glm::vec2{ 1, 0 }), InputMode::Hold });
	inputManager.BindInput(0, GAMEPAD_X, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_1.wav", 0.2f), InputMode::Press });
	inputManager.BindInput(0, GAMEPAD_Y, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_2.wav", 0.2f), InputMode::Press });
	inputManager.BindInput(0, GAMEPAD_B, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_3.wav", 0.2f), InputMode::Press });
	inputManager.BindInput(0, GAMEPAD_A, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_4.wav", 0.2f), InputMode::Press });
	
	scene.Add(fpsObject);
	scene.Add(playerOneObject);
	//scene.Add(levelObject);
	scene.Add(groundCollision);
	scene.Add(groundCollision2);
	scene.Add(SceneManager::GetInstance().GetRootObject());
}

int main(int, char* []) 
{
	Engine engine{"../Data/"};
	engine.Run(load);
	return 0;
}