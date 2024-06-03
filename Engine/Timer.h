#pragma once
#include "Singleton.h"
#include <chrono>

class Timer final : public Singleton<Timer>
{
public:
	inline float GetDeltaTime() const { return m_DeltaTime; };
	inline float GetFixedTimeStep() const { return m_FixedTimeStep; };
	inline float GetTotalTime() const { return m_TotalTime; }

	void Update();
private:
	const float m_FixedTimeStep{ 0.01f };
	float m_DeltaTime{};
	float m_TotalTime{};
    std::chrono::high_resolution_clock::time_point m_LastTime{ std::chrono::high_resolution_clock::now() };
};