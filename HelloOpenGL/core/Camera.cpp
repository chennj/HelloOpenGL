#include "Camera.h"

/**
* Forward的向量算法
*/
Camera::Camera(glm::vec3 position, glm::vec3 target/*被观察目标位置*/, glm::vec3 worldup/*世界坐标系上方*/)
{
	m_position = position;
	m_worldup = worldup;
	// 向量target - 向量position = 从相机指向被观察目标的向量
	m_forward = glm::normalize(target - position);
	// 向量m_forward X 向量m_worldup = 从相机指向相机右边的向量, m_right垂直m_forward与m_worlup，无论m_forward与m_worlup是否相互垂直
	m_right = glm::normalize(glm::cross(m_forward, m_worldup));
	// 向量m_forward X 向量m_right = 从相机指向相机上边的向量，m_up垂直m_forward与m_right，无论m_forward与m_right是否相互垂直
	m_up = glm::normalize(glm::cross(m_forward, m_right));

	SetViewMatrix();
}

/**
* Forward的欧拉角算法
*/
Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup)
{
	m_position = position;
	m_worldup = worldup;
	m_pitch = pitch;
	m_yaw = yaw;

	m_forward.x = glm::cos(pitch) * glm::sin(yaw);
	m_forward.y = glm::sin(pitch);
	m_forward.z = glm::cos(pitch) * glm::cos(yaw);
	m_right = glm::normalize(glm::cross(m_forward, m_worldup));
	m_up = glm::normalize(glm::cross(m_right, m_forward));

	SetViewMatrix();
}

Camera::~Camera()
{
}

void Camera::SetViewMatrix()
{
	m_viewmatrix = glm::lookAt(
		m_position,				// Camera position
		m_position + m_forward,	// target position because m_forward = target - position
		m_worldup
	);
}

void Camera::UpdateVector(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup)
{
	m_position = position;
	m_worldup = worldup;
	m_pitch = pitch;
	m_yaw = yaw;

	m_forward.x = glm::cos(m_pitch) * glm::sin(m_yaw);
	m_forward.y = glm::sin(m_pitch);			
	m_forward.z = glm::cos(m_pitch) * glm::cos(m_yaw);
	m_right = glm::normalize(glm::cross(m_forward, m_worldup));
	m_up = glm::normalize(glm::cross(m_right, m_forward));

	SetViewMatrix();
}

void Camera::UpdatePosition()
{
	m_position += m_forward * m_speedZ * 0.1f + m_right * m_speedX * 0.1f + m_up * m_speedY * 0.1f;
	SetViewMatrix();
}

void Camera::Move(float x, float y, float z)
{
	m_speedX = x;
	m_speedY = y;
	m_speedZ = z;
	UpdatePosition();
}

void Camera::MouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= m_MouseSensitivity;
	yoffset *= m_MouseSensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	UpdateVector();
}

void Camera::UpdateVector()
{
	m_forward.x = glm::cos(m_pitch) * glm::sin(m_yaw);
	m_forward.y = glm::sin(m_pitch);
	m_forward.z = glm::cos(m_pitch) * glm::cos(m_yaw);
	m_right = glm::normalize(glm::cross(m_forward, m_worldup));
	m_up = glm::normalize(glm::cross(m_right, m_forward));

	SetViewMatrix();
}

void Camera::Keyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = m_MovementSpeed * deltaTime;
	if (direction == FORWARD)
		m_position += m_forward * velocity;
	if (direction == BACKWARD)
		m_position -= m_forward * velocity;
	if (direction == LEFT)
		m_position -= m_right * velocity;
	if (direction == RIGHT)
		m_position += m_right * velocity;
	if (direction == UP)
		m_position -= m_up * velocity;
	if (direction == DOWN)
		m_position += m_up * velocity;

	SetViewMatrix();
}
