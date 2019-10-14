#include "engine/engine.h"
#include "engine/input/input.h"
#include "engine/renderer/renderer.h"
#include "engine/renderer/program.h"
#include "engine/renderer/vertex_index_array.h"
#include "engine/renderer/texture.h"

const GLfloat vertices[] = {
	-1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top-left
	 1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
	 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
	-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f  // bottom-left
};

const GLuint indices[] = {
	0, 1, 2,
	2, 3, 0
};

const GLfloat cube_vertices[] = {
	// front
	-1.0, -1.0,  1.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	 1.0, -1.0,  1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
	 1.0,  1.0,  1.0, 0.0, 0.0, 1.0, 1.0, 1.0,
	-1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 0.0, 1.0,
	// back
	-1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 1.0, 0.0,
	 1.0, -1.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0,
	 1.0,  1.0, -1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 
	-1.0,  1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

GLushort cube_elements[] = {
	// front
	0, 1, 2,
	2, 3, 0,
	// right
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// left
	4, 0, 3,
	3, 7, 4,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// top
	3, 2, 6,
	6, 7, 3
};

//static GLfloat cube_vertices[] = {
//	// Front
//	-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
//	 1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
//	 1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
//	-1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
//	// Right
//	 1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
//	 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
//	 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
//	 1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
//	// Back
//	-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
//	-1.0f,  1.0f, -1.0f, 1.0f, 0.0f,
//	 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
//	 1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
//	// Left
//	-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
//	-1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
//	-1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
//	-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
//	// Bottom
//	-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
//	-1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
//	 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
//	 1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
//	// Top
//	-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
//	 1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
//	 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
//	-1.0f,  1.0f, -1.0f, 0.0f, 1.0f
//};
//
//static GLushort cube_elements[] =
//{
//	 0,  1,  2,  0,  2,  3,
//	 4,  5,  6,  4,  6,  7,
//	 8,  9, 10,  8, 10, 11,
//	12, 13, 14, 12, 14, 15,
//	16, 17, 18, 16, 18, 19,
//	20, 21, 22, 20, 22, 23
//};


int main(int argc, char** argv)
{
	filesystem::set_current_path("content");

	int result = SDL_Init(SDL_INIT_VIDEO);
	if (result != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
	}

	std::shared_ptr<Input> input = std::make_shared<Input>();
	input->Initialize();

	std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>();
	renderer->Initialize(800, 600);

	VertexIndexArray vertex_array;
	vertex_array.CreateBuffer(VertexArray::MULTI, sizeof(cube_vertices), sizeof(cube_vertices) / sizeof(GLfloat), (void*)cube_vertices);
	vertex_array.CreateIndexBuffer(GL_UNSIGNED_SHORT, sizeof(cube_elements) / sizeof(GLushort), (void*)cube_elements);
	vertex_array.SetAttribute(VertexArray::POSITION, 3, 8 * sizeof(GLfloat), 0);
	vertex_array.SetAttribute(VertexArray::COLOR, 3, 8 * sizeof(GLfloat), 3 * sizeof(GLfloat));
	vertex_array.SetAttribute(VertexArray::TEXCOORD, 2, 8 * sizeof(GLfloat), 6 * sizeof(GLfloat));
	
	Program program;
	program.CreateShaderFromFile("shaders/texture_unlit.vs", GL_VERTEX_SHADER);
	program.CreateShaderFromFile("shaders/texture_unlit.fs", GL_FRAGMENT_SHADER);
	program.Link();
	program.Use();

	//Texture texture;
	//texture.CreateTexture("textures/nc.bmp");
	//texture.Bind();

	int width, height, bpp;
	u8* data = Texture::LoadImage("textures/nc.bmp", width, height, bpp);

	GLuint texture;
	glGenTextures(1, &texture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	delete data;
	
	glm::mat4 mxTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
	glm::mat4 mxRotate = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mxProjection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.01f, 1000.0f);

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}

		SDL_PumpEvents();

		g_timer.tick();
		
		glm::vec3 translate(0.0f, 0.0f, 0.0f);
		float speed = 10.0f;
		if (input->GetKey(SDL_SCANCODE_RIGHT))	translate.x = speed;
		if (input->GetKey(SDL_SCANCODE_LEFT))	translate.x = -speed;
		if (input->GetKey(SDL_SCANCODE_UP))		translate.y = speed;
		if (input->GetKey(SDL_SCANCODE_DOWN))	translate.y = -speed;
		if (input->GetKey(SDL_SCANCODE_W))		translate.z = speed;
		if (input->GetKey(SDL_SCANCODE_S))		translate.z = -speed;


		mxTranslate = glm::translate(mxTranslate, translate * g_timer.dt());
		mxRotate = glm::rotate(mxRotate, glm::radians(45.0f) * g_timer.dt(), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 mxModel = mxTranslate * mxRotate;
		glm::mat4 mx = mxProjection * mxModel;
		program.SetUniform("mx", mx);

		renderer->ClearBuffer();

		vertex_array.Draw();

		renderer->SwapBuffer();
	}
	input->Shutdown();
	renderer->Shutdown();

	SDL_Quit();

	return 0;
}
