#pragma once
#include "Component.h"

class AnimatorComponent;
class CircleColliderComponent;

class BubbleComponent final : public Component{
public:
    BubbleComponent(GameObject* pOwner, const glm::vec2& dir);
    ~BubbleComponent() = default;
    BubbleComponent(const BubbleComponent& other) = delete;
    BubbleComponent& operator=(const BubbleComponent& other) = delete;
    BubbleComponent(BubbleComponent&& other) = delete;
    BubbleComponent& operator=(BubbleComponent&& other) = delete;

    void Render() const override;
    void Update() override;
    void FixedUpdate() override;
private:
    enum class State
    {
        FORMING,
        COMPLETE
    };

    State m_State{ State::FORMING };

    AnimatorComponent* m_pAnimator{};
    CircleColliderComponent* m_pMainTrigger{};

    const float m_Speed{ 300.f };
    float m_FormingTimer{};
    const float m_FormingTime{ 0.5f };

    float m_PoppingTimer{};
    const float m_PoppingTime{ 0.5f };
    glm::vec2 m_Velocity{};
};