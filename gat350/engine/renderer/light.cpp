#include "light.h"
#include "program.h"

void Light::SetShader(class Program* program, const glm::mat4& view_matrix)
{
	ASSERT(program);

	program->SetUniform("light.ambient", ambient);
	program->SetUniform("light.diffuse", diffuse);
	program->SetUniform("light.specular", specular);

	glm::mat4 light_view_matrix = m_transform.GetMatrix() * view_matrix;
	program->SetUniform("light.position", glm::vec4(m_transform.translation, 1.0f) * view_matrix);
}

void Light::Edit()
{
	ImGui::Begin("Light");
	ImGui::ColorEdit3("Ambient", (float*)&ambient);
	ImGui::ColorEdit3("Diffuse", (float*)&diffuse);
	ImGui::ColorEdit3("Specular", (float*)&specular);
	ImGui::End();
}
