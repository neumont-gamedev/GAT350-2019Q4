#include "game_scene.h"
#include "../engine/engine.h"
#include "../engine/renderer/renderer.h"
#include "../engine/renderer/program.h"
#include "../engine/renderer/texture.h"
#include "../engine/renderer/material.h"
#include "../engine/renderer/light.h"
#include "../engine/renderer/mesh.h"
#include "../engine/renderer/model.h"
#include "../engine/renderer/camera.h"
#include "../engine/renderer/gui.h"

bool GameScene::Create(const Name& name)
{
	// shader
	auto shader = m_engine->Factory()->Create<Program>(Program::GetClassName());
	shader->m_name = "shader";
	shader->m_engine = m_engine;
	shader->CreateShaderFromFile("shaders/texture_phong.vert", GL_VERTEX_SHADER);
	shader->CreateShaderFromFile("shaders/texture_phong.frag", GL_FRAGMENT_SHADER);
	shader->Link();
	m_engine->Resources()->Add("phong_shader", std::move(shader));

	// material
	auto material = m_engine->Factory()->Create<Material>(Material::GetClassName());
	material->m_name = "material";
	material->m_engine = m_engine;
	material->ambient = glm::vec3(1.0f);
	material->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	material->specular = glm::vec3(1.0f);
	material->shininess = 128.0f;

	// texture
	auto texture = m_engine->Resources()->Get<Texture>("textures/uvgrid.jpg");
	material->textures.push_back(texture);
	m_engine->Resources()->Add("material", std::move(material));

	// scene actors

	// model
	auto model = m_engine->Factory()->Create<Model>(Model::GetClassName());
	model->m_name = "model";
	model->m_engine = m_engine;
	model->m_scene = this;
	model->m_transform.translation = glm::vec3(0.0f);
	model->m_transform.scale = glm::vec3(0.5f);
	model->m_mesh = m_engine->Resources()->Get<Mesh>("meshes/sphere.obj");
	model->m_mesh->m_material = m_engine->Resources()->Get<Material>("material");
	model->m_shader = m_engine->Resources()->Get<Program>("phong_shader");
	Add(std::move(model));

	// light
	auto light = m_engine->Factory()->Create<Light>(Light::GetClassName());
	light->m_name = "light";
	light->m_engine = m_engine;
	light->m_scene = this;
	light->m_transform.translation = glm::vec3(10.0f);
	light->ambient = glm::vec3(0.1f);
	light->diffuse = glm::vec3(1.0f);
	light->specular = glm::vec3(1.0f);
	Add(std::move(light));

	// camera
	auto camera = m_engine->Factory()->Create<Camera>(Camera::GetClassName());
	camera->m_name = "camera";
	camera->m_engine = m_engine;
	camera->m_scene = this;
	camera->m_transform.translation = glm::vec3(0.0f, 0.0f, 5.0f);
	camera->m_transform.qrotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera->SetProjection(45.0f, 1280.0f / 720.0f, 0.01f, 100.0f);
	
	Add(std::move(camera));

	return true;
}

void GameScene::Update()
{
	Scene::Update();

	Model* model = Get<Model>("model");
	glm::quat r = glm::angleAxis(glm::radians(45.0f) * g_timer.dt(), glm::vec3(0, 1, 0));
	model->m_transform.qrotation = model->m_transform.qrotation * r;
	
	// set shader uniforms
	Light* light = Get<Light>("light");
	light->SetShader(m_engine->Resources()->Get<Program>("phong_shader").get());

	// gui
	GUI::Update(m_engine->GetEvent());
	GUI::Begin(m_engine->Get<Renderer>());
	GUI::End();
}

void GameScene::Draw()
{
	m_engine->Get<Renderer>()->ClearBuffer();

	GUI::Draw();
	Scene::Draw();

	m_engine->Get<Renderer>()->SwapBuffer();
}
