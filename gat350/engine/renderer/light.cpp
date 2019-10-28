#include "light.h"
#include "program.h"

void Light::SetShader(class Program* program, const glm::mat4& view)
{
	ASSERT(program);

	program->SetUniform("light.ambient", ambient);
	program->SetUniform("light.diffuse", diffuse);
	program->SetUniform("light.specular", specular);
	program->SetUniform("light.position", position * view);
}

void Light::Edit()
{
	ImGui::Begin("Light");
	ImGui::ColorEdit3("Ambient", (float*)&ambient);
	ImGui::ColorEdit3("Diffuse", (float*)&diffuse);
	ImGui::ColorEdit3("Specular", (float*)&specular);
	ImGui::End();
}
