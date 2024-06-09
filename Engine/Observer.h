#pragma once
#include "GameObject.h"

class GameObject;

enum class Event
{
	PLAYER_JOIN,
	PLAYER_DAMAGE,
	PLAYER_DIED,
	PLAYER_SCORE,
	WATERMELON_PICKUP
};

class Observer
{
public:
	Observer() = default;
	virtual ~Observer() = default;
	Observer(const Observer& other) = delete;
	Observer& operator=(const Observer& other) = delete;
	Observer(Observer&& other) = delete;
	Observer& operator=(Observer&& other) = delete;
public:
	virtual void Notify(Event event, GameObject* object) = 0;
};