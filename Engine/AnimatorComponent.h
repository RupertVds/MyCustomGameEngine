#pragma once
#include "Component.h"
#include "RenderComponent.h"
#include <memory>
#include <SDL.h>
#include <unordered_map>
#include <string>

struct Animation
{
    std::shared_ptr<Texture2D> m_Sprites;
    int framesPerSecond{};
};


class AnimatorComponent : public Component
{
public:
    AnimatorComponent(GameObject* pOwner, int spriteWidth, int spriteHeight, bool isLooping = true);

    void Update() override;
    void AddSpriteSheet(const std::string& animationName, std::shared_ptr<Texture2D> texture, int framesPerSecond);
    void Play(const std::string& animationName, bool isLooping = true);
    RenderComponent* GetRenderComponent() const { return m_RenderComponent; }
    bool ReachedEndFrame() const;
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

    std::unordered_map<std::string, Animation> m_SpriteSheets;
    std::string m_CurrentAnimation;

    void UpdateFrame();
    void SetSpriteSheet(const std::shared_ptr<Texture2D>& texture);
};
