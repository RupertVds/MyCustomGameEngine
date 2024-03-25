#pragma once
#include "Observer.h"

class Achievements final : public Observer
{
public:
	Achievements() : Observer() {}
	~Achievements() = default;
	Achievements(const Achievements& other) = delete;
	Achievements& operator=(const Achievements& other) = delete;
	Achievements(Achievements&& other) = delete;
	Achievements& operator=(Achievements&& other) = delete;
public:
	void Notify(Event event, GameObject* object) override;
};

