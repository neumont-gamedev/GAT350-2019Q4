#include "material.h"

void Material::Destroy()
{
	if (program) delete program;
	for (Texture* texture : textures)
	{
		delete texture;
	}
}

void Material::Update()
{
	program->Use();

	program->SetUniform("material.ambient", ambient);
	program->SetUniform("material.diffuse", diffuse);
	program->SetUniform("material.specular", specular);
	program->SetUniform("material.shininess", shininess);
}

void Material::Use()
{
	for (Texture* texture : textures)
	{
		texture->Bind();
	}
	program->Use();
}
