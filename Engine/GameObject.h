#pragma once
#include <memory>
#include "Transform.h"
#include <vector>
#include "Component.h"
#include "Exceptions.h"

class Texture2D;
class Transform;
class Component;

class GameObject final : public std::enable_shared_from_this<GameObject>
{
public:
	GameObject() = default;
	~GameObject() = default;
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
	void SetParent(std::shared_ptr<GameObject>, bool keepWorldPosition = true);
	bool IsChild(GameObject* parent);
	size_t GetChildCount() const;
	const std::vector<std::shared_ptr<GameObject>>& GetChildren() const;
	const std::shared_ptr<GameObject>& GetChildAtIndex(int index) const;

	void UpdateWorldPosition();
	void SetLocalPosition(const glm::vec3& pos);
	const glm::vec3& GetLocalPosition() const;
	const glm::vec3& GetWorldPosition();

	bool IsMarkedForDeletion() const;
	void DeleteSelf();
private:
	void SetPositionDirty();
	void AddChild(std::shared_ptr<GameObject> child);
	void RemoveChild(std::shared_ptr<GameObject> child);
private:
	bool m_IsMarkedForDeletion{ false };
	bool m_PositionIsDirty{};
	std::vector<std::unique_ptr<Component>> m_Components;
	Transform m_LocalTransform{};
	Transform m_WorldTransform{};

	GameObject* m_Parent{};
	std::vector<std::shared_ptr<GameObject>> m_Children{};
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
