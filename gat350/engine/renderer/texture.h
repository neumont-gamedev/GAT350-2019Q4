#pragma once
#include "../framework/resource.h"

class Texture : public Resource
{
public:
	OBJECT_DECLARATION(Texture, Resource)
	~Texture();

	void CreateTexture(const std::string& filename, GLenum type = GL_TEXTURE_2D, GLuint unit = GL_TEXTURE0);
	void Bind();

	static u8* LoadImage(const std::string& filename, int& width, int& height, int& components);

protected:
	GLenum m_type = GL_TEXTURE_2D;
	GLuint m_unit = GL_TEXTURE0;
	GLuint m_texture = 0;
};