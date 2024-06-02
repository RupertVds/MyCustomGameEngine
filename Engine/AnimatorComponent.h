#pragma once
#include "Component.h"
#include "RenderComponent.h"
#include <memory>
#include <SDL.h>
#include <unordered_map>
#include <string>

class AnimatorComponent : public Component
{
public:
    AnimatorComponent(GameObject* pOwner, int spriteWidth, int spriteHeight, int framesPerSecond, bool isLooping = true);

    void Update() override;
    void SetAnimation(int startFrame, int endFrame, bool isLooping = true);
    void AddSpriteSheet(const std::string& animationName, std::shared_ptr<Texture2D> texture);
    void Play(const std::string& animationName, int framesPerSecond = 6, bool isLooping = true);

private:
    RenderComponent* m_RenderComponent;
    int m_SpriteWidth{};
    int m_SpriteHeight{};
    int m_FramesPerSecond{};
    int m_CurrentFrame{};
    int m_StartFrame{};
    int m_EndFrame{};
    float m_ElapsedTime{};
    int m_TotalColumns{};
    bool m_IsLooping{};

    std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_SpriteSheets;
    std::string m_CurrentAnimation;

    void UpdateFrame();
    void SetSpriteSheet(const std::shared_ptr<Texture2D>& texture);
};
