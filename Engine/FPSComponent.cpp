#include "FPSComponent.h"
#include "TextComponent.h"

FPSComponent::FPSComponent(GameObject* pOwner)
    :
    Component{ pOwner }
{
    m_TextComponent = GetOwner()->GetComponent<TextComponent>();
}

void FPSComponent::Update()
{
    // Accumulate elapsed time and frame count
    m_ElapsedTime += Timer::GetInstance().GetDeltaTime();
    m_FrameCount++;

    // Update FPS every second
    if (m_ElapsedTime >= 0.2f) {
        // Calculate FPS
        float fps = static_cast<float>(m_FrameCount) / m_ElapsedTime;

        // Update the text with the current FPS, limiting to one decimal place
        m_TextComponent->SetText("FPS: " + std::to_string(fps).substr(0, std::to_string(fps).find('.') + 2));
        
        // Reset counters for the next second
        m_FrameCount = 0;
        m_ElapsedTime = 0.0f;
    }
}
