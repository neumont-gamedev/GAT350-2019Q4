#include "shadowmap_scene.h"
#include "../engine/engine.h"
#include "../engine/editor/editor.h"
#include "../engine/renderer/renderer.h"
#include "../engine/renderer/program.h"
#include "../engine/renderer/texture.h"
#include "../engine/renderer/material.h"
#include "../engine/renderer/framebuffer.h"
#include "../engine/renderer/light.h"
#include "../engine/renderer/mesh.h"
#include "../engine/renderer/model.h"
#include "../engine/renderer/camera.h"
#include "../engine/renderer/gui.h"

#define SHADOWMAP_SIZE 512

bool ShadowmapScene::Create(const Name& name)
{
	// shader
	//auto shader = m_engine->Factory()->Create<Program>(Program::GetClassName());
	//shader->m_name = "shader";
	//shader->m_engine = m_engine;
	//shader->CreateShaderFromFile("shaders/texture_phong.vert", GL_VERTEX_SHADER);
	//shader->CreateShaderFromFile("shaders/texture_phong_light.frag", GL_FRAGMENT_SHADER);
	//shader->Link();
	//m_engine->Resources()->Add("phong_shader", std::move(shader));

	auto shader = m_engine->Factory()->Create<Program>(Program::GetClassName());
	shader->m_name = "shader";
	shader->m_engine = m_engine;
	shader->CreateShaderFromFile("shaders/phong_shadow.vert", GL_VERTEX_SHADER);
	shader->CreateShaderFromFile("shaders/phong_shadow.frag", GL_FRAGMENT_SHADER);
	shader->Link();
	m_engine->Resources()->Add("phong_shadow", std::move(shader));


	shader = m_engine->Factory()->Create<Program>(Program::GetClassName());
	shader->m_name = "shader";
	shader->m_engine = m_engine;
	shader->CreateShaderFromFile("shaders/shadowmap.vert", GL_VERTEX_SHADER);
	shader->CreateShaderFromFile("shaders/shadowmap.frag", GL_FRAGMENT_SHADER);
	shader->Link();
	m_engine->Resources()->Add("shadowmap", std::move(shader));

	shader = m_engine->Factory()->Create<Program>(Program::GetClassName());
	shader->m_name = "shader";
	shader->m_engine = m_engine;
	shader->CreateShaderFromFile("shaders/depth_debug.vert", GL_VERTEX_SHADER);
	shader->CreateShaderFromFile("shaders/depth_debug.frag", GL_FRAGMENT_SHADER);
	shader->Link();
	m_engine->Resources()->Add("depth_debug", std::move(shader));

	shader = m_engine->Factory()->Create<Program>(Program::GetClassName());
	shader->m_name = "shader";
	shader->m_engine = m_engine;
	shader->CreateShaderFromFile("shaders/basic_color.vert", GL_VERTEX_SHADER);
	shader->CreateShaderFromFile("shaders/basic.frag", GL_FRAGMENT_SHADER);
	shader->Link();
	m_engine->Resources()->Add("debug_shader", std::move(shader));

	// framebuffer
	{
		auto framebuffer = m_engine->Factory()->Create<Framebuffer>(Framebuffer::GetClassName());
		framebuffer->Create("framebuffer");

		auto texture = m_engine->Factory()->Create<Texture>(Texture::GetClassName());
		texture->CreateTexture(SHADOWMAP_SIZE, SHADOWMAP_SIZE, GL_TEXTURE_2D, GL_DEPTH_COMPONENT, GL_TEXTURE1);
		m_engine->Resources()->Add("render_texture", std::move(texture));

		framebuffer->AttachTexture(m_engine->Resources()->Get<Texture>("render_texture"), GL_DEPTH_ATTACHMENT);
		framebuffer->Unbind();
		m_engine->Resources()->Add("framebuffer", std::move(framebuffer));
	}

	// material
	auto material = m_engine->Factory()->Create<Material>(Material::GetClassName());
	material->m_name = "material";
	material->m_engine = m_engine;
	material->ambient = glm::vec3(1);
	material->diffuse = glm::vec3(1);
	material->specular = glm::vec3(1);
	material->shininess = 16.0f;

	// texture
	auto texture = m_engine->Resources()->Get<Texture>("textures/uvgrid.jpg");
	material->textures.push_back(texture);
	texture = m_engine->Resources()->Get<Texture>("render_texture");
	material->textures.push_back(texture);
	m_engine->Resources()->Add("material", std::move(material));

	// render material
	material = m_engine->Factory()->Create<Material>(Material::GetClassName());
	material->m_name = "material";
	material->m_engine = m_engine;
	material->ambient = glm::vec3(1);
	material->diffuse = glm::vec3(1);
	material->specular = glm::vec3(1);
	material->shininess = 16.0f;
	texture = m_engine->Resources()->Get<Texture>("render_texture");
	material->textures.push_back(texture);
	m_engine->Resources()->Add("render_material", std::move(material));
	
	// debug material
	material = m_engine->Factory()->Create<Material>(Material::GetClassName());
	material->m_name = "material";
	material->m_engine = m_engine;
	material->ambient = glm::vec3(1);
	material->diffuse = glm::vec3(1);
	material->specular = glm::vec3(1);
	material->shininess = 16.0f;
	m_engine->Resources()->Add("debug_material", std::move(material));

	// scene actors

	// model
	auto model = m_engine->Factory()->Create<Model>(Model::GetClassName());
	model->m_name = "model1";
	model->m_engine = m_engine;
	model->m_scene = this;
	model->m_transform.translation = glm::vec3(0);
	model->m_transform.scale = glm::vec3(1);
	model->m_mesh = m_engine->Resources()->Get<Mesh>("meshes/suzanne.obj");
	model->m_mesh->m_material = m_engine->Resources()->Get<Material>("material");
	model->m_shader = m_engine->Resources()->Get<Program>("phong_shadow");
	Add(std::move(model));

	model = m_engine->Factory()->Create<Model>(Model::GetClassName());
	model->m_name = "model2";
	model->m_engine = m_engine;
	model->m_scene = this;
	model->m_transform.translation = glm::vec3(0, -2, 0);
	model->m_transform.scale = glm::vec3(20);
	model->m_mesh = m_engine->Resources()->Get<Mesh>("meshes/plane.obj");
	model->m_mesh->m_material = m_engine->Resources()->Get<Material>("material");
	model->m_shader = m_engine->Resources()->Get<Program>("phong_shadow");
	Add(std::move(model));

	model = m_engine->Factory()->Create<Model>(Model::GetClassName());
	model->m_name = "model3";
	model->m_engine = m_engine;
	model->m_scene = this;
	model->m_transform.translation = glm::vec3(0);
	model->m_transform.scale = glm::vec3(1);
	model->m_mesh = m_engine->Resources()->Get<Mesh>("meshes/quad.obj");
	model->m_mesh->m_material = m_engine->Resources()->Get<Material>("render_material");
	model->m_shader = m_engine->Resources()->Get<Program>("depth_debug");
	Add(std::move(model));

	// light
	auto light = m_engine->Factory()->Create<Light>(Light::GetClassName());
	light->m_name = "light";
	light->m_engine = m_engine;
	light->m_scene = this;
	light->Create("light");
	light->m_transform.translation = vec3(2);
	light->m_transform.rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0));
	light->ambient = glm::vec3(0.5f);
	light->diffuse = glm::vec3(1);
	light->specular = glm::vec3(1);
	light->cutoff = 30.0f;
	light->exponent = 8.0f;
	Add(std::move(light));

	// camera
	{
		auto camera = m_engine->Factory()->Create<Camera>(Camera::GetClassName());
		camera->m_name = "camera";
		camera->m_engine = m_engine;
		camera->m_scene = this;
		camera->Create("camera");
		camera->m_transform.translation = glm::vec3(0.0f, 0.0f, 5.0f);
		camera->m_transform.rotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		camera->SetProjection(45.0f, 1280.0f / 720.0f, 0.01f, 100.0f);
		Add(std::move(camera));
	}

	{
		auto camera = m_engine->Factory()->Create<Camera>(Camera::GetClassName());
		camera->m_name = "buffer_camera";
		camera->m_engine = m_engine;
		camera->m_scene = this;
		camera->m_user_camera = false;
		camera->m_view_matrix = glm::lookAt(glm::vec3(0, 3, 2), glm::vec3(0), glm::vec3(0, 1, 0));
		camera->m_projection_matrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 20.0f);
		Add(std::move(camera));
	}

	return true;
}

void ShadowmapScene::Update()
{
	Scene::Update();

	{
		auto model = Get<Model>("model1");
		model->m_transform.rotation = model->m_transform.rotation * glm::angleAxis(-glm::radians(45.0f) * g_timer.dt(), glm::vec3(0, 1, 0));
	}

	// set shader uniforms
	auto light = Get<Light>("light");
	light->m_transform.translation = light->m_transform.translation * glm::angleAxis(glm::radians(45.0f) * g_timer.dt(), glm::vec3(0, 1, 0));


	glm::mat4 bias_matrix(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	);

	glm::mat4 projection_matrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 20.0f);
	glm::mat4 light_view_matrix = glm::lookAt(light->m_transform.translation, glm::vec3(0), glm::vec3(0, 1, 0));
	glm::mat4 lvp_matrix = projection_matrix * light_view_matrix;

	auto shader = m_engine->Resources()->Get<Program>("phong_shadow");
	shader->Use();
	shader->SetUniform("lvp_matrix", bias_matrix * lvp_matrix);
	light->SetShader(shader.get());

	shader = m_engine->Resources()->Get<Program>("shadowmap");
	shader->Use();
	shader->SetUniform("lvp_matrix", lvp_matrix);

	// gui
	GUI::Update(m_engine->GetEvent());
	GUI::Begin(m_engine->Get<Renderer>());

	m_engine->Get<Editor>()->UpdateGUI();

	GUI::End();
}

void ShadowmapScene::Draw()
{
	RenderToTexture();
	RenderScene();

	GUI::Draw();
	m_engine->Get<Renderer>()->SwapBuffer();
}

void ShadowmapScene::RenderToTexture()
{
	auto framebuffer = m_engine->Resources()->Get<Framebuffer>("framebuffer");
	framebuffer->Bind();
	auto texture = m_engine->Resources()->Get<Texture>("render_texture");
	texture->Bind();

	SetActive<Camera>("buffer_camera", true);

	m_engine->Get<Renderer>()->SetViewport(0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE);
	m_engine->Get<Renderer>()->ClearBuffer();

	auto shader = m_engine->Resources()->Get<Program>("shadowmap");
	auto model = Get<Model>("model1");
	model->Draw(shader.get());

	model = Get<Model>("model2");
	model->Draw(shader.get());

	//SetInactive<Model>("model3");
	//Scene::Draw();

	framebuffer->Unbind();
}

void ShadowmapScene::RenderScene()
{
	SetActive<Camera>("camera", true);

	m_engine->Get<Renderer>()->RestoreViewport();
	m_engine->Get<Renderer>()->ClearBuffer();

	Scene::Draw();	
}
