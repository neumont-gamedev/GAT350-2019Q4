#include "engine/engine.h"
#include "engine/input/input.h"
#include "engine/renderer/renderer.h"
#include "engine/renderer/program.h"
#include "engine/renderer/vertex_index_array.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/material.h"
#include "engine/renderer/light.h"
#include "engine/renderer/mesh.h"
#include "engine/renderer/gui.h"
#include "engine/math/math.h"

class Data
{
public:
	Data() { SDL_Log("constructor"); }
	~Data() { SDL_Log("destructor"); }

	int i = 0;
};

int main(int argc, char** argv)
{
	//{
	//	std::shared_ptr<Data> sdata1 = std::make_shared<Data>();
	//	{
	//		std::weak_ptr<Data> wdata = sdata1;
	//		std::shared_ptr<Data> sdata2 = sdata1;

	//		//std::unique_ptr<Data> data = std::make_unique<Data>();
	//		//if (data)
	//		//{
	//		//	int i = data->i;
	//		//}
	//	}
	//	int c = sdata1.use_count();
	//}


	//////////////////////////////////////////////
	Name::AllocNames();
	filesystem::set_current_path("content");

	int result = SDL_Init(SDL_INIT_VIDEO);
	if (result != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
	}

	std::shared_ptr<Input> input = std::make_shared<Input>();
	input->Initialize();

	std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>();
	renderer->Initialize(1280, 720, false);

	GUI::Initialize(renderer.get());

	Mesh mesh;
	mesh.Load("meshes/cube.obj");
	Transform transform;
	
	std::unique_ptr<Program> shader = std::make_unique<Program>();
	shader->CreateShaderFromFile("shaders/texture_phong.vert", GL_VERTEX_SHADER);
	shader->CreateShaderFromFile("shaders/texture_phong_specular.frag", GL_FRAGMENT_SHADER);
	shader->Link();
	shader->Use();

	shader->SetUniform("fog.distance_min", 10.0f);
	shader->SetUniform("fog.distance_max", 30.0f);
	shader->SetUniform("fog.color", glm::vec3(0.8f));
	
	Material material;
	material.ambient = glm::vec3(1.0f);
	material.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	material.specular = glm::vec3(1.0f);
	material.shininess = 128.0f;

	std::unique_ptr<Texture> texture = std::make_unique<Texture>();
	texture->CreateTexture("textures/metal-diffuse.png", GL_TEXTURE_2D, GL_TEXTURE0);
	material.textures.push_back(std::move(texture));

	texture = std::make_unique<Texture>();
	texture->CreateTexture("textures/specular.png", GL_TEXTURE_2D, GL_TEXTURE1);
	material.textures.push_back(std::move(texture));

	material.SetShader(shader.get());
	material.Use();

	Light light("light");
	light.GetTransform().translation = glm::vec3(5.0f);
	light.ambient = glm::vec3(0.1f);
	light.diffuse = glm::vec3(1.0f);
	light.specular = glm::vec3(1.0f);
	
	glm::mat4 mxView = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mxProjection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.01f, 1000.0f);

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
		
		glm::mat4 mx = transform;

		transform.translation = transform.translation + translate * g_timer.dt();
		transform.Rotate(glm::angleAxis(glm::radians(45.0f) * g_timer.dt(), glm::vec3(0.0f, 1.0f, 0.0f)));

		glm::mat4 model_view_matrix = mxView * transform.GetMatrix();
		shader->SetUniform("model_view_matrix", model_view_matrix);
		glm::mat4 mvp_matrix = mxProjection * model_view_matrix;
		shader->SetUniform("mvp_matrix", mvp_matrix);

		// set shader uniforms
		light.SetShader(shader.get(), mxView);
		material.SetShader(shader.get());

		// gui
		GUI::Update(event);
		GUI::Begin(renderer.get());

		ImGui::Text("Hello World");
		light.Edit();
		material.Edit();
		ImGui::Button("Pause");

		GUI::End();

		// render
		renderer->ClearBuffer();
		GUI::Draw();
		mesh.Draw();
		renderer->SwapBuffer();
	}
	material.Destroy();

	GUI::Shutdown();
	input->Shutdown();
	renderer->Shutdown();

	SDL_Quit();

	Name::FreeNames();

	return 0;
}
