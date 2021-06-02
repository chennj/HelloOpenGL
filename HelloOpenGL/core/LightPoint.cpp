#include "LightPoint.h"

#include <glm/gtx/rotate_vector.hpp>

LightPoint::LightPoint(glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic)
	:m_Position(position), m_Color(color)
{
	m_Attenuation.Constant = constant;
	m_Attenuation.Linear = linear;
	m_Attenuation.Quadratic = quadratic;
}

LightPoint::~LightPoint()
{
}
