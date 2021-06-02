#include "LightSpot.h"

#include <glm/gtx/rotate_vector.hpp>

LightSpot::LightSpot(const glm::vec3 & position, const glm::vec3 & angle, const glm::vec3 color)
	:m_Position(position),m_Angle(angle),m_Color(color)
{
	UpdateDirection();
}

LightSpot::~LightSpot()
{
}

void LightSpot::UpdateDirection()
{
	m_Direction = glm::vec3(0.0f, 0.0f, 1.0f);	// pointing to z (forward)
	// Ðý×ªË³Ðò£ºz -> x -> y
	m_Direction = glm::rotateZ(m_Direction, glm::radians(m_Angle.z));
	m_Direction = glm::rotateX(m_Direction, glm::radians(m_Angle.x));
	m_Direction = glm::rotateY(m_Direction, glm::radians(m_Angle.y));
	// ·´Ïò
	m_Direction = -1.0f * m_Direction;
}
