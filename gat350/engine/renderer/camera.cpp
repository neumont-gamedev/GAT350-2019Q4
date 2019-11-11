#include "camera.h"

void Camera::Update()
{
	glm::vec3 target = m_transform.translation + m_transform.qrotation * glm::vec3(0.0f, 0.0f, 1.0f);
	view_matrix = glm::lookAt(m_transform.translation, target, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::SetProjection(float fov, float aspect_ratio, float near_clip, float far_clip)
{
	this->fov = fov;
	this->aspect_ratio = aspect_ratio;
	this->near_clip = near_clip;
	this->far_clip = far_clip;

	projection_matrix = glm::perspective(glm::radians(fov), aspect_ratio, near_clip, far_clip);
}
