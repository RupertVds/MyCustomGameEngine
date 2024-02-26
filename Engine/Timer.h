#pragma once
#include "Singleton.h"
#include <chrono>

class Timer final : public Singleton<Timer>
{
public:
	//=======================================
	// MUTATORS AND ACCESSORS
	//=======================================
	inline float GetDeltaTime() const { return m_DeltaTime; };
	inline float GetFixedTimeStep() const { return m_FixedTimeStep; };

	//=======================================
	// METHODS
	//=======================================
	void Update();
private:
	const float m_FixedTimeStep{ 0.02f };
    std::chrono::high_resolution_clock::time_point m_LastTime{ std::chrono::high_resolution_clock::now() };
	float m_DeltaTime{};
};