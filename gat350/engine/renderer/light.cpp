#include "light.h"
#include "program.h"
#include "camera.h"

void Light::SetShader(Program* program)
{
	ASSERT(program);

	program->Use();
	program->SetUniform("light.ambient", ambient);
	program->SetUniform("light.diffuse", diffuse);
	program->SetUniform("light.specular", specular);

	std::vector<Camera*> cameras = m_scene->Get<Camera>();
	ASSERT(!cameras.empty());

	glm::mat4 light_view_matrix = m_transform.GetMatrix() * cameras[0]->m_view_matrix;
	glm::vec4 position = light_view_matrix[3];
	program->SetUniform("light.position", position);
}

void Light::Edit()
{
	ImGui::Begin("Light");
	ImGui::ColorEdit3("Ambient", (float*)&ambient);
	ImGui::ColorEdit3("Diffuse", (float*)&diffuse);
	ImGui::ColorEdit3("Specular", (float*)&specular);
	ImGui::End();
}
