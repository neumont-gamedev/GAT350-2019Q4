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

struct Transform
{
	glm::vec3 translation;
	glm::mat4 matrix;

	void Update()
	{
		matrix = glm::translate(glm::mat4(1.0f), translation);
	}
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

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

	//glm::mat4 mxProjection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);
	glm::mat4 mxProjection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.01f, 100.0f);
	glm::vec3 eye = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::mat4 mxView = glm::lookAt(eye, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	Transform transform;
	transform.translation = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 mxModel = glm::mat4(1.0f);
	//mxModel = glm::scale(mxModel, glm::vec3(1.0f));
	//glm::mat4 mx = mxProjection * mxView * mxModel;

	transform.Update();
	glm::mat4 mx = mxProjection * mxView * transform.matrix;
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

		//float rotate = input->GetKey(SDL_SCANCODE_SPACE) ? 90.0f : 0.0f;
		//mxModel = glm::rotate(mxModel, glm::radians(rotate) * g_timer.dt(), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 translate(0.0f, 0.0f, 0.0f);
		float speed = 10.0f;
		if (input->GetKey(SDL_SCANCODE_RIGHT))	translate.x =  speed;
		if (input->GetKey(SDL_SCANCODE_LEFT))	translate.x = -speed;
		if (input->GetKey(SDL_SCANCODE_UP))		translate.y =  speed;
		if (input->GetKey(SDL_SCANCODE_DOWN))	translate.y = -speed;
		if (input->GetKey(SDL_SCANCODE_W))		translate.z =  speed;
		if (input->GetKey(SDL_SCANCODE_S))		translate.z = -speed;


		//mxModel = glm::translate(mxModel, translate * g_timer.dt());
		eye = eye + translate * g_timer.dt();
		mxView = glm::lookAt(eye, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		//glm::mat4 mx = mxProjection * mxView * mxModel;
		glm::mat4 mx = mxProjection * mxView * transform.matrix;
		program.SetUniform("mx", mx);

		renderer->ClearBuffer();

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, sizeof(cube_elements) / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);

		renderer->SwapBuffer();
	}
	input->Shutdown();
	renderer->Shutdown();

	SDL_Quit();

	return 0;
}
