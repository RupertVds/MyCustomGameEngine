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

	auto levelObject = std::make_unique<GameObject>();
	levelObject->AddComponent<TilemapComponent>("level_1_collisions.txt", 16, 16);
	levelObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("level_1.png"));

	// Add other components like FPS, player, etc.
	scene.Add(std::move(levelObject));

	//auto groundCollision = std::make_unique<GameObject>();
	//groundCollision->SetLocalPosition({ 0.f, Renderer::HEIGHT - 32.f, 0.f });
	//groundCollision->AddComponent<BoxColliderComponent>(static_cast<float>(Renderer::WIDTH), 32.f, CollisionComponent::ColliderType::STATIC);

	//auto ceilingCollision = std::make_unique<GameObject>();
	//ceilingCollision->SetLocalPosition({ 0.f, 0, 0.f });
	//ceilingCollision->AddComponent<BoxColliderComponent>(static_cast<float>(Renderer::WIDTH), 32.f, CollisionComponent::ColliderType::STATIC);

	//auto leftWallCollision = std::make_unique<GameObject>();
	//leftWallCollision->SetLocalPosition({ 0.f, 0, 0.f });
	//leftWallCollision->AddComponent<BoxColliderComponent>(32.f, static_cast<float>(Renderer::HEIGHT), CollisionComponent::ColliderType::STATIC);

	//auto rightWallCollision = std::make_unique<GameObject>();
	//rightWallCollision->SetLocalPosition({ Renderer::WIDTH - 32.f, 0, 0.f });
	//rightWallCollision->AddComponent<BoxColliderComponent>(32.f, static_cast<float>(Renderer::HEIGHT), CollisionComponent::ColliderType::STATIC);

	std::unique_ptr<GameObject> fpsObject = std::make_unique<GameObject>();
	fpsObject->AddComponent<RenderComponent>();
	fpsObject->AddComponent<TextComponent>(" ", font);
	fpsObject->AddComponent<FPSComponent>();
	fpsObject->SetLocalPosition(glm::vec3{ 40, 20, 0 });
	
	// Player one
	std::unique_ptr<GameObject> playerOneObject = std::make_unique<GameObject>();
	playerOneObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("player_1.png"));
	playerOneObject->SetLocalPosition(glm::vec3{ Renderer::WIDTH / 2 - 50, 50, 0 });
	playerOneObject->AddComponent<BoxColliderComponent>(32.f, 32.f, CollisionComponent::ColliderType::DYNAMIC);
	playerOneObject->AddComponent<PlayerComponent>();
	playerOneObject->SetScale({ 2.f, 2.f, 2.f });

	std::unique_ptr<GameObject> player2 = std::make_unique<GameObject>();
	player2->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("player_1.png"));
	player2->SetLocalPosition(glm::vec3{ 50, 0, 0 });

	std::unique_ptr<GameObject> player3 = std::make_unique<GameObject>();
	player3->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("player_1.png"));
	player3->SetLocalPosition(glm::vec3{ 50, 0, 0 });
	player2->AddChild(std::move(player3));
	player2->DeleteSelf();

	playerOneObject->AddChild(std::move(player2));

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
	
	scene.Add(std::move(fpsObject));
	scene.Add(std::move(playerOneObject));
	//scene.Add(std::move(groundCollision));
	//scene.Add(std::move(ceilingCollision));
	//scene.Add(std::move(leftWallCollision));
	//scene.Add(std::move(rightWallCollision));

	//for (auto& tile : tiles)
	//{
	//	scene.Add(std::move(tile));
	//}
}

int main(int, char* []) 
{
	Engine engine{"../Data/"};
	engine.Run(load);
	return 0;
}