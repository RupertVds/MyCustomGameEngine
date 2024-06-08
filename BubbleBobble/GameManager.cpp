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

void GameManager::LoadScene()
{
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

    auto font = ResourceManager::GetInstance().LoadFont("Pixel_NES.otf", 16);
    auto titleObject = std::make_unique<GameObject>();
    auto titleTextObject = titleObject->AddComponent<RenderComponent>();
    titleObject->AddComponent<TextComponent>("Press LEFT", font);
    titleObject->SetLocalPosition({ Renderer::WIDTH * 0.5f - titleTextObject->GetDestRect().w * 0.5f, titleTextObject->GetDestRect().h });
    scene.Add(std::move(titleObject));

    auto logoTexture = ResourceManager::GetInstance().LoadTexture("Logo_Anim.png");

    auto logoObject = std::make_unique<GameObject>();
    logoObject->AddComponent<RenderComponent>();
    auto logoObjectAnimator = logoObject->AddComponent<AnimatorComponent>();
    logoObjectAnimator->AddSpriteSheet("logo", logoTexture, 267, 216, 10);
    logoObject->SetLocalPosition({ Renderer::WIDTH * 0.5f - titleTextObject->GetDestRect().w * 0.5f, titleTextObject->GetDestRect().h });
    scene.Add(std::move(logoObject));
    std::cout << "Title Screen Loaded" << std::endl;




}

void GameManager::LoadSinglePlayer()
{
    auto& scene = SceneManager::GetInstance().CreateScene("Singleplayer");
    auto font = ResourceManager::GetInstance().LoadFont("Pixel_NES.otf", 24);
    auto& inputManager = InputManager::GetInstance();

    auto levelObject = std::make_unique<GameObject>();
    levelObject->AddComponent<TilemapComponent>("level_1_collisions.txt", 16, 16);
    levelObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("level_1.png"));

    // ANIMS
    auto playerOneRunTexture = ResourceManager::GetInstance().LoadTexture("player_1_run.png");
    auto playerOneIdleTexture = ResourceManager::GetInstance().LoadTexture("player_1_idle.png");
    auto playerOneAttackTexture = ResourceManager::GetInstance().LoadTexture("player_1_attack.png");
    auto playerOneJumpUpTexture = ResourceManager::GetInstance().LoadTexture("player_1_jump_up.png");
    auto playerOneJumpDownTexture = ResourceManager::GetInstance().LoadTexture("player_1_jump_down.png");
    auto playerOneDeathTexture = ResourceManager::GetInstance().LoadTexture("player_1_death.png");

    auto zenChanRunTexture = ResourceManager::GetInstance().LoadTexture("ZenChan/Run_Anim.png");
    auto zenChanDeadBubbleTexture = ResourceManager::GetInstance().LoadTexture("ZenChan/Bubble_Anim.png");
    auto zenChanDeadTexture = ResourceManager::GetInstance().LoadTexture("ZenChan/Death_Anim.png");

    // Zen Chan
    std::unique_ptr<GameObject> zenChanObject = std::make_unique<GameObject>("zenChan");
    zenChanObject->AddComponent<RenderComponent>();
    auto zenChanAnimator = zenChanObject->AddComponent<AnimatorComponent>();
    zenChanAnimator->AddSpriteSheet("DeadBubble", zenChanDeadBubbleTexture, 16, 16, 8);
    zenChanAnimator->AddSpriteSheet("Dead", zenChanDeadTexture, 16, 16, 8);
    zenChanAnimator->AddSpriteSheet("Run", zenChanRunTexture, 16, 16, 8);
    zenChanObject->AddComponent<BoxColliderComponent>(24.f, 32.f, CollisionComponent::ColliderType::DYNAMIC);
    zenChanObject->AddComponent<ZenChanComponent>();

    zenChanObject->SetScale({ 2.f, 2.f, 2.f });
    zenChanObject->SetLocalPosition({ (Renderer::WIDTH - Renderer::UI_WIDTH) / 2 - 8 + 30, 0 });

    std::unique_ptr<GameObject> zenChanObject2 = std::make_unique<GameObject>("zenChan");
    zenChanObject2->AddComponent<RenderComponent>();
    auto zenChanAnimator2 = zenChanObject2->AddComponent<AnimatorComponent>();
    zenChanAnimator2->AddSpriteSheet("DeadBubble", zenChanDeadBubbleTexture, 16, 16, 4);
    zenChanAnimator2->AddSpriteSheet("Dead", zenChanDeadTexture, 16, 16, 4);
    zenChanAnimator2->AddSpriteSheet("Run", zenChanRunTexture, 16, 16, 8);
    zenChanObject2->AddComponent<BoxColliderComponent>(24.f, 32.f, CollisionComponent::ColliderType::DYNAMIC);
    zenChanObject2->AddComponent<ZenChanComponent>();

    zenChanObject2->SetScale({ 2.f, 2.f, 2.f });
    zenChanObject2->SetLocalPosition({ (Renderer::WIDTH - Renderer::UI_WIDTH) / 2 - 8 - 30, 0 });

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


    inputManager.BindInput(SDL_SCANCODE_A, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{-1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(SDL_SCANCODE_D, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(SDL_SCANCODE_W, InputBinding{ playerOneObject->AddCommand<JumpCommand>(), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(SDL_SCANCODE_SPACE, InputBinding{ playerOneObject->AddCommand<PlayerAttackCommand>(), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(SDL_SCANCODE_0, InputBinding{ playerOneObject->AddCommand<PlaySFX>("normal_mode.mp3", 0.1f), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(SDL_SCANCODE_1, InputBinding{ playerOneObject->AddCommand<PlaySFX>("versus_mode.mp3", 0.2f), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(SDL_SCANCODE_2, InputBinding{ playerOneObject->AddCommand<PlaySFX>("versus_mode_2.mp3", 0.2f), playerOneObject.get(), InputMode::Press });

    inputManager.AddController();
    inputManager.BindInput(0, GAMEPAD_DPAD_LEFT, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{-1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(0, GAMEPAD_DPAD_RIGHT, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(0, GAMEPAD_A, InputBinding{ playerOneObject->AddCommand<JumpCommand>(), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(0, GAMEPAD_X, InputBinding{ playerOneObject->AddCommand<PlayerAttackCommand>(), playerOneObject.get(), InputMode::Press });

    //scene.Add(std::move(fpsObject));
    scene.Add(std::move(levelObject));
    scene.Add(std::move(playerOneObject));
    scene.Add(std::move(zenChanObject));
    scene.Add(std::move(zenChanObject2));
    std::cout << "Single Player Loaded" << std::endl;

}

void GameManager::LoadMultiplayer()
{
    auto& scene = SceneManager::GetInstance().CreateScene("Multiplayer");
    auto font = ResourceManager::GetInstance().LoadFont("Pixel_NES.otf", 24);
    auto& inputManager = InputManager::GetInstance();

    auto levelObject = std::make_unique<GameObject>();
    levelObject->AddComponent<TilemapComponent>("level_3_collisions.txt", 16, 16);
    levelObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture("level_3.png"));
    scene.Add(std::move(levelObject));

    std::unique_ptr<GameObject> fpsObject = std::make_unique<GameObject>();
    fpsObject->AddComponent<RenderComponent>();
    fpsObject->AddComponent<TextComponent>(" ", font);
    fpsObject->AddComponent<FPSComponent>();
    fpsObject->SetLocalPosition({ 40, 20 });

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

    auto zenChanRunTexture = ResourceManager::GetInstance().LoadTexture("ZenChan/Run_Anim.png");
    auto zenChanDeadBubbleTexture = ResourceManager::GetInstance().LoadTexture("ZenChan/Bubble_Anim.png");
    auto zenChanDeadTexture = ResourceManager::GetInstance().LoadTexture("ZenChan/Death_Anim.png");

    // Zen Chan
    std::unique_ptr<GameObject> zenChanObject = std::make_unique<GameObject>("zenChan");
    zenChanObject->AddComponent<RenderComponent>();
    auto zenChanAnimator = zenChanObject->AddComponent<AnimatorComponent>();
    zenChanAnimator->AddSpriteSheet("DeadBubble", zenChanDeadBubbleTexture, 16, 16, 8);
    zenChanAnimator->AddSpriteSheet("Dead", zenChanDeadTexture, 16, 16, 8);
    zenChanAnimator->AddSpriteSheet("Run", zenChanRunTexture, 16, 16, 8);
    zenChanObject->AddComponent<BoxColliderComponent>(24.f, 32.f, CollisionComponent::ColliderType::DYNAMIC);
    zenChanObject->AddComponent<ZenChanComponent>();

    zenChanObject->SetScale({ 2.f, 2.f, 2.f });
    zenChanObject->SetLocalPosition({ (Renderer::WIDTH - Renderer::UI_WIDTH) / 2 - 8 + 30, 0 });

    std::unique_ptr<GameObject> zenChanObject2 = std::make_unique<GameObject>("zenChan");
    zenChanObject2->AddComponent<RenderComponent>();
    auto zenChanAnimator2 = zenChanObject2->AddComponent<AnimatorComponent>();
    zenChanAnimator2->AddSpriteSheet("DeadBubble", zenChanDeadBubbleTexture, 16, 16, 4);
    zenChanAnimator2->AddSpriteSheet("Dead", zenChanDeadTexture, 16, 16, 4);
    zenChanAnimator2->AddSpriteSheet("Run", zenChanRunTexture, 16, 16, 8);
    zenChanObject2->AddComponent<BoxColliderComponent>(24.f, 32.f, CollisionComponent::ColliderType::DYNAMIC);
    zenChanObject2->AddComponent<ZenChanComponent>();

    zenChanObject2->SetScale({ 2.f, 2.f, 2.f });
    zenChanObject2->SetLocalPosition({ (Renderer::WIDTH - Renderer::UI_WIDTH) / 2 - 8 - 30, 0 });

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

    inputManager.AddController();
    inputManager.BindInput(0, GAMEPAD_DPAD_LEFT, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{-1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(0, GAMEPAD_DPAD_RIGHT, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(0, GAMEPAD_A, InputBinding{ playerOneObject->AddCommand<JumpCommand>(), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(0, GAMEPAD_X, InputBinding{ playerOneObject->AddCommand<PlayerAttackCommand>(), playerOneObject.get(), InputMode::Press });

    inputManager.AddController();
    inputManager.BindInput(1, GAMEPAD_DPAD_LEFT, InputBinding{ playerTwoObject->AddCommand<MoveHorizontalCommand>(glm::vec2{-1.f, 0.f}), playerTwoObject.get(), InputMode::Hold });
    inputManager.BindInput(1, GAMEPAD_DPAD_RIGHT, InputBinding{ playerTwoObject->AddCommand<MoveHorizontalCommand>(glm::vec2{1.f, 0.f}), playerTwoObject.get(), InputMode::Hold });
    inputManager.BindInput(1, GAMEPAD_A, InputBinding{ playerTwoObject->AddCommand<JumpCommand>(), playerTwoObject.get(), InputMode::Press });
    inputManager.BindInput(1, GAMEPAD_X, InputBinding{ playerTwoObject->AddCommand<PlayerAttackCommand>(), playerTwoObject.get(), InputMode::Press });

    scene.Add(std::move(fpsObject));
    scene.Add(std::move(playerOneObject));
    scene.Add(std::move(playerTwoObject));
    scene.Add(std::move(zenChanObject));
    scene.Add(std::move(zenChanObject2));


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

    inputManager.AddController();
    inputManager.BindInput(0, GAMEPAD_DPAD_LEFT, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{-1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(0, GAMEPAD_DPAD_RIGHT, InputBinding{ playerOneObject->AddCommand<MoveHorizontalCommand>(glm::vec2{1.f, 0.f}), playerOneObject.get(), InputMode::Hold });
    inputManager.BindInput(0, GAMEPAD_A, InputBinding{ playerOneObject->AddCommand<JumpCommand>(), playerOneObject.get(), InputMode::Press });
    inputManager.BindInput(0, GAMEPAD_X, InputBinding{ playerOneObject->AddCommand<PlayerAttackCommand>(), playerOneObject.get(), InputMode::Press });

    inputManager.AddController();
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