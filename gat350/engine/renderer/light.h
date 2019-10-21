#pragma once
#include "../engine.h"

struct Light
{
	void SetShader(class Program* program, const glm::mat4& view);

	union
	{
		glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 direction;
	};

	glm::vec3 ambient = glm::vec3(0.0f);
	glm::vec3 diffuse = glm::vec3(1.0f);
	glm::vec3 specular = glm::vec3(1.0f);
};