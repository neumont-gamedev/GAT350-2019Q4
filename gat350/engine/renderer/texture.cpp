#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define BMP_HEADER_SIZE 54

Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
}

bool Texture::Create(const Name& name)
{
	CreateTexture(name.c_str());

	return true;
}

void Texture::CreateTexture(const std::string& filename, GLenum type, GLuint unit)
{
	m_type = type;
	m_unit = unit;

	int width;
	int height;
	int channels;

	u8* data = LoadImage(filename, width, height, channels);
	ASSERT(data);

	glGenTextures(1, &m_texture);
	glBindTexture(type, m_texture);

	GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

#ifdef STB_IMAGE_IMPLEMENTATION
	stbi_image_free(data);
#else
	delete data;
#endif
}

void Texture::CreateTexture(u32 width, u32 height, GLenum format, GLenum type, GLuint unit)
{
	m_type = type;
	m_unit = unit;

	glGenTextures(1, &m_texture);
	glBindTexture(type, m_texture);

	glTexImage2D(type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::CreateCubeTexture(const std::vector<std::string>& filenames, GLuint unit)
{
	m_type = GL_TEXTURE_CUBE_MAP;
	m_unit = unit;

	glGenTextures(1, &m_texture);
	glBindTexture(m_type, m_texture);

	int width;
	int height;
	int channels;

	GLuint targets[] =
	{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (size_t i = 0; i < filenames.size(); i++)
	{
		u8* data = LoadImage(filenames[i], width, height, channels);
		ASSERT(data);

		GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(targets[i], 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

#ifdef STB_IMAGE_IMPLEMENTATION
		stbi_image_free(data);
#else
		delete data;
#endif
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

//void Texture::CreateDepthTexture(u32 width, u32 height)
//{
//	glGenTextures(1, &m_texture);
//	glBindTexture(GL_TEXTURE_2D, m_texture);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//}
//
//void Texture::CreateFramebuffer(u32 width, u32 height, GLenum format)
//{
//	glGenFramebuffers(1, &m_framebuffer);
//	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
//
//	CreateTexture(width, height, format);
//
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
//
//	glGenRenderbuffers(1, &m_depthbuffer);
//	glBindRenderbuffer(GL_RENDERBUFFER, m_depthbuffer);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
//
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthbuffer);
//
//	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
//	glDrawBuffers(1, drawBuffers);
//
//	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//	ASSERT(result == GL_FRAMEBUFFER_COMPLETE);
//
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}

void Texture::Bind()
{
	glActiveTexture(m_unit);
	glBindTexture(m_type, m_texture);
}

std::vector<std::string> Texture::GenerateCubeMapNames(const std::string& basename, const std::vector<std::string>& suffixes, const std::string& extension)
{
	std::vector<std::string> names;
	for (size_t i = 0; i < 6; i++)
	{
		std::string name = basename + suffixes[i] + extension;
		names.push_back(name);
	}

	return names;
}

#ifdef STB_IMAGE_IMPLEMENTATION
u8* Texture::LoadImage(const std::string& filename, int& width, int& height, int& channels)
{
	stbi_set_flip_vertically_on_load(false);
	u8* image = stbi_load(filename.c_str(), &width, &height, &channels, 0);

	return image;
}
#else
u8* Texture::LoadImage(const std::string& filename, int& width, int& height, int& bpp)
{
	u8* image = nullptr;

	std::ifstream stream(filename, std::ios::binary);
	if (stream.is_open())
	{
		char header[BMP_HEADER_SIZE];
		size_t data_offset = 0;
		size_t image_size = 0;

		stream.read(header, BMP_HEADER_SIZE);
		ASSERT(header[0] == 'B' && header[1] == 'M');

		width		= *(int*) & (header[0x12]);
		height		= *(int*) & (header[0x16]);
		bpp			= *(int*) & (header[0x1C]);
		image_size	= *(int*) & (header[0x22]);
		data_offset	= *(int*) & (header[0x0A]);

		image = new u8[image_size];
		stream.seekg(data_offset);
		stream.read((char*)image, image_size);

		stream.close();
	}

	return image;
}
#endif 