#include "game_scene.h"
#include "../engine/input/input.h"
#include "../engine/engine.h"
#include "../engine/renderer/renderer.h"
#include "../engine/renderer/program.h"
#include "../engine/renderer/vertex_index_array.h"
#include "../engine/renderer/texture.h"
#include "../engine/renderer/material.h"
#include "../engine/renderer/light.h"
#include "../engine/renderer/mesh.h"
#include "../engine/renderer/model.h"
#include "../engine/renderer/gui.h"
#include "../engine/renderer/camera.h"

bool GameScene::Create(const Name& name)
{
	m_engine->Get<Input>()->AddAction("forward", SDL_SCANCODE_UP);
	m_engine->Get<Input>()->AddAction("backward", SDL_SCANCODE_DOWN);
	m_engine->Get<Input>()->AddAction("pitch", Input::eAxis::Y, Input::MOUSE);
	m_engine->Get<Input>()->AddAction("yaw", Input::eAxis::X, Input::MOUSE);

	std::shared_ptr<Model> model = m_engine->Resources()->Get<Model>("meshes/cube.obj");

	std::unique_ptr<Program> shader = m_engine->Factory()->Create<Program>(Program::GetClassName());
	shader->CreateShaderFromFile("shaders/texture_phong.vert", GL_VERTEX_SHADER);
	shader->CreateShaderFromFile("shaders/texture_phong.frag", GL_FRAGMENT_SHADER);
	shader->Link();
	shader->Use();

	std::unique_ptr<Material> material = m_engine->Factory()->Create<Material>("material");
	material->ambient = glm::vec3(1.0f);
	material->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	material->specular = glm::vec3(1.0f);
	material->shininess = 128.0f;

	std::shared_ptr<Texture> texture = m_engine->Resources()->Get<Texture>("textures/crate.bmp");
	material->textures.push_back(texture);
	material->SetShader(shader.get());
	material->Use();

	m_engine->Resources()->Add("texture_phong", std::move(shader));
	m_engine->Resources()->Add("crate_material", std::move(material));

	std::unique_ptr<Light> light = m_engine->Factory()->Create<Light>(Light::GetClassName());
	light->SetName("light");
	light->GetTransform().translation = glm::vec3(10.0f);
	light->ambient = glm::vec3(0.1f);
	light->diffuse = glm::vec3(1.0f);
	light->specular = glm::vec3(1.0f);
	AddObject(std::move(light));

	std::unique_ptr<Camera> camera = m_engine->Factory()->Create<Camera>(Camera::GetClassName());
	camera->SetName("camera");
	camera->GetTransform() = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera->SetProjection(45.0f, 1280.0f / 720.0f, 0.01f, 100.0f);
	AddObject(std::move(camera));

	return true;
}

void GameScene::Update()
{
	glm::vec3 camera_rotate(0.0f);
	camera_rotate.x = m_engine->Get<Input>()->GetAxisRelative("pitch") * 0.001f;
	camera_rotate.y = m_engine->Get<Input>()->GetAxisRelative("yaw") * 0.001f;

	glm::quat qpitch = glm::angleAxis(camera_rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat qyaw = glm::angleAxis(camera_rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));

	Camera* camera = GetObject<Camera>("camera");
	camera->GetTransform().rotation = qpitch * camera->GetTransform().rotation * qyaw;
	camera->GetTransform().rotation = glm::normalize(camera->GetTransform().rotation);
	//glm::mat4 camera_matrix = camera.GetTransform().rotation


	glm::vec3 translate(0.0f, 0.0f, 0.0f);
	float speed = 10.0f;
	if (m_engine->Get<Input>()->GetButton("forward", Input::HELD))		translate.z = speed;
	if (m_engine->Get<Input>()->GetButton("backward", Input::HELD))		translate.z = -speed;

	//glm::quat r = glm::angleAxis(glm::radians(45.0f) * g_timer.dt(), glm::vec3(0, 1, 0));
	//transform.rotation = transform.rotation * r;
	Transform transform(glm::vec3(0.0f, 0.0f, 0.0f));
	transform.translation = transform.translation + translate * g_timer.dt();

	glm::mat4 model_view_matrix = camera->GetTransform().GetMatrix() * transform.GetMatrix();

	auto shader = m_engine->Resources()->Get<Program>("texture_phong");

	shader->SetUniform("model_view_matrix", model_view_matrix);
	glm::mat4 mvp_matrix = camera->GetProjection() * model_view_matrix;
	shader->SetUniform("mvp_matrix", mvp_matrix);

	// set shader uniforms
	Light* light = GetObject<Light>("light");
	light->SetShader(shader.get(), camera->GetTransform());
	//light->Edit();

	auto material = m_engine->Resources()->Get<Material>("crate_material");
	material->SetShader(shader.get());
	//material->Edit();

	//shader->SetUniform("steps", steps);
	shader->SetUniform("fog.min_distance", 40.0f);
	shader->SetUniform("fog.max_distance", 50.0f);
	shader->SetUniform("fog.color", glm::vec3(0.85f));

	// gui
	GUI::Update(m_engine->GetEvent());
	GUI::Begin(m_engine->Get<Renderer>());
	
	GUI::End();
}

void GameScene::Draw()
{
	m_engine->Get<Renderer>()->ClearBuffer();
	GUI::Draw();

	std::shared_ptr<Model> model = m_engine->Resources()->Get<Model>("meshes/cube.obj");
	model->Draw();

	m_engine->Get<Renderer>()->SwapBuffer();
}
