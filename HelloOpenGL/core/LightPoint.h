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
	LightPoint(const glm::vec3 & position = { 1.2f, 1.0f, 8.0f }, const glm::vec3 color = { 1.0f,1.0f,1.0f });
	~LightPoint();

};