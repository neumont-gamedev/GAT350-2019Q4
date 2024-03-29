#pragma once
#include "../framework/actor.h"

class Camera : public Actor
{
public:
	OBJECT_DECLARATION(Camera, Actor)
	~Camera() {}

	virtual bool Create(const Name& name);
	virtual void Destroy();

	virtual void Update();

	void SetProjection(float fov, float aspect_ratio, float near_clip, float far_clip);
	glm::mat4& GetProjection() { return m_projection; }
	glm::mat4& GetView() { return m_view; }

protected:
	float m_fov;
	float m_aspect_ratio;
	float m_near_clip;
	float m_far_clip;

	glm::mat4 m_projection;
	glm::mat4 m_view;
};