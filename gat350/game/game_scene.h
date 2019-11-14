#pragma once
#include "../engine/framework/scene.h"

class GameScene : public Scene
{
public:
	OBJECT_DECLARATION(GameScene, Scene)

	bool Create(const Name& name) override;
	void Update() override;
	void Draw() override;

protected:
	glm::vec3 m_scale = glm::vec3(1);
	
	glm::vec2 m_uv_offset = glm::vec2(0, 0);
	glm::vec2 m_uv_scale = glm::vec2(1, 1);

	float m_time = 0.0f;
	float m_amplitude = 0.0f;
	float m_frequency = 1.0f;
	float m_rate = 1.0f;

	glm::vec3 m_bloat_position = glm::vec3(0);
	float m_bloat_scale = 0.0f;
	float m_bloat_range = 1.0f;

	float m_dissolve = 0.0f;
	glm::vec3 m_discard_color = glm::vec3(0);
};