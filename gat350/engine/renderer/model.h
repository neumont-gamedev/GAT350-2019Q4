#pragma once
#include "../framework/actor.h"
#include "mesh.h"
#include "program.h"

class Model : public Actor
{
public:
	OBJECT_DECLARATION(Model, Actor)
	~Model() {}

	void Update() override;
	void Draw(GLenum primitiveType = GL_TRIANGLES) override;
	void Draw(Program* shader, GLenum primitiveType = GL_TRIANGLES);
	void Edit() override;

public:
	std::shared_ptr<Mesh> m_mesh;
	std::shared_ptr<Program> m_shader;
};
