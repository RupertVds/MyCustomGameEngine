#include "EventQueue.h"

void EventQueue::AttachEvent(Event event, EventCallback callback) {
    eventMap[event].push_back(callback);
}

void EventQueue::DetachEvent(Event event, EventCallback callback) {
    auto& callbacks = eventMap[event];
    callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(),
        [&callback](const EventCallback& cb) { return cb.target_type() == callback.target_type(); }),
        callbacks.end());
}

void EventQueue::NotifyEvent(Event event) 
{
    auto& callbacks = eventMap[event];
    for (const auto& callback : callbacks)
    {
        callback();
    }
}