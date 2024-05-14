#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

class GameObject;
class Scene;
class SceneManager final : public Singleton<SceneManager>
{
public:
	Scene& CreateScene(const std::string& name);

	void Update();
	void FixedUpdate();
	void LateUpdate();

	void Render() const;
	void RenderImGui();
private:
	friend class Singleton<SceneManager>;
	SceneManager() = default;
	std::vector<std::shared_ptr<Scene>> m_Scenes;
};