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
#include "BoxColliderComponent.h"
#include "CircleColliderComponent.h"
#include <Renderer.h>
#include <AnimatorComponent.h>

void load() {
	auto& scene = SceneManager::GetInstance().CreateScene("level");
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	auto& inputManager = InputManager::GetInstance();
#if _DEBUG
	ServiceLocator::RegisterSoundSystem(std::make_unique<LoggingSoundSystem>(std::make_unique<SDLSoundSystem>()));
#else
	ServiceLocator::RegisterSoundSystem(std::make_unique<SDLSoundSystem>());
#endif

	//auto levelObject = std::make_unique<GameObject>();
	//levelObject->AddComponent<TilemapComponent>("level_1_collisions.txt", 16, 16);
	//levelObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("level_1.png"));

	auto levelObject = std::make_unique<GameObject>();
	levelObject->AddComponent<TilemapComponent>("level_2_collisions.txt", 16, 16);
	levelObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("level_2.png"));

	// Add other components like FPS, player, etc.
	scene.Add(std::move(levelObject));

	std::unique_ptr<GameObject> fpsObject = std::make_unique<GameObject>();
	fpsObject->AddComponent<RenderComponent>();
	fpsObject->AddComponent<TextComponent>(" ", font);
	fpsObject->AddComponent<FPSComponent>();
	fpsObject->SetLocalPosition({ 40, 20 });

	//auto testCollider = std::make_unique<GameObject>();
	//testCollider->AddComponent<BoxColliderComponent>(32.f, 32.f, CollisionComponent::ColliderType::STATIC, true);
	//testCollider->SetLocalPosition({ 300, 200 });

	// ANIMS
	auto playerOneRunTexture = ResourceManager::GetInstance().LoadTexture("player_1_run.png");
	auto playerTwoRunTexture = ResourceManager::GetInstance().LoadTexture("player_2_run.png");
	auto playerOneIdleTexture = ResourceManager::GetInstance().LoadTexture("player_1_idle.png");
	auto playerTwoIdleTexture = ResourceManager::GetInstance().LoadTexture("player_2_idle.png");
	auto playerOneAttackTexture = ResourceManager::GetInstance().LoadTexture("player_1_attack.png");
	auto playerTwoAttackTexture = ResourceManager::GetInstance().LoadTexture("player_2_attack.png");
	auto playerOneJumpUpTexture = ResourceManager::GetInstance().LoadTexture("player_1_jump_up.png");
	auto playerTwoJumpUpTexture = ResourceManager::GetInstance().LoadTexture("player_2_jump_up.png");
	auto playerOneJumpDownTexture = ResourceManager::GetInstance().LoadTexture("player_1_jump_down.png");
	auto playerTwoJumpDownTexture = ResourceManager::GetInstance().LoadTexture("player_2_jump_down.png");
	auto playerOneDeathTexture = ResourceManager::GetInstance().LoadTexture("player_1_death.png");

	// Player one
	std::unique_ptr<GameObject> playerOneObject = std::make_unique<GameObject>("player_1");

	playerOneObject->AddComponent<RenderComponent>(playerOneRunTexture);
	auto playerOneAnimator = playerOneObject->AddComponent<AnimatorComponent>(16, 16, true);
	playerOneObject->SetLocalPosition({ Renderer::WIDTH / 2 - 50, 25 });
	playerOneObject->AddComponent<BoxColliderComponent>(24.f, 32.f, CollisionComponent::ColliderType::DYNAMIC);
	//playerOneObject->AddComponent<CircleColliderComponent>(16.f, CollisionComponent::ColliderType::DYNAMIC);
	playerOneObject->AddComponent<PlayerComponent>();
	playerOneObject->SetScale({ 2.f, 2.f, 2.f });

	playerOneAnimator->AddSpriteSheet("Run", playerOneRunTexture, 12);
	playerOneAnimator->AddSpriteSheet("Idle", playerOneIdleTexture, 4);
	playerOneAnimator->AddSpriteSheet("Attack", playerOneAttackTexture, 20);
	playerOneAnimator->AddSpriteSheet("JumpUp", playerOneJumpUpTexture, 6);
	playerOneAnimator->AddSpriteSheet("Death", playerOneDeathTexture, 12);
	playerOneAnimator->AddSpriteSheet("JumpDown", playerOneJumpDownTexture, 12);

	// Player two
	std::unique_ptr<GameObject> playerTwoObject = std::make_unique<GameObject>("player_2");
	playerTwoObject->AddComponent<RenderComponent>(playerTwoRunTexture);
	auto playerTwoAnimator = playerTwoObject->AddComponent<AnimatorComponent>(16, 16, true);

	playerTwoObject->SetLocalPosition({ Renderer::WIDTH / 2 + 50, 25 });
	//playerTwoObject->AddComponent<CircleColliderComponent>(16.f, CollisionComponent::ColliderType::DYNAMIC);
	playerTwoObject->AddComponent<BoxColliderComponent>(24.f, 32.f, CollisionComponent::ColliderType::DYNAMIC);
	playerTwoObject->AddComponent<PlayerComponent>();
	playerTwoObject->SetScale({ 2.f, 2.f, 2.f });

	playerTwoAnimator->AddSpriteSheet("Run", playerTwoRunTexture, 12);
	playerTwoAnimator->AddSpriteSheet("Idle", playerTwoIdleTexture, 4);
	playerTwoAnimator->AddSpriteSheet("Attack", playerTwoAttackTexture, 20);
	playerTwoAnimator->AddSpriteSheet("JumpUp", playerTwoJumpUpTexture, 6);
	playerTwoAnimator->AddSpriteSheet("JumpDown", playerTwoJumpDownTexture, 12);

	inputManager.BindInput(SDL_SCANCODE_A, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{-1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
	inputManager.BindInput(SDL_SCANCODE_D, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
	inputManager.BindInput(SDL_SCANCODE_W, InputBinding{ playerOneObject->AddCommand<JumpCommand>(), playerOneObject.get(), InputMode::Press });
	inputManager.BindInput(SDL_SCANCODE_SPACE, InputBinding{ playerOneObject->AddCommand<PlayerAttackCommand>(), playerOneObject.get(), InputMode::Press });
	inputManager.BindInput(SDL_SCANCODE_0, InputBinding{ playerOneObject->AddCommand<PlaySFX>("Mega_Man_3_The_Passing_of_the_Blue_Crown_OC_ReMix.mp3", 0.1f), playerOneObject.get(), InputMode::Press});
	inputManager.BindInput(SDL_SCANCODE_1, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_1.wav", 0.2f), playerOneObject.get(), InputMode::Press});
	inputManager.BindInput(SDL_SCANCODE_2, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_2.wav", 0.2f), playerOneObject.get(), InputMode::Press});

	inputManager.BindInput(SDL_SCANCODE_LEFT, InputBinding{ playerTwoObject->AddCommand<MoveHorizontalCommand>(glm::vec2{-1.f, 0.f}), playerTwoObject.get(), InputMode::Hold });
	inputManager.BindInput(SDL_SCANCODE_RIGHT, InputBinding{ playerTwoObject->AddCommand<MoveHorizontalCommand>(glm::vec2{1.f, 0.f}), playerTwoObject.get(), InputMode::Hold });
	inputManager.BindInput(SDL_SCANCODE_UP, InputBinding{ playerTwoObject->AddCommand<JumpCommand>(), playerTwoObject.get(), InputMode::Press });
	inputManager.BindInput(SDL_SCANCODE_RCTRL, InputBinding{ playerTwoObject->AddCommand<PlayerAttackCommand>(), playerTwoObject.get(), InputMode::Press });

	inputManager.AddController();
	inputManager.BindInput(0, GAMEPAD_DPAD_LEFT, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{-1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
	inputManager.BindInput(0, GAMEPAD_DPAD_RIGHT, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
	inputManager.BindInput(0, GAMEPAD_A, InputBinding{ playerOneObject->AddCommand<JumpCommand>(), playerOneObject.get(), InputMode::Press });
	inputManager.BindInput(0, GAMEPAD_X, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_1.wav", 0.2f), playerOneObject.get(), InputMode::Press });
	inputManager.BindInput(0, GAMEPAD_Y, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_2.wav", 0.2f), playerOneObject.get(), InputMode::Press });
	inputManager.BindInput(0, GAMEPAD_B, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_3.wav", 0.2f), playerOneObject.get(), InputMode::Press });
	//inputManager.BindInput(0, GAMEPAD_A, InputBinding{ playerOneObject->AddCommand<PlaySFX>("BubbleBobble_SFX_4.wav", 0.2f), InputMode::Press });
	
	scene.Add(std::move(fpsObject));
	scene.Add(std::move(playerOneObject));
	scene.Add(std::move(playerTwoObject));
	//scene.Add(std::move(testCollider));
}

int main(int, char* []) 
{
	Engine engine{"../Data/"};
	engine.Run(load);
	return 0;
}