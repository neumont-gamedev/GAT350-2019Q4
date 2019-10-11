#include "engine/engine.h"
#include "engine/input/input.h"
#include "engine/renderer/renderer.h"
#include "engine/renderer/program.h"
#include "engine/renderer/vertex_index_array.h"

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
	-1.0, -1.0,  1.0, 1.0, 0.0, 0.0,
	 1.0, -1.0,  1.0, 0.0, 1.0, 0.0,
	 1.0,  1.0,  1.0, 0.0, 0.0, 1.0,
	-1.0,  1.0,  1.0, 1.0, 1.0, 1.0,
	// back
	-1.0, -1.0, -1.0, 1.0, 0.0, 0.0,
	 1.0, -1.0, -1.0, 0.0, 1.0, 0.0,
	 1.0,  1.0, -1.0, 0.0, 0.0, 1.0,
	-1.0,  1.0, -1.0, 1.0, 1.0, 1.0
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
	vertex_array.SetAttribute(VertexArray::POSITION, 3, 6 * sizeof(GLfloat), 0);
	vertex_array.SetAttribute(VertexArray::COLOR, 3, 6 * sizeof(GLfloat), 3 * sizeof(GLfloat));
	
	Program program;
	program.CreateShaderFromFile("shaders/basic.vs", GL_VERTEX_SHADER);
	program.CreateShaderFromFile("shaders/basic.fs", GL_FRAGMENT_SHADER);
	program.Link();
	program.Use();

	glm::mat4 mxModel = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	program.SetUniform("mx", mxModel);

	//glm::mat4 mxProjection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, 0.01f, 1000.0f);
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

		mxModel = glm::translate(mxModel, translate * g_timer.dt());

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
