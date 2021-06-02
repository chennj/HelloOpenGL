#pragma once

#include <glm/glm.hpp>

/**
*
* �۹��
*/
class LightSpot
{
public:
	glm::vec3 m_Position;
	glm::vec3 m_Color;
	glm::vec3 m_Angle;
	glm::vec3 m_Direction = { 0.0f, 0.0f, 1.0f };

	// �۹�ư뾶��cosֵ
	float m_Cutoff = 0.975f;
	// ģ�����ذ뾶��cosֵ
	float m_CutoffOuter = 0.95f;

	struct Attenuation {
		float Constant;
		float Linear;
		float Quadratic;
	};

	Attenuation m_Attenuation;

public:
	LightSpot(glm::vec3 position, glm::vec3 angle, glm::vec3 color, float cutoff, float cutoffouter, float constant, float linear, float quadratic);
	~LightSpot();

public:
	void UpdateDirection();
};