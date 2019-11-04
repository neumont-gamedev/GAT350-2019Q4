#include "material.h"

bool Material::Create(const Name& name)
{
	return true;
}

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
	for (auto texture : textures)
	{
		texture->Bind();
	}
}

void Material::Edit()
{
	ImGui::PushID("Material");
	ImGui::Text("Material");
	ImGui::ColorEdit3("Ambient", (float*)&ambient);
	ImGui::ColorEdit3("Diffuse", (float*)&diffuse);
	ImGui::ColorEdit3("Specular", (float*)&specular);
	ImGui::SliderFloat("Shininess", &shininess, 1.0f, 128.0f);
	ImGui::PopID();
}
