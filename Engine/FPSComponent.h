#pragma once
#include "TextComponent.h"
#include "Timer.h"  // Assuming Timer class is available

class FPSComponent : public TextComponent
{
private:
    int frameCount = 0;
    float elapsedTime = 0.0f;

public:
    FPSComponent(GameObject* owner, std::shared_ptr<Font> font) : TextComponent(owner,"FPS: 0", font) {}

    virtual void Update() override {
        // Accumulate elapsed time and frame count
        elapsedTime += Timer::GetInstance().GetDeltaTime();
        frameCount++;

        // Update FPS every second
        if (elapsedTime >= 0.15f) {
            // Calculate FPS
            float fps = static_cast<float>(frameCount) / elapsedTime;

            // Update the text with the current FPS
            SetText("FPS: " + std::to_string(static_cast<float>(fps)));

            // Reset counters for the next second
            frameCount = 0;
            elapsedTime = 0.0f;
        }

        // Call the base class Update method
        TextComponent::Update();
    }
};
