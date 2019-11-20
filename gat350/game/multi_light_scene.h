#pragma once
#include "../engine/framework/scene.h"

class MultiLightScene : public Scene
{
public:
	OBJECT_DECLARATION(MultiLightScene, Scene)

	bool Create(const Name& name) override;
	void Update() override;
	void Draw() override;

public:
	glm::vec3 m_scale = glm::vec3(1);

	float m_time = 0.0f;
	float m_amplitude = 0.0f;
	float m_frequency = 0.0f;
	float m_rate = 1.0f;

	glm::vec2 m_uv_scale = glm::vec2(1);
	glm::vec2 m_uv_offset = glm::vec2(0);
};