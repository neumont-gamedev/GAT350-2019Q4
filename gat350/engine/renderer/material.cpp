#include "material.h"

void Material::Destroy()
{
	//
}

void Material::SetShader(class Program* shader)
{
	shader->SetUniform("material.ambient", ambient);
	shader->SetUniform("material.diffuse", diffuse);
	shader->SetUniform("material.specular", specular);
	shader->SetUniform("material.shininess", shininess);
}

void Material::Use()
{
	for (const auto& texture : textures)
	{
		texture->Bind();
	}
}

void Material::Edit()
{
	ImGui::Begin("Material");
	ImGui::ColorEdit3("Ambient", (float*)&ambient);
	ImGui::ColorEdit3("Diffuse", (float*)&diffuse);
	ImGui::ColorEdit3("Specular", (float*)&specular);
	ImGui::SliderFloat("Shininess", &shininess, 0.1f, 300.0f);
	ImGui::End();
}
