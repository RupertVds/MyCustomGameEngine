#include "GameManager.h"

#include "GameManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "PlayerComponent.h"
#include "BoxColliderComponent.h"
#include "CircleColliderComponent.h"
#include "InputManager.h"
#include "Renderer.h"
#include "ZenChanComponent.h"
#include "AnimatorComponent.h"
#include <iostream>
#include "GameCommands.h"
#include "TilemapComponent.h"
#include "LevelComponent.h"

void GameManager::LoadScene()
{
    // Here you can add more controllers if needed (only 2 for requirement now)
    if (!m_AddedControllers)
    {
        auto& inputManager = InputManager::GetInstance();
        inputManager.AddController();
        inputManager.AddController();
        m_AddedControllers = true;
    }
    //InputManager::GetInstance().ClearAllBindings();

    switch (m_CurrentState)
    {
    case GameState::TITLESCREEN:
        LoadTitleScreen();
        break;
    case GameState::SINGLEPLAYER:
        LoadSinglePlayer();
        break;
    case GameState::MULTIPLAYER:
        LoadMultiplayer();
        break;
    case GameState::VERSUS:
        LoadVersus();
        break;
    }
}

void GameManager::Render()
{
    // Render the current scene
    SceneManager::GetInstance().Render();
}

void GameManager::Update()
{
    // Update the current scene
    SceneManager::GetInstance().Update();
}

void GameManager::LateUpdate()
{
    // Late update the current scene
    SceneManager::GetInstance().LateUpdate();
}

const std::string& GameManager::GetCurrentGameSceneName() const
{
    static std::string sceneName{};
    switch (m_CurrentState)
    {
    case GameManager::GameState::SINGLEPLAYER:
        sceneName = "Singleplayer";
        break;
    case GameManager::GameState::MULTIPLAYER:
        sceneName = "Multiplayer";
        break;
    case GameManager::GameState::VERSUS:
        sceneName = "Versus";
        break;
    default:
        sceneName = "NO GAME SCENE";
        break;
    }
    return sceneName;
}

void GameManager::LoadTitleScreen()
{
    auto& scene = SceneManager::GetInstance().CreateScene("TitleScreen");
    auto& inputManager = InputManager::GetInstance();
    auto font = ResourceManager::GetInstance().LoadFont("Pixel_NES.otf", 16);

    const float textSpacing{ 25.f };

    auto singleplayerObject = std::make_unique<GameObject>();
    auto textSingleplayerObject = singleplayerObject->AddComponent<RenderComponent>();
    singleplayerObject->AddComponent<TextComponent>("Singleplayer - Press LEFT/X", font);
    singleplayerObject->SetLocalPosition({ Renderer::WIDTH * 0.5f - textSingleplayerObject->GetDestRect().w * 0.5f, Renderer::HEIGHT * 0.6f + textSpacing });
    scene.Add(std::move(singleplayerObject));

    auto multiplayerObject = std::make_unique<GameObject>();
    auto textMultiplayerObject = multiplayerObject->AddComponent<RenderComponent>();
    multiplayerObject->AddComponent<TextComponent>("Multiplayer - Press RIGHT/B", font);
    multiplayerObject->SetLocalPosition({ Renderer::WIDTH * 0.5f - textMultiplayerObject->GetDestRect().w * 0.5f, Renderer::HEIGHT * 0.6f + textSpacing * 2});
    scene.Add(std::move(multiplayerObject));

    auto versusObject = std::make_unique<GameObject>();
    auto textVersusObject = versusObject->AddComponent<RenderComponent>();
    versusObject->AddComponent<TextComponent>("Versus - Press DOWN/A", font);
    versusObject->SetLocalPosition({ Renderer::WIDTH * 0.5f - textVersusObject->GetDestRect().w * 0.5f, Renderer::HEIGHT * 0.6f + textSpacing * 3});
    scene.Add(std::move(versusObject));

    auto logoTexture = ResourceManager::GetInstance().LoadTexture("Logo_Anim.png");

    auto logoObject = std::make_unique<GameObject>();
    auto logoObjectRenderer = logoObject->AddComponent<RenderComponent>();
    auto logoObjectAnimator = logoObject->AddComponent<AnimatorComponent>();
    logoObjectAnimator->AddSpriteSheet("logo", logoTexture, 267, 216, 10);
    logoObject->SetLocalPosition({ Renderer::WIDTH * 0.5f - logoObjectRenderer->GetDestRect().w * 0.5f, textSpacing });
    scene.Add(std::move(logoObject));

    auto inputOneObject = std::make_unique<GameObject>();
    inputManager.BindInput(SDL_SCANCODE_LEFT, InputBinding{ inputOneObject->AddCommand<LoadGameModeCommand>(GameManager::GameState::SINGLEPLAYER), inputOneObject.get(), InputMode::Press });
    inputManager.BindInput(0, GAMEPAD_X, InputBinding{ inputOneObject->AddCommand<LoadGameModeCommand>(GameManager::GameState::SINGLEPLAYER), inputOneObject.get(), InputMode::Press });
    inputManager.BindInput(SDL_SCANCODE_RIGHT, InputBinding{ inputOneObject->AddCommand<LoadGameModeCommand>(GameManager::GameState::MULTIPLAYER), inputOneObject.get(), InputMode::Press });
    inputManager.BindInput(0, GAMEPAD_B, InputBinding{ inputOneObject->AddCommand<LoadGameModeCommand>(GameManager::GameState::MULTIPLAYER), inputOneObject.get(), InputMode::Press });
    inputManager.BindInput(SDL_SCANCODE_DOWN, InputBinding{ inputOneObject->AddCommand<LoadGameModeCommand>(GameManager::GameState::VERSUS), inputOneObject.get(), InputMode::Press });
    inputManager.BindInput(0, GAMEPAD_A, InputBinding{ inputOneObject->AddCommand<LoadGameModeCommand>(GameManager::GameState::VERSUS), inputOneObject.get(), InputMode::Press });
    scene.Add(std::move(inputOneObject));

    auto inputTwoObject = std::make_unique<GameObject>();
    inputManager.BindInput(1, GAMEPAD_X, InputBinding{ inputTwoObject->AddCommand<LoadGameModeCommand>(GameManager::GameState::SINGLEPLAYER), inputTwoObject.get(), InputMode::Press });
    inputManager.BindInput(1, GAMEPAD_B, InputBinding{ inputTwoObject->AddCommand<LoadGameModeCommand>(GameManager::GameState::MULTIPLAYER), inputTwoObject.get(), InputMode::Press });
    inputManager.BindInput(1, GAMEPAD_A, InputBinding{ inputTwoObject->AddCommand<LoadGameModeCommand>(GameManager::GameState::VERSUS), inputTwoObject.get(), InputMode::Press });
    scene.Add(std::move(inputTwoObject));

    std::cout << "Title Screen Loaded" << std::endl;
}

void GameManager::LoadSinglePlayer()
{
    auto& scene = SceneManager::GetInstance().CreateScene("Singleplayer");
    auto font = ResourceManager::GetInstance().LoadFont("Pixel_NES.otf", 24);
    auto& inputManager = InputManager::GetInstance();

    auto levelObject = std::make_unique<GameObject>();
    levelObject->AddComponent<LevelComponent>();

    // ANIMS
    auto playerOneRunTexture = ResourceManager::GetInstance().LoadTexture("player_1_run.png");
    auto playerOneIdleTexture = ResourceManager::GetInstance().LoadTexture("player_1_idle.png");
    auto playerOneAttackTexture = ResourceManager::GetInstance().LoadTexture("player_1_attack.png");
    auto playerOneJumpUpTexture = ResourceManager::GetInstance().LoadTexture("player_1_jump_up.png");
    auto playerOneJumpDownTexture = ResourceManager::GetInstance().LoadTexture("player_1_jump_down.png");
    auto playerOneDeathTexture = ResourceManager::GetInstance().LoadTexture("player_1_death.png");

    // Player one
    std::unique_ptr<GameObject> playerOneObject = std::make_unique<GameObject>("player_1");

    playerOneObject->AddComponent<RenderComponent>(playerOneRunTexture);
    auto playerOneAnimator = playerOneObject->AddComponent<AnimatorComponent>();
    playerOneObject->SetLocalPosition({ (Renderer::WIDTH - Renderer::UI_WIDTH) / 2 - 50, 25 });
    playerOneObject->AddComponent<BoxColliderComponent>(24.f, 32.f, CollisionComponent::ColliderType::DYNAMIC);
    playerOneObject->AddComponent<PlayerComponent>();
    playerOneObject->SetScale({ 2.f, 2.f, 2.f });

    playerOneAnimator->AddSpriteSheet("Run", playerOneRunTexture, 16, 16, 12);
    playerOneAnimator->AddSpriteSheet("Idle", playerOneIdleTexture, 16, 16, 4);
    playerOneAnimator->AddSpriteSheet("Attack", playerOneAttackTexture, 16, 16, 20);
    playerOneAnimator->AddSpriteSheet("JumpUp", playerOneJumpUpTexture, 16, 16, 6);
    playerOneAnimator->AddSpriteSheet("Death", playerOneDeathTexture, 16, 32, 8);
    playerOneAnimator->AddSpriteSheet("JumpDown", playerOneJumpDownTexture, 16, 16, 12);

    inputManager.BindInput(SDL_SCANCODE_A, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{-1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(SDL_SCANCODE_D, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(SDL_SCANCODE_W, InputBinding{ playerOneObject->AddCommand<JumpCommand>(), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(SDL_SCANCODE_SPACE, InputBinding{ playerOneObject->AddCommand<PlayerAttackCommand>(), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(SDL_SCANCODE_0, InputBinding{ playerOneObject->AddCommand<PlaySFX>("normal_mode.mp3", 0.1f), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(SDL_SCANCODE_1, InputBinding{ playerOneObject->AddCommand<PlaySFX>("versus_mode.mp3", 0.2f), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(SDL_SCANCODE_2, InputBinding{ playerOneObject->AddCommand<PlaySFX>("versus_mode_2.mp3", 0.2f), playerOneObject.get(), InputMode::Press });

    inputManager.BindInput(0, GAMEPAD_DPAD_LEFT, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{-1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(0, GAMEPAD_DPAD_RIGHT, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(0, GAMEPAD_A, InputBinding{ playerOneObject->AddCommand<JumpCommand>(), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(0, GAMEPAD_X, InputBinding{ playerOneObject->AddCommand<PlayerAttackCommand>(), playerOneObject.get(), InputMode::Press });

    scene.Add(std::move(levelObject));
    scene.Add(std::move(playerOneObject));
    std::cout << "Single Player Loaded" << std::endl;

}

void GameManager::LoadMultiplayer()
{
    auto& scene = SceneManager::GetInstance().CreateScene("Multiplayer");
    auto font = ResourceManager::GetInstance().LoadFont("Pixel_NES.otf", 24);
    auto& inputManager = InputManager::GetInstance();

    std::unique_ptr<GameObject> fpsObject = std::make_unique<GameObject>();
    fpsObject->AddComponent<RenderComponent>();
    fpsObject->AddComponent<TextComponent>(" ", font);
    fpsObject->AddComponent<FPSComponent>();
    fpsObject->SetLocalPosition({ 40, 20 });

    auto levelObject = std::make_unique<GameObject>();
    levelObject->AddComponent<LevelComponent>();

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
    auto playerTwoDeathTexture = ResourceManager::GetInstance().LoadTexture("player_2_death.png");

    // Player one
    std::unique_ptr<GameObject> playerOneObject = std::make_unique<GameObject>("player_1");

    playerOneObject->AddComponent<RenderComponent>(playerOneRunTexture);
    auto playerOneAnimator = playerOneObject->AddComponent<AnimatorComponent>();
    playerOneObject->SetLocalPosition({ (Renderer::WIDTH - Renderer::UI_WIDTH) / 2 - 50, 25 });
    playerOneObject->AddComponent<BoxColliderComponent>(24.f, 32.f, CollisionComponent::ColliderType::DYNAMIC);
    //playerOneObject->AddComponent<CircleColliderComponent>(16.f, CollisionComponent::ColliderType::DYNAMIC);
    playerOneObject->AddComponent<PlayerComponent>();
    playerOneObject->SetScale({ 2.f, 2.f, 2.f });

    playerOneAnimator->AddSpriteSheet("Run", playerOneRunTexture, 16, 16, 12);
    playerOneAnimator->AddSpriteSheet("Idle", playerOneIdleTexture, 16, 16, 4);
    playerOneAnimator->AddSpriteSheet("Attack", playerOneAttackTexture, 16, 16, 20);
    playerOneAnimator->AddSpriteSheet("JumpUp", playerOneJumpUpTexture, 16, 16, 6);
    playerOneAnimator->AddSpriteSheet("Death", playerOneDeathTexture, 16, 32, 8);
    playerOneAnimator->AddSpriteSheet("JumpDown", playerOneJumpDownTexture, 16, 16, 12);

    // Player two
    std::unique_ptr<GameObject> playerTwoObject = std::make_unique<GameObject>("player_2");
    playerTwoObject->AddComponent<RenderComponent>(playerTwoRunTexture);
    auto playerTwoAnimator = playerTwoObject->AddComponent<AnimatorComponent>();

    playerTwoObject->SetLocalPosition({ (Renderer::WIDTH - Renderer::UI_WIDTH) / 2 + 50, 25 });
    //playerTwoObject->AddComponent<CircleColliderComponent>(16.f, CollisionComponent::ColliderType::DYNAMIC);
    playerTwoObject->AddComponent<BoxColliderComponent>(24.f, 32.f, CollisionComponent::ColliderType::DYNAMIC);
    playerTwoObject->AddComponent<PlayerComponent>();
    playerTwoObject->SetScale({ 2.f, 2.f, 2.f });

    playerTwoAnimator->AddSpriteSheet("Run", playerTwoRunTexture, 16, 16, 12);
    playerTwoAnimator->AddSpriteSheet("Idle", playerTwoIdleTexture, 16, 16, 4);
    playerTwoAnimator->AddSpriteSheet("Attack", playerTwoAttackTexture, 16, 16, 20);
    playerTwoAnimator->AddSpriteSheet("JumpUp", playerTwoJumpUpTexture, 16, 16, 6);
    playerTwoAnimator->AddSpriteSheet("Death", playerTwoDeathTexture, 16, 32, 8);
    playerTwoAnimator->AddSpriteSheet("JumpDown", playerTwoJumpDownTexture, 16, 16, 12);

    inputManager.BindInput(SDL_SCANCODE_A, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{-1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(SDL_SCANCODE_D, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(SDL_SCANCODE_W, InputBinding{ playerOneObject->AddCommand<JumpCommand>(), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(SDL_SCANCODE_SPACE, InputBinding{ playerOneObject->AddCommand<PlayerAttackCommand>(), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(SDL_SCANCODE_0, InputBinding{ playerOneObject->AddCommand<PlaySFX>("normal_mode.mp3", 0.1f), playerOneObject.get(), InputMode::Press});
    inputManager.BindInput(SDL_SCANCODE_1, InputBinding{ playerOneObject->AddCommand<PlaySFX>("versus_mode.mp3", 0.2f), playerOneObject.get(), InputMode::Press});
    inputManager.BindInput(SDL_SCANCODE_2, InputBinding{ playerOneObject->AddCommand<PlaySFX>("versus_mode_2.mp3", 0.2f), playerOneObject.get(), InputMode::Press});

    inputManager.BindInput(SDL_SCANCODE_LEFT, InputBinding{ playerTwoObject->AddCommand<MoveHorizontalCommand>(glm::vec2{-1.f, 0.f}), playerTwoObject.get(), InputMode::Hold });
    inputManager.BindInput(SDL_SCANCODE_RIGHT, InputBinding{ playerTwoObject->AddCommand<MoveHorizontalCommand>(glm::vec2{1.f, 0.f}), playerTwoObject.get(), InputMode::Hold });
    inputManager.BindInput(SDL_SCANCODE_UP, InputBinding{ playerTwoObject->AddCommand<JumpCommand>(), playerTwoObject.get(), InputMode::Press });
    inputManager.BindInput(SDL_SCANCODE_RCTRL, InputBinding{ playerTwoObject->AddCommand<PlayerAttackCommand>(), playerTwoObject.get(), InputMode::Press });

    inputManager.BindInput(0, GAMEPAD_DPAD_LEFT, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{-1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(0, GAMEPAD_DPAD_RIGHT, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(0, GAMEPAD_A, InputBinding{ playerOneObject->AddCommand<JumpCommand>(), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(0, GAMEPAD_X, InputBinding{ playerOneObject->AddCommand<PlayerAttackCommand>(), playerOneObject.get(), InputMode::Press });

    inputManager.BindInput(1, GAMEPAD_DPAD_LEFT, InputBinding{ playerTwoObject->AddCommand<MoveHorizontalCommand>(glm::vec2{-1.f, 0.f}), playerTwoObject.get(), InputMode::Hold });
    inputManager.BindInput(1, GAMEPAD_DPAD_RIGHT, InputBinding{ playerTwoObject->AddCommand<MoveHorizontalCommand>(glm::vec2{1.f, 0.f}), playerTwoObject.get(), InputMode::Hold });
    inputManager.BindInput(1, GAMEPAD_A, InputBinding{ playerTwoObject->AddCommand<JumpCommand>(), playerTwoObject.get(), InputMode::Press });
    inputManager.BindInput(1, GAMEPAD_X, InputBinding{ playerTwoObject->AddCommand<PlayerAttackCommand>(), playerTwoObject.get(), InputMode::Press });

    scene.Add(std::move(fpsObject));
    scene.Add(std::move(levelObject));
    scene.Add(std::move(playerOneObject));
    scene.Add(std::move(playerTwoObject));

    std::cout << "Multiplayer Loaded" << std::endl;
}

void GameManager::LoadVersus()
{
    auto& scene = SceneManager::GetInstance().CreateScene("Versus");
    auto font = ResourceManager::GetInstance().LoadFont("Pixel_NES.otf", 16);
    auto& inputManager = InputManager::GetInstance();

    auto levelObject = std::make_unique<GameObject>();
    levelObject->AddComponent<TilemapComponent>("level_1_collisions.txt", 16, 16);
    levelObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("level_1.png"));
    scene.Add(std::move(levelObject));

    std::unique_ptr<GameObject> fpsObject = std::make_unique<GameObject>();
    fpsObject->AddComponent<RenderComponent>();
    fpsObject->AddComponent<TextComponent>(" ", font);
    fpsObject->AddComponent<FPSComponent>();
    fpsObject->SetLocalPosition({ 0, Renderer::HEIGHT });

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
    auto playerTwoDeathTexture = ResourceManager::GetInstance().LoadTexture("player_2_death.png");

    // Player one
    std::unique_ptr<GameObject> playerOneObject = std::make_unique<GameObject>("player_1");

    playerOneObject->AddComponent<RenderComponent>(playerOneRunTexture);
    auto playerOneAnimator = playerOneObject->AddComponent<AnimatorComponent>();
    playerOneObject->SetLocalPosition({ (Renderer::WIDTH - Renderer::UI_WIDTH) / 2 - 50, 25 });
    playerOneObject->AddComponent<BoxColliderComponent>(24.f, 32.f, CollisionComponent::ColliderType::DYNAMIC);
    playerOneObject->AddComponent<PlayerComponent>();
    playerOneObject->SetScale({ 2.f, 2.f, 2.f });

    playerOneAnimator->AddSpriteSheet("Run", playerOneRunTexture, 16, 16, 12);
    playerOneAnimator->AddSpriteSheet("Idle", playerOneIdleTexture, 16, 16, 4);
    playerOneAnimator->AddSpriteSheet("Attack", playerOneAttackTexture, 16, 16, 20);
    playerOneAnimator->AddSpriteSheet("JumpUp", playerOneJumpUpTexture, 16, 16, 6);
    playerOneAnimator->AddSpriteSheet("Death", playerOneDeathTexture, 16, 32, 8);
    playerOneAnimator->AddSpriteSheet("JumpDown", playerOneJumpDownTexture, 16, 16, 12);

    // Player two
    std::unique_ptr<GameObject> playerTwoObject = std::make_unique<GameObject>("player_2");
    playerTwoObject->AddComponent<RenderComponent>(playerTwoRunTexture);
    auto playerTwoAnimator = playerTwoObject->AddComponent<AnimatorComponent>();

    playerTwoObject->SetLocalPosition({ (Renderer::WIDTH - Renderer::UI_WIDTH) / 2 + 50, 25 });
    playerTwoObject->AddComponent<BoxColliderComponent>(24.f, 32.f, CollisionComponent::ColliderType::DYNAMIC);
    playerTwoObject->AddComponent<PlayerComponent>();
    playerTwoObject->SetScale({ 2.f, 2.f, 2.f });

    playerTwoAnimator->AddSpriteSheet("Run", playerTwoRunTexture, 16, 16, 12);
    playerTwoAnimator->AddSpriteSheet("Idle", playerTwoIdleTexture, 16, 16, 4);
    playerTwoAnimator->AddSpriteSheet("Attack", playerTwoAttackTexture, 16, 16, 20);
    playerTwoAnimator->AddSpriteSheet("JumpUp", playerTwoJumpUpTexture, 16, 16, 6);
    playerTwoAnimator->AddSpriteSheet("Death", playerTwoDeathTexture, 16, 32, 8);
    playerTwoAnimator->AddSpriteSheet("JumpDown", playerTwoJumpDownTexture, 16, 16, 12);

    inputManager.BindInput(SDL_SCANCODE_A, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{-1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(SDL_SCANCODE_D, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(SDL_SCANCODE_W, InputBinding{ playerOneObject->AddCommand<JumpCommand>(), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(SDL_SCANCODE_SPACE, InputBinding{ playerOneObject->AddCommand<PlayerAttackCommand>(), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(SDL_SCANCODE_0, InputBinding{ playerOneObject->AddCommand<PlaySFX>("normal_mode.mp3", 0.1f), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(SDL_SCANCODE_1, InputBinding{ playerOneObject->AddCommand<PlaySFX>("versus_mode.mp3", 0.2f), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(SDL_SCANCODE_2, InputBinding{ playerOneObject->AddCommand<PlaySFX>("versus_mode_2.mp3", 0.2f), playerOneObject.get(), InputMode::Press });

    inputManager.BindInput(SDL_SCANCODE_LEFT, InputBinding{ playerTwoObject->AddCommand<MoveHorizontalCommand>(glm::vec2{-1.f, 0.f}), playerTwoObject.get(), InputMode::Hold });
    inputManager.BindInput(SDL_SCANCODE_RIGHT, InputBinding{ playerTwoObject->AddCommand<MoveHorizontalCommand>(glm::vec2{1.f, 0.f}), playerTwoObject.get(), InputMode::Hold });
    inputManager.BindInput(SDL_SCANCODE_UP, InputBinding{ playerTwoObject->AddCommand<JumpCommand>(), playerTwoObject.get(), InputMode::Press });
    inputManager.BindInput(SDL_SCANCODE_RCTRL, InputBinding{ playerTwoObject->AddCommand<PlayerAttackCommand>(), playerTwoObject.get(), InputMode::Press });

    inputManager.BindInput(0, GAMEPAD_DPAD_LEFT, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{-1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(0, GAMEPAD_DPAD_RIGHT, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(0, GAMEPAD_A, InputBinding{ playerOneObject->AddCommand<JumpCommand>(), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(0, GAMEPAD_X, InputBinding{ playerOneObject->AddCommand<PlayerAttackCommand>(), playerOneObject.get(), InputMode::Press });

    inputManager.BindInput(1, GAMEPAD_DPAD_LEFT, InputBinding{ playerTwoObject->AddCommand<MoveHorizontalCommand>(glm::vec2{-1.f, 0.f}), playerTwoObject.get(), InputMode::Hold });
    inputManager.BindInput(1, GAMEPAD_DPAD_RIGHT, InputBinding{ playerTwoObject->AddCommand<MoveHorizontalCommand>(glm::vec2{1.f, 0.f}), playerTwoObject.get(), InputMode::Hold });
    inputManager.BindInput(1, GAMEPAD_A, InputBinding{ playerTwoObject->AddCommand<JumpCommand>(), playerTwoObject.get(), InputMode::Press });
    inputManager.BindInput(1, GAMEPAD_X, InputBinding{ playerTwoObject->AddCommand<PlayerAttackCommand>(), playerTwoObject.get(), InputMode::Press });

    scene.Add(std::move(fpsObject));
    scene.Add(std::move(playerOneObject));
    scene.Add(std::move(playerTwoObject));

    std::cout << "Versus Loaded" << std::endl;
}

void GameManager::LoadGameUI()
{
    auto& scene = SceneManager::GetInstance().CreateScene("GameUI");
    auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
    scene;
    font;
    // Load your versus mode scene as before
    // Add your player, enemies, and other game objects
    // ...

    std::cout << "Versus Loaded" << std::endl;
}