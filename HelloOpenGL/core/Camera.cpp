#include "Camera.h"

/**
* Forward�������㷨
*/
Camera::Camera(glm::vec3 position, glm::vec3 target/*���۲�Ŀ��λ��*/, glm::vec3 worldup/*��������ϵ�Ϸ�*/)
{
	m_position = position;
	m_worldup = worldup;
	// ����target - ����position = �����ָ�򱻹۲�Ŀ�������
	m_forward = glm::normalize(target - position);
	// ����m_forward X ����m_worldup = �����ָ������ұߵ�����, m_right��ֱm_forward��m_worlup������m_forward��m_worlup�Ƿ��໥��ֱ
	m_right = glm::normalize(glm::cross(m_forward, m_worldup));
	// ����m_forward X ����m_right = �����ָ������ϱߵ�������m_up��ֱm_forward��m_right������m_forward��m_right�Ƿ��໥��ֱ
	m_up = glm::normalize(glm::cross(m_forward, m_right));

	SetViewMatrix();
}

/**
* Forward��ŷ�����㷨
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
