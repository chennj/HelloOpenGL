#pragma once

#include <glm/glm.hpp>

/**
*
* Æ½ÐÐ¹â
*/
class LightPoint
{
public:
	glm::vec3 m_Position;
	glm::vec3 m_Color;

	struct Attenuation {
		float Constant;
		float Linear;
		float Quadratic;
	};

	Attenuation m_Attenuation;

public:
	LightPoint(glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic);
	~LightPoint();

};