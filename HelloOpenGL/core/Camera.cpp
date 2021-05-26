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
	m_up = glm::normalize(glm::cross(m_forward, m_right));

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

void Camera::Update(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup)
{
	m_position = position;
	m_worldup = worldup;
	m_pitch = pitch;
	m_yaw = yaw;

	m_forward.x = glm::cos(m_pitch) * glm::sin(m_yaw);
	m_forward.y = glm::sin(m_pitch);			
	m_forward.z = glm::cos(m_pitch) * glm::cos(m_yaw);
	m_right = glm::normalize(glm::cross(m_forward, m_worldup));
	m_up = glm::normalize(glm::cross(m_forward, m_right));

	SetViewMatrix();
}

void Camera::Move(float x, float y, float z)
{
	m_forward.x = x;
	m_forward.y = y;
	m_forward.z = z;
	m_right = glm::normalize(glm::cross(m_forward, m_worldup));
	m_up = glm::normalize(glm::cross(m_forward, m_right));
}