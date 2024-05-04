#pragma once
#include <glm/glm.hpp>

class Transform final
{
public:
    const glm::vec3& GetPosition() const { return m_Position; }
    void SetPosition(float x, float y, float z);
    void SetPosition(const glm::vec3& pos);

    const glm::vec3& GetRotation() const { return m_Rotation; }
    void SetRotation(float pitch, float yaw, float roll);

    const glm::vec3& GetScale() const { return m_Scale; }
    void SetScale(float scaleX, float scaleY, float scaleZ);

private:
    glm::vec3 m_Position{};
    glm::vec3 m_Rotation{}; // Pitch, Yaw, Roll
    glm::vec3 m_Scale{1.f, 1.f, 1.f};
};