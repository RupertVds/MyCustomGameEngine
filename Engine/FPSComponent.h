#pragma once
#include "TextComponent.h"
#include "Timer.h"

class FPSComponent final : public TextComponent
{
public:
    //=======================================
    // RULE OF ZERO/FIVE/SIX
    //=======================================
    FPSComponent(GameObject* pOwner, std::shared_ptr<Font> font) : TextComponent(pOwner, "FPS: 0", font) {}
    virtual ~FPSComponent() = default;
    FPSComponent(const FPSComponent& other) = delete;
    FPSComponent(FPSComponent&& other) = delete;
    FPSComponent& operator=(const FPSComponent& other) = delete;
    FPSComponent& operator=(FPSComponent&& other) = delete;
public:
    virtual void Update() override
    {
        // Accumulate elapsed time and frame count
        m_ElapsedTime += Timer::GetInstance().GetDeltaTime();
        m_FrameCount++;

        // Update FPS every second
        if (m_ElapsedTime >= 0.2f) {
            // Calculate FPS
            float fps = static_cast<float>(m_FrameCount) / m_ElapsedTime;

            // Update the text with the current FPS, limiting to one decimal place
            SetText("FPS: " + std::to_string(fps).substr(0, std::to_string(fps).find('.') + 2));

            // Reset counters for the next second
            m_FrameCount = 0;
            m_ElapsedTime = 0.0f;
        }

        // Call the base class Update method
        TextComponent::Update();
    }

private:
    int m_FrameCount = 0;
    float m_ElapsedTime = 0.0f;
};
