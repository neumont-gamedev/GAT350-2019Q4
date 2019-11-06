#include "camera.h"

bool Camera::Create(const Name& name)
{
	m_name = name;

	return true;
}

void Camera::Destroy()
{
}

void Camera::SetProjection(float fov, float aspect_ratio, float near_clip, float far_clip)
{
	m_fov = fov;
	m_aspect_ratio = aspect_ratio;
	m_near_clip = near_clip;
	m_far_clip = far_clip;

	m_projection = glm::perspective(glm::radians(fov), aspect_ratio, near_clip, far_clip);
}

