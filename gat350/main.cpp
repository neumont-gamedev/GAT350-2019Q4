#include "engine/engine.h"
#include "engine/input/input.h"
#include "engine/renderer/renderer.h"
#include "engine/renderer/program.h"

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

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLubyte*)NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLubyte*)(3 * sizeof(GLfloat)));
	
	Program program;
	program.CreateShaderFromFile("shaders/basic.vs", GL_VERTEX_SHADER);
	program.CreateShaderFromFile("shaders/basic.fs", GL_FRAGMENT_SHADER);
	program.Link();
	program.Use();

	glm::mat4 mx = glm::mat4(1.0f);
	program.SetUniform("mx", mx);

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

		float rotate = input->GetKey(SDL_SCANCODE_SPACE) ? 90.0f : 0.0f;
		mx = glm::rotate(mx, glm::radians(rotate) * g_timer.dt(), glm::vec3(0.0f, 0.0f, 1.0f));
		program.SetUniform("mx", mx);

		renderer->ClearBuffer();

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		renderer->SwapBuffer();
	}
	input->Shutdown();
	renderer->Shutdown();

	SDL_Quit();

	return 0;
}
