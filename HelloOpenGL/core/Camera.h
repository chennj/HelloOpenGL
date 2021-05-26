#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
	glm::vec3 m_position;
	// camera forward
	// https://learnopengl-cn.github.io/
	glm::vec3 m_forward;	//z-direction
	glm::vec3 m_right;		//x-direction
	glm::vec3 m_up;			//y-direction
	glm::vec3 m_worldup;

	glm::mat4 m_viewmatrix;

	float m_pitch;
	float m_yaw;

public:
	Camera(glm::vec3 position, glm::vec3 target/*被观察目标位置*/, glm::vec3 worldup/*世界坐标系上方*/);
	Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup);
	virtual ~Camera();

public:
	inline glm::mat4 GetViewMatrix()const { return m_viewmatrix; }

	void  SetViewMatrix();
	void Update(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup);
	void Move(float x, float y, float z);
};