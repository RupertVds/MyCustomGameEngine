#pragma once
#include <memory>
#include "Transform.h"
#include <vector>
#include "Component.h"
#include "Exceptions.h"
#include "Command.h"
#include "Observer.h"

class Texture2D;
class Transform;
class Component;
class Observer;
enum class Event;

class GameObject final
{
public:
	GameObject() = default;
	~GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) noexcept = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) noexcept = delete;
public:
	void Update();
	void FixedUpdate();
	void LateUpdate();
	void Render() const;
	void RenderImGui();

	GameObject* GetParent() const;
	//void SetParent(GameObject* pParent, bool keepWorldPosition = true);
	void AddChild(std::unique_ptr<GameObject>&& pChild);
	void RemoveChild(GameObject* child);
	bool IsChild(GameObject* parent);
	size_t GetChildCount() const;
	GameObject* GetChildAtIndex(int index) const;

	void AddObserver(Observer* observer);
	void RemoveObserver(Observer* observer);
	void NotifyObservers(Event event);

	void UpdateWorldPosition();
	void SetLocalPosition(const glm::vec2& pos);
	const glm::vec2& GetLocalPosition() const;
	const glm::vec2& GetWorldPosition();

	const Transform& GetLocalTransform() const;
	void SetScale(const glm::vec3& scale);

	bool IsMarkedForDeletion() const;
	void DeleteSelf();
private:
	void SetPositionDirty();
private:
	bool m_IsMarkedForDeletion{ false };
	bool m_PositionIsDirty{};
	std::vector<std::unique_ptr<Component>> m_Components{};
	Transform m_LocalTransform{};
	Transform m_WorldTransform{};

	std::vector<std::unique_ptr<Command>> m_Commands{};

	GameObject* m_Parent{};
	std::vector<std::unique_ptr<GameObject>> m_Children{};

	std::vector<Observer*> m_Observers{};
public:
	template <typename T, typename... Args>
	T* AddComponent(Args&&... args)
	{
		if (HasComponent<T>()) throw TooManyComponentsException();
		std::unique_ptr<T> component{ std::make_unique<T>(this, std::forward<Args>(args)...) };
		m_Components.emplace_back(std::move(component));
		return reinterpret_cast<T*>(m_Components.back().get());
	}

	template <typename T>
	void RemoveComponent() {
		m_Components.erase(std::remove_if(
			m_Components.begin(),
			m_Components.end(),
			[](const auto& component) {
				return dynamic_cast<T*>(component.get()) != nullptr;
			}),
			m_Components.end());
	}

	template <typename T>
	T* GetComponent() const {
		for (const auto& component : m_Components) {
			if (auto result = dynamic_cast<T*>(component.get()))
				return result;
		}
		return nullptr;
	}

	template <typename T>
	bool HasComponent() const {
		return GetComponent<T>() != nullptr;
	}

	// Add a command to the GameObject
	template <typename T, typename... Args>
	T* AddCommand(Args&&... args)
	{
		std::unique_ptr<T> command{ std::make_unique<T>(this, std::forward<Args>(args)...) };
		T* commandPtr = command.get(); // Get raw pointer before moving
		m_Commands.emplace_back(std::move(command));
		return commandPtr;
	}

	// Remove a command from the GameObject
	template <typename T>
	void RemoveCommand() {
		m_Commands.erase(std::remove_if(
			m_Commands.begin(),
			m_Commands.end(),
			[](const auto& command) {
				return dynamic_cast<T*>(command.get()) != nullptr;
			}),
			m_Commands.end());
	}

	// Get a command from the GameObject
	template <typename T>
	T* GetCommand() const {
		for (const auto& command : m_Commands) {
			if (auto result = dynamic_cast<T*>(command.get()))
				return result;
		}
		return nullptr;
	}

	// Check if a command exists in the GameObject
	template <typename T>
	bool HasCommand() const {
		return GetCommand<T>() != nullptr;
	}
};
