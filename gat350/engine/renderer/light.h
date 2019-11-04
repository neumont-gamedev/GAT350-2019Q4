#pragma once
#include "../framework/actor.h"

class Light : public Actor
{
public:
	OBJECT_DECLARATION(Light, Actor)
	~Light() {}

	bool Create(const Name& name);
	void Destroy();

	void SetShader(class Program* program, const glm::mat4& view);
	void Edit();

public:
	glm::vec3 ambient = glm::vec3(0.0f);
	glm::vec3 diffuse = glm::vec3(1.0f);
	glm::vec3 specular = glm::vec3(1.0f);
};