#pragma once
#include <glm/glm.hpp>

class Transform final
{
public:
    const glm::vec2& GetPosition() const { return m_Position; }
    void SetPosition(float x, float y);
    void SetPosition(const glm::vec2& pos);

    const glm::vec3& GetRotation() const { return m_Rotation; }
    void SetRotation(float pitch, float yaw, float roll);

    const glm::vec3& GetScale() const { return m_Scale; }
    void SetScale(float scaleX, float scaleY, float scaleZ);

private:
    glm::vec2 m_Position{};
    glm::vec3 m_Rotation{}; // Pitch, Yaw, Roll
    glm::vec3 m_Scale{1.f, 1.f, 1.f};
};