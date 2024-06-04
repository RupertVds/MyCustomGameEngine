#include "AnimatorComponent.h"
#include "GameObject.h"
#include "Timer.h"
#include <iostream>

AnimatorComponent::AnimatorComponent(GameObject* pOwner, int spriteWidth, int spriteHeight, bool isLooping)
    : Component(pOwner),
    m_SpriteWidth(spriteWidth),
    m_SpriteHeight(spriteHeight),
    m_FramesPerSecond(0),
    m_CurrentFrame(0),
    m_StartFrame(0),
    m_EndFrame(0),
    m_ElapsedTime(0.0f),
    m_IsLooping(isLooping)
{
    m_RenderComponent = pOwner->GetComponent<RenderComponent>();

    SDL_Rect destRect;
    destRect.x = static_cast<int>(GetOwner()->GetLocalPosition().x);
    destRect.y = static_cast<int>(GetOwner()->GetLocalPosition().y);
    destRect.w = m_SpriteWidth;
    destRect.h = m_SpriteHeight;
    m_RenderComponent->SetDestRect(destRect);
}

void AnimatorComponent::Update()
{
    if (m_FramesPerSecond == 0) return;
    if (m_TotalColumns == 0) return;

    m_ElapsedTime += Timer::GetInstance().GetDeltaTime();
    if (m_ElapsedTime >= 1.0f / m_FramesPerSecond)
    {
        m_ElapsedTime = 0;
        UpdateFrame();
    }
}

void AnimatorComponent::AddSpriteSheet(const std::string& animationName, std::shared_ptr<Texture2D> texture, int framesPerSecond)
{
    m_SpriteSheets[animationName] = { texture, framesPerSecond };

    Play(animationName, m_FramesPerSecond);
    UpdateFrame();
}

void AnimatorComponent::Play(const std::string& animationName, bool isLooping)
{
    if (m_SpriteSheets.find(animationName) != m_SpriteSheets.end())
    {
        if (m_CurrentAnimation == animationName)
        {
            return;
            //std::cout << "WARNING: ALREADY PLAYING THE: " << animationName << " ANIMATION\n";
        }

        m_FramesPerSecond = m_SpriteSheets[animationName].framesPerSecond;
        m_CurrentAnimation = animationName;
        m_IsLooping = isLooping;
        SetSpriteSheet(m_SpriteSheets[animationName].m_Sprites);
    }
    else
    {
        std::cout << "WARNING: INVALID: " << animationName << " ANIMATION\n";
    }
}
bool AnimatorComponent::ReachedEndFrame() const
{
    return m_CurrentFrame == m_EndFrame;
}


void AnimatorComponent::SetSpriteSheet(const std::shared_ptr<Texture2D>& texture)
{
    if (m_RenderComponent)
    {
        m_RenderComponent->SetTexture(texture);
    }

    if (texture)
    {
        const auto& size = texture->GetSize();
        m_TotalColumns = size.x / m_SpriteWidth;
        m_EndFrame = (size.x / m_SpriteWidth) * (size.y / m_SpriteHeight) - 1;
        m_StartFrame = 0; // Always start at the beginning of the sprite sheet
        m_CurrentFrame = 0;
        m_ElapsedTime = 0;
        SDL_Rect srcRect;
        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = m_SpriteWidth;
        srcRect.h = m_SpriteHeight;

        m_RenderComponent->SetSrcRect(srcRect);
    }
}

void AnimatorComponent::UpdateFrame()
{
    if (m_CurrentFrame < m_EndFrame)
    {
        m_CurrentFrame++;
    }
    else if (m_IsLooping)
    {
        m_CurrentFrame = m_StartFrame;
    }
    else
    {
        m_CurrentFrame = m_EndFrame;  // Ensure it stays at the end frame if not looping
    }

    int column = m_CurrentFrame % m_TotalColumns;
    int row = m_CurrentFrame / m_TotalColumns;

    SDL_Rect srcRect;
    srcRect.x = column * m_SpriteWidth;
    srcRect.y = row * m_SpriteHeight;
    srcRect.w = m_SpriteWidth;
    srcRect.h = m_SpriteHeight;

    m_RenderComponent->SetSrcRect(srcRect);
}