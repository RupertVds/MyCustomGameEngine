#pragma once
#include "Component.h"
#include "PlayerComponent.h"

class AnimatorComponent;
class CircleColliderComponent;

class WatermelonComponent final : public Component {
public:
    WatermelonComponent(GameObject* pOwner);
    ~WatermelonComponent() = default;
    WatermelonComponent(const WatermelonComponent& other) = delete;
    WatermelonComponent& operator=(const WatermelonComponent& other) = delete;
    WatermelonComponent(WatermelonComponent&& other) = delete;
    WatermelonComponent& operator=(WatermelonComponent&& other) = delete;

    void Render() const override;
    void Update() override;
    void FixedUpdate() override;
private:
    int m_ScoreGain{ 100 };
    BoxColliderComponent* m_pTrigger{};
};