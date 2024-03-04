#pragma once
#include <memory>
#include "Transform.h"
#include <vector>
#include "Component.h"
#include "Exceptions.h"

class Texture2D;
class Transform;
class Component;

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

	GameObject* GetParent() const;
	void SetParent(GameObject* parent, bool keepWorldPosition = true);
	bool IsChild(GameObject*& parent);
	size_t GetChildCount() const;
	const std::vector<GameObject*>& GetChildren() const;
	GameObject* GetChildAtIndex(int index) const;

	void UpdateWorldPosition();
	void SetPositionDirty();
	void SetLocalPosition(const glm::vec3& pos);
	const glm::vec3& GetLocalPosition() const;
	const glm::vec3& GetWorldPosition();

	bool IsMarkedForDeletion() const;
	void DeleteSelf();
private:
	void AddChild(GameObject* child);
	void RemoveChild(GameObject* child);
private:
	bool m_IsMarkedForDeletion{ false };
	std::vector<std::unique_ptr<Component>> m_Components;

	bool m_PositionIsDirty{};
	Transform m_LocalTransform{};
	Transform m_WorldTransform{};

	GameObject* m_Parent{};
	std::vector<GameObject*> m_Children{};
public:
	template <typename T, typename... Args>
	void AddComponent(Args&&... args)
	{
		if (HasComponent<T>()) throw TooManyComponentsException();
		std::unique_ptr<T> component{ std::make_unique<T>(this, std::forward<Args>(args)...) };
		m_Components.emplace_back(std::move(component));
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
};
