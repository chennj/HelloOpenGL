#pragma once

#include <glm/glm.hpp>

/**
*
* 聚光灯
*/
class LightSpot
{
public:
	glm::vec3 m_Position;
	glm::vec3 m_Color;
	glm::vec3 m_Angle;
	glm::vec3 m_Direction = { 0.0f, 0.0f, 1.0f };

	// 聚光灯半径的cos值
	float m_Cutoff = 0.975f;
	// 模糊边沿半径的cos值
	float m_CutoffOuter = 0.95f;

public:
	LightSpot(const glm::vec3 & position = { 1.0f, 1.0f, -1.0f }, const glm::vec3 & angle = { 0.0f, 0.0f, 0.0f }, const glm::vec3 color = { 1.0f,1.0f,1.0f });
	~LightSpot();

public:
	void UpdateDirection();
};