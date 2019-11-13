#include "camera.h"
#include "../input/input.h"
#include "../engine.h"

bool Camera::Create(const Name& name)
{
	m_name = name;

	m_engine->Get<Input>()->AddAction("camera_x", Input::X, Input::MOUSE);
	m_engine->Get<Input>()->AddAction("camera_y", Input::Y, Input::MOUSE);
	
	m_engine->Get<Input>()->AddAction("camera_forward", SDL_SCANCODE_W);
	m_engine->Get<Input>()->AddAction("camera_backward", SDL_SCANCODE_S);

	return true;
}

void Camera::Update()
{
	glm::vec3 rotation(0);
	rotation.x = m_engine->Get<Input>()->GetAxisRelative("camera_x") * 0.001f;
	rotation.y = m_engine->Get<Input>()->GetAxisRelative("camera_y") * 0.001f;

	glm::quat pitch = glm::angleAxis(rotation.y, glm::vec3(1, 0, 0));
	glm::quat yaw = glm::angleAxis(rotation.x, glm::vec3(0, 1, 0));

	m_transform.rotation = pitch * m_transform.rotation * yaw;

	glm::vec3 target = m_transform.translation + m_transform.rotation * glm::vec3(0.0f, 0.0f, 1.0f);
	m_view_matrix = glm::lookAt(m_transform.translation, target, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::SetProjection(float fov, float aspect_ratio, float near_clip, float far_clip)
{
	this->fov = fov;
	this->aspect_ratio = aspect_ratio;
	this->near_clip = near_clip;
	this->far_clip = far_clip;

	m_projection_matrix = glm::perspective(glm::radians(fov), aspect_ratio, near_clip, far_clip);
}
