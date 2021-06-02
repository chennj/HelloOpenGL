#include "LightSpot.h"

#include <glm/gtx/rotate_vector.hpp>

LightSpot::LightSpot(glm::vec3 position, glm::vec3 angle, glm::vec3 color, float cutoff, float cutoffouter, float constant, float linear, float quadratic)
	:m_Position(position),m_Angle(angle),m_Color(color),m_Cutoff(cutoff),m_CutoffOuter(cutoffouter)
{
	m_Attenuation.Constant = constant;
	m_Attenuation.Linear = linear;
	m_Attenuation.Quadratic = quadratic;

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
