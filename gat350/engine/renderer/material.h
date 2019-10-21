#pragma once
#include "../engine.h"
#include "texture.h"
#include "program.h"

struct Material
{
	void Destroy();

	void Update();
	void Use();

	glm::vec3 ambient = glm::vec3(0.0f);
	glm::vec3 diffuse = glm::vec3(1.0f);
	glm::vec3 specular = glm::vec3(1.0f);
	float shininess = 100.0f;
	
	Program* program;
	std::vector<Texture*> textures;
};