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

	shader = m_engine->Factory()->Create<Program>(Program::GetClassName());
	shader->m_name = "shader";
	shader->m_engine = m_engine;
	shader->CreateShaderFromFile("shaders/texture_phong_fx.vert", GL_VERTEX_SHADER);
	shader->CreateShaderFromFile("shaders/texture_phong_fx.frag", GL_FRAGMENT_SHADER);
	shader->Link();
	m_engine->Resources()->Add("phong_shader_fx", std::move(shader));

	shader = m_engine->Factory()->Create<Program>(Program::GetClassName());
	shader->m_name = "shader";
	shader->m_engine = m_engine;
	shader->CreateShaderFromFile("shaders/basic_color.vert", GL_VERTEX_SHADER);
	shader->CreateShaderFromFile("shaders/basic.frag", GL_FRAGMENT_SHADER);
	shader->Link();
	m_engine->Resources()->Add("debug_shader", std::move(shader));

	// material
	auto material = m_engine->Factory()->Create<Material>(Material::GetClassName());
	material->m_name = "material";
	material->m_engine = m_engine;
	material->ambient = glm::vec3(1.0f);
	material->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	material->specular = glm::vec3(1.0f);
	material->shininess = 128.0f;

	// texture
	auto texture = m_engine->Resources()->Get<Texture>("textures/ogre_diffuse.bmp");
	material->textures.push_back(texture);
	texture = m_engine->Resources()->Get<Texture>("textures/noise.png");
	texture->m_unit = GL_TEXTURE1;
	material->textures.push_back(texture);
	m_engine->Resources()->Add("material1", std::move(material));

	material = m_engine->Factory()->Create<Material>(Material::GetClassName());
	material->m_name = "material";
	material->m_engine = m_engine;
	material->ambient = glm::vec3(1.0f);
	material->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	material->specular = glm::vec3(1.0f);
	material->shininess = 128.0f;

	// texture
	texture = m_engine->Resources()->Get<Texture>("textures/uvgrid.jpg");
	material->textures.push_back(texture);
	m_engine->Resources()->Add("material2", std::move(material));

	material = m_engine->Factory()->Create<Material>(Material::GetClassName());
	material->m_name = "material";
	material->m_engine = m_engine;
	material->ambient = glm::vec3(1.0f);
	material->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	material->specular = glm::vec3(1.0f);
	material->shininess = 128.0f;
	m_engine->Resources()->Add("debug_material", std::move(material));

	// scene actors

	// model
	auto model = m_engine->Factory()->Create<Model>(Model::GetClassName());
	model->m_name = "model";
	model->m_engine = m_engine;
	model->m_scene = this;
	model->m_transform.translation = glm::vec3(0);
	model->m_transform.scale = glm::vec3(1.0f);
	model->m_mesh = m_engine->Resources()->Get<Mesh>("meshes/ogre.obj");
	model->m_mesh->m_material = m_engine->Resources()->Get<Material>("material1");
	model->m_shader = m_engine->Resources()->Get<Program>("phong_shader_fx");
	Add(std::move(model));

	model = m_engine->Factory()->Create<Model>(Model::GetClassName());
	model->m_name = "floor";
	model->m_engine = m_engine;
	model->m_scene = this;
	model->m_transform.translation = glm::vec3(0, -2, 0);
	model->m_transform.scale = glm::vec3(10);
	model->m_mesh = m_engine->Resources()->Get<Mesh>("meshes/plane.obj");
	model->m_mesh->m_material = m_engine->Resources()->Get<Material>("material2");
	model->m_shader = m_engine->Resources()->Get<Program>("phong_shader");
	Add(std::move(model));

	// light
	auto light = m_engine->Factory()->Create<Light>(Light::GetClassName());
	light->m_name = "light";
	light->m_engine = m_engine;
	light->m_scene = this;
	light->Create("light");
	light->m_transform.translation = glm::vec3(2);
	light->ambient = glm::vec3(0.7f);
	light->diffuse = glm::vec3(1, 1, 1);
	light->specular = glm::vec3(1.0f);
	Add(std::move(light));

	// camera
	auto camera = m_engine->Factory()->Create<Camera>(Camera::GetClassName());
	camera->m_name = "camera";
	camera->m_engine = m_engine;
	camera->m_scene = this;
	camera->Create("camera");
	camera->m_transform.translation = glm::vec3(0.0f, 0.0f, 5.0f);
	camera->m_transform.rotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera->SetProjection(45.0f, 1280.0f / 720.0f, 0.01f, 100.0f);
	
	Add(std::move(camera));

	return true;
}

void GameScene::Update()
{
	Scene::Update();

	//Model* model = Get<Model>("model2");
	//glm::quat r = glm::angleAxis(glm::radians(45.0f) * g_timer.dt(), glm::vec3(0, 1, 0));
	//model->m_transform.rotation = model->m_transform.rotation * r;
	
	// set shader uniforms
	Light* light = Get<Light>("light");
	light->m_transform.translation = light->m_transform.translation * glm::angleAxis(glm::radians(45.0f) * g_timer.dt(), glm::vec3(0, 1, 0));
	light->SetShader(m_engine->Resources()->Get<Program>("phong_shader").get());
	light->SetShader(m_engine->Resources()->Get<Program>("phong_shader_fx").get());

	auto shader = m_engine->Resources()->Get<Program>("phong_shader_fx");

	shader->SetUniform("scale", m_scale);

	//m_uv_offset.y = m_uv_offset.y + g_timer.dt();
	shader->SetUniform("uv_offset", m_uv_offset);
	shader->SetUniform("uv_scale", m_uv_scale);

	//m_time = m_time + g_timer.dt();
	shader->SetUniform("amplitude", m_amplitude);
	shader->SetUniform("frequency", m_frequency);
	shader->SetUniform("time", m_time);
	shader->SetUniform("rate", m_rate);

	shader->SetUniform("dissolve", m_dissolve);
	shader->SetUniform("resolution", glm::vec2(1280, 720));

	shader->SetUniform("bloat_position", m_bloat_position);
	shader->SetUniform("bloat_scale", m_bloat_scale);
	shader->SetUniform("bloat_range", m_bloat_range);

	shader->SetUniform("discard_color", m_discard_color);

	// gui
	GUI::Update(m_engine->GetEvent());
	GUI::Begin(m_engine->Get<Renderer>());

	ImGui::SliderFloat3("scale", glm::value_ptr(m_scale), 0, 10);

	ImGui::SliderFloat2("uv offset", glm::value_ptr(m_uv_offset), -10, 10);
	ImGui::SliderFloat2("uv scale", glm::value_ptr(m_uv_scale), 0, 10);

	ImGui::SliderFloat("amplitude", &m_amplitude, 0, 2);
	ImGui::SliderFloat("frequency", &m_frequency, 0, 10);
	ImGui::SliderFloat("rate", &m_rate, 0, 10);

	ImGui::SliderFloat("dissolve", &m_dissolve, 0, 1);

	ImGui::SliderFloat3("bloat_position", glm::value_ptr(m_bloat_position), -2, 2);
	ImGui::SliderFloat("bloat_scale", &m_bloat_scale, 0, 1);
	ImGui::SliderFloat("bloat_range", &m_bloat_range, 0, 5);

	ImGui::ColorEdit3("discard_color", glm::value_ptr(m_discard_color));
	   
	GUI::End();
}

void GameScene::Draw()
{
	m_engine->Get<Renderer>()->ClearBuffer();

	GUI::Draw();
	Scene::Draw();

	m_engine->Get<Renderer>()->SwapBuffer();
}
