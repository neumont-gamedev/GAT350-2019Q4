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
#include "engine/renderer/camera.h"

int main(int argc, char** argv)
{
	filesystem::set_current_path("content");
	Name::AllocNames();

	int result = SDL_Init(SDL_INIT_VIDEO);
	if (result != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
	}

	std::shared_ptr<Input> input = std::make_shared<Input>();
	input->Initialize();
	input->AddAction("forward", SDL_SCANCODE_UP);
	input->AddAction("backward", SDL_SCANCODE_DOWN);
	input->AddAction("pitch", Input::eAxis::Y, Input::MOUSE);
	input->AddAction("yaw", Input::eAxis::X, Input::MOUSE);

	std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>();
	renderer->Initialize(1280, 720, false);

	GUI::Initialize(renderer.get());

	Mesh mesh;
	mesh.Load("meshes/sphere.obj");
	
	Program* shader = new Program();
	shader->CreateShaderFromFile("shaders/texture_phong.vert", GL_VERTEX_SHADER);
	shader->CreateShaderFromFile("shaders/texture_phong_fog.frag", GL_FRAGMENT_SHADER);
	shader->Link();
	shader->Use();

	shader->SetUniform("fog.distance_min", 10.0f);
	shader->SetUniform("fog.distance_max", 30.0f);
	shader->SetUniform("fog.color", glm::vec3(0.8f));
	
	Allocator<Material> material_allocator;

	std::shared_ptr<Material> material = material_allocator.Get("material");
	material->ambient = glm::vec3(1.0f);
	material->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	material->specular = glm::vec3(1.0f);
	material->shininess = 128.0f;

	Allocator<Texture> texture_allocator;
	std::shared_ptr<Texture> texture = texture_allocator.Get("textures/uvgrid.jpg");

	material->textures.push_back(texture);
	material->SetShader(shader);
	material->Use();

	Light light;
	light.Create("light");
	light.GetTransform().translation = glm::vec3(5.0f, 5.0f, 5.0f);
	light.ambient = glm::vec3(0.1f);
	light.diffuse = glm::vec3(1.0f);
	light.specular = glm::vec3(1.0f);

	Transform transform(glm::vec3(0.0f, 0.0f, -5.0f));

	Camera camera;
	camera.GetTransform() = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera.SetProjection(45.0f, 1280.0f / 720.0f, 0.01f, 1000.0f);

	int steps = 4;

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
		input->Update();
		
		//glm::vec3 camera_rotate(0.0f);
		//camera_rotate.x = input->GetAxisRelative("pitch") * 0.001f;
		//camera_rotate.y = input->GetAxisRelative("yaw") * 0.001f;

		//glm::quat qcamera = glm::quat(camera_rotate);
		//glm::quat qpitch = glm::angleAxis(camera_rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
		//glm::quat qyaw = glm::angleAxis(camera_rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));

		////camera.GetTransform().rotation = glm::normalize(camera.GetTransform().rotation * qpitch);
		////camera.GetTransform().rotation = qcamera * camera.GetTransform().rotation;
		//camera.GetTransform().rotation = qpitch * camera.GetTransform().rotation * qyaw;
		////camera.GetTransform().rotation = qpitch * camera.GetTransform().rotation;
		//camera.GetTransform().rotation = glm::normalize(camera.GetTransform().rotation);

		glm::vec3 translate(0.0f, 0.0f, 0.0f);
		float speed = 10.0f;
		if (input->GetButton("forward", Input::HELD))		translate.z = speed;
		if (input->GetButton("backward", Input::HELD))		translate.z = -speed;
		
		glm::quat r = glm::angleAxis(glm::radians(45.0f) * g_timer.dt(), glm::vec3(0, 1, 0));
		transform.rotation = transform.rotation * r;

		glm::mat4 model_view_matrix = camera.GetTransform().GetMatrix() * transform.GetMatrix();
		shader->SetUniform("model_view_matrix", model_view_matrix);
		glm::mat4 mvp_matrix = camera.GetProjection() * model_view_matrix;
		shader->SetUniform("mvp_matrix", mvp_matrix);

		// set shader uniforms
		light.SetShader(shader, camera.GetTransform());
		material->SetShader(shader);

		//shader->SetUniform("steps", steps);
		shader->SetUniform("fog.min_distance", 10.0f);
		shader->SetUniform("fog.max_distance", 30.0f);
		shader->SetUniform("fog.color", glm::vec3(0.85f));

		// gui
		GUI::Update(event);
		GUI::Begin(renderer.get());

		ImGui::Text("Hello World");
		light.Edit();
		material->Edit();
		ImGui::Button("Pause");
		ImGui::SliderInt("Steps", &steps, 1, 16);

		GUI::End();

		// render
		renderer->ClearBuffer();
		GUI::Draw();
		mesh.Draw();
		renderer->SwapBuffer();
	}
	delete shader;
	material->Destroy();

	GUI::Shutdown();
	input->Shutdown();
	renderer->Shutdown();

	Name::FreeNames();

	SDL_Quit();

	return 0;
}
