#include "LightDirectional.h"

#include <glm/gtx/rotate_vector.hpp>

LightDirectional::LightDirectional(const glm::vec3 & angle, const glm::vec3 color)
	:m_Angle(angle),m_Color(color)
{
	UpdateDirection();
}

LightDirectional::~LightDirectional()
{
}

void LightDirectional::UpdateDirection()
{
	m_Direction = glm::vec3(0.0f, 0.0f, 1.0f);	// pointing to z (forward)
	// ��ת˳��z -> x -> y
	m_Direction = glm::rotateZ(m_Direction, glm::radians(m_Angle.z));
	m_Direction = glm::rotateX(m_Direction, glm::radians(m_Angle.x));
	m_Direction = glm::rotateY(m_Direction, glm::radians(m_Angle.y));
	// ����
	m_Direction = -1.0f * m_Direction;
}
