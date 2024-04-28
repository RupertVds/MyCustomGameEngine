#pragma once
#include "Singleton.h"
#include <functional>
#include <unordered_map>
#include <vector>

enum class Event
{
	PLAYER_JOIN,
	PLAYER_DAMAGE,
	PLAYER_DIED,
	PLAYER_SCORE,
};

class EventQueue final : public Singleton<EventQueue>
{
public:
	using EventCallback = std::function<void()>;

	~EventQueue() = default;
	EventQueue(const EventQueue& other) = delete;
	EventQueue(EventQueue&& other) noexcept = delete;
	EventQueue& operator=(const EventQueue& other) = delete;
	EventQueue& operator=(EventQueue&& other) noexcept = delete;
public:
	void AttachEvent(Event eventName, EventCallback callback);
	void DetachEvent(Event eventName, EventCallback callback);
	void NotifyEvent(Event eventName);
private:
	std::unordered_map<Event, std::vector<EventCallback>> eventMap;

private:
	friend class Singleton<EventQueue>;
	EventQueue() = default;
};