#include "Transform.h"

void Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void Transform::SetPosition(const glm::vec3& pos)
{
	m_Position.x = pos.x;
	m_Position.y = pos.y;
	m_Position.z = pos.z;
}

void Transform::SetRotation(float pitch, float yaw, float roll)
{
    m_Rotation.x = pitch;
    m_Rotation.y = yaw;
    m_Rotation.z = roll;
}

void Transform::SetScale(float scaleX, float scaleY, float scaleZ)
{
    m_Scale.x = scaleX;
    m_Scale.y = scaleY;
    m_Scale.z = scaleZ;
}