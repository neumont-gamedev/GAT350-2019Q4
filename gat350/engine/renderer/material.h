#pragma once
#include "../engine.h"
#include "texture.h"
#include "program.h"

class Material
{
public:
	void Destroy();

	void SetShader(class Program* shader);
	void Use();

	void Edit();
public:
	glm::vec3 ambient = glm::vec3(0.0f);
	glm::vec3 diffuse = glm::vec3(1.0f);
	glm::vec3 specular = glm::vec3(1.0f);
	float shininess = 100.0f;

	std::vector<std::shared_ptr<Texture>> textures;
};