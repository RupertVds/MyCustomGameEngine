#pragma once

#include "Component.h"
#include "Timer.h"

class MovementComponent final : public Component
{
public:
	//=======================================
	// RULE OF ZERO/FIVE/SIX
	//=======================================
	MovementComponent(float speed = 20.f) : m_Speed(speed) {}
	virtual ~MovementComponent() = default;
	MovementComponent(const MovementComponent& other) = delete;
	MovementComponent(MovementComponent&& other) = delete;
	MovementComponent& operator=(const MovementComponent& other) = delete;
	MovementComponent& operator=(MovementComponent&& other) = delete;
public:
	virtual void Update() 
	{
		auto& timer = Timer::GetInstance();
		m_Incrementer += timer.GetDeltaTime() * m_Speed;
		m_Owner->SetPosition(m_Owner->GetPosition().x + sinf(m_Incrementer), m_Owner->GetPosition().y + sinf(m_Incrementer));
	}
private:
	const float m_Speed;
	float m_Incrementer = 0;
};