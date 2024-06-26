#include "Timer.h"

void Timer::Update()
{
	const auto currentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float>(currentTime - m_LastTime).count();
	m_LastTime = currentTime;

	m_TotalTime += m_DeltaTime;
}