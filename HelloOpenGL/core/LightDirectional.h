#pragma once

#include <glm/glm.hpp>

/**
*
* Æ½ÐÐ¹â
*/
class LightDirectional
{
public:
	glm::vec3 m_Color;
	glm::vec3 m_Angle;
	glm::vec3 m_Direction;

public:
	LightDirectional(const glm::vec3 & angle= { 0.0f, 0.0f, 0.0f }, const glm::vec3 color= { 1.0f,1.0f,1.0f });
	~LightDirectional();

public:
	void UpdateDirection();
};