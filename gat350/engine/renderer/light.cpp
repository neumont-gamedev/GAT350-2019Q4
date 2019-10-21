#include "light.h"
#include "program.h"

void Light::SetShader(class Program* program, const glm::mat4& view)
{
	ASSERT(program);

	program->Use();

	program->SetUniform("light.ambient", ambient);
	program->SetUniform("light.diffuse", diffuse);
	program->SetUniform("light.specular", specular);
	program->SetUniform("light.position", position * view);
}
