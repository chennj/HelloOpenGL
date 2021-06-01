#include "LightPoint.h"

#include <glm/gtx/rotate_vector.hpp>

LightPoint::LightPoint(const glm::vec3 & position, const glm::vec3 color)
	:m_Position(position), m_Color(color)
{
	m_Attenuation.Constant = 1.0f;
	m_Attenuation.Linear = 0.09f;
	m_Attenuation.Quadratic = 0.032f;
}

LightPoint::~LightPoint()
{
}
