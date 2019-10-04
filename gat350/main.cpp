#include "engine/renderer/program.h"
#include "core/core.h"

#include <sdl.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const GLfloat positions[] =
{
	-0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f
};

const GLfloat colors[] =
{
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

const GLfloat vertices[] =
{
	-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
	 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
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

	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
	if (window == nullptr)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetSwapInterval(1);

	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (!gladLoadGL()) {
		exit(-1);
	}

	//
	GLuint position_vbo;
	glGenBuffers(1, &position_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, position_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	
	GLuint color_vbo;
	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	GLuint vertex_vbo;
	glGenBuffers(1, &vertex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, position_vbo);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	//glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLubyte*)NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLubyte*)(3 * sizeof(GLfloat)));
	
	//Program program;
	//program.CompileShaderFromFile("content/shaders/basic.vs", GL_VERTEX_SHADER);
	//program.CompileShaderFromFile("content/shaders/basic.fs", GL_FRAGMENT_SHADER);
	//program.Link();
	//program.Use();

	std::string vertex_source;
	filesystem::read_file("shaders/basic.vs", vertex_source);
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	const char* c_str = vertex_source.c_str();
	glShaderSource(vertex_shader, 1, &c_str, 0);
	glCompileShader(vertex_shader);

	std::string fragment_source;
	filesystem::read_file("shaders/basic.fs", fragment_source);
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	c_str = fragment_source.c_str();
	glShaderSource(fragment_shader, 1, &c_str, 0);
	glCompileShader(fragment_shader);

	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	glUseProgram(shader_program);

	glm::mat4 mx = glm::mat4(1.0f);
	//program.SetUniform("mx", mx);
	
	GLint uniform = glGetUniformLocation(shader_program, "mx");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, &mx[0][0]);

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

		mx = glm::rotate(mx, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//program.SetUniform("mx", mx);
		GLint uniform = glGetUniformLocation(shader_program, "mx");
		glUniformMatrix4fv(uniform, 1, GL_FALSE, &mx[0][0]);

		glClearColor(0.85f, 0.85f, 0.85f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
