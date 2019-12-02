#include "framebuffer_scene.h"
#include "../engine/engine.h"
#include "../engine/editor/editor.h"
#include "../engine/renderer/renderer.h"
#include "../engine/renderer/program.h"
#include "../engine/renderer/texture.h"
#include "../engine/renderer/framebuffer.h"
#include "../engine/renderer/material.h"
#include "../engine/renderer/light.h"
#include "../engine/renderer/mesh.h"
#include "../engine/renderer/model.h"
#include "../engine/renderer/camera.h"
#include "../engine/renderer/gui.h"

bool FramebufferScene::Create(const Name& name)
{
	// shader
	auto shader = m_engine->Factory()->Create<Program>(Program::GetClassName());
	shader->m_name = "shader";
	shader->m_engine = m_engine;
	shader->CreateShaderFromFile("shaders/texture_phong.vert", GL_VERTEX_SHADER);
	shader->CreateShaderFromFile("shaders/texture_phong_light.frag", GL_FRAGMENT_SHADER);
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

	// framebuffer
	{
		auto framebuffer = m_engine->Factory()->Create<Framebuffer>(Framebuffer::GetClassName());
		framebuffer->Create("framebuffer");
		framebuffer->CreateDepthbuffer(512, 512);

		auto texture = m_engine->Factory()->Create<Texture>(Texture::GetClassName());
		texture->CreateTexture(512, 512);
		m_engine->Resources()->Add("render_texture", std::move(texture));
				
		framebuffer->AttachTexture(m_engine->Resources()->Get<Texture>("render_texture"));
		framebuffer->Unbind();
		m_engine->Resources()->Add("framebuffer", std::move(framebuffer));
	}

	// material
	{
		auto material = m_engine->Factory()->Create<Material>(Material::GetClassName());
		material->m_name = "material";
		material->m_engine = m_engine;
		material->ambient = glm::vec3(1.0f);
		material->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
		material->specular = glm::vec3(1.0f);
		material->shininess = 8.0f;

		// texture
		auto texture = m_engine->Resources()->Get<Texture>("textures/uvgrid.jpg");
		material->textures.push_back(texture);
		m_engine->Resources()->Add("material", std::move(material));
	}
	
	{
		auto material = m_engine->Factory()->Create<Material>(Material::GetClassName());
		material->m_name = "material";
		material->m_engine = m_engine;
		material->ambient = glm::vec3(1.0f);
		material->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
		material->specular = glm::vec3(1.0f);
		material->shininess = 16.0f;

		// texture
		auto texture = m_engine->Resources()->Get<Texture>("render_texture");
		material->textures.push_back(texture);
		m_engine->Resources()->Add("render_material", std::move(material));
	}

	{
		auto material = m_engine->Factory()->Create<Material>(Material::GetClassName());
		material->m_name = "material";
		material->m_engine = m_engine;
		material->ambient = glm::vec3(1.0f);
		material->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
		material->specular = glm::vec3(1.0f);
		material->shininess = 126.0f;
		m_engine->Resources()->Add("debug_material", std::move(material));
	}

	// scene actors

	// model
	{
		auto model = m_engine->Factory()->Create<Model>(Model::GetClassName());
		model->m_name = "model1";
		model->m_engine = m_engine;
		model->m_scene = this;
		model->m_transform.translation = glm::vec3(0);
		model->m_transform.scale = glm::vec3(1);
		model->m_mesh = m_engine->Resources()->Get<Mesh>("meshes/suzanne.obj");
		model->m_mesh->m_material = m_engine->Resources()->Get<Material>("material");
		model->m_shader = m_engine->Resources()->Get<Program>("phong_shader");
		Add(std::move(model));
	}

	{
		auto model = m_engine->Factory()->Create<Model>(Model::GetClassName());
		model->m_name = "model2";
		model->m_engine = m_engine;
		model->m_scene = this;
		model->m_transform.translation = glm::vec3(0);
		model->m_transform.scale = glm::vec3(1);
		model->m_mesh = m_engine->Resources()->Get<Mesh>("meshes/cube.obj");
		model->m_mesh->m_material = m_engine->Resources()->Get<Material>("render_material");
		model->m_shader = m_engine->Resources()->Get<Program>("phong_shader");
		Add(std::move(model));
	}

	// light
	auto light = m_engine->Factory()->Create<Light>(Light::GetClassName());
	light->m_name = "light";
	light->m_engine = m_engine;
	light->m_scene = this;
	light->Create("light");
	light->m_transform.translation = glm::vec3(2);
	light->m_transform.rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0));
	light->ambient = glm::vec3(0.5f);
	light->diffuse = glm::vec3(1);
	light->specular = glm::vec3(1.0f);
	light->cutoff = 30.0f;
	light->exponent = 64.0f;
	Add(std::move(light));

	// camera
	auto camera = m_engine->Factory()->Create<Camera>(Camera::GetClassName());
	camera->m_name = "camera";
	camera->m_engine = m_engine;
	camera->m_scene = this;
	camera->Create("camera");
	camera->m_transform.translation = glm::vec3(0.0f, 0.0f, 5.0f);
	camera->m_transform.rotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera->SetProjection(45.0f, m_engine->Get<Renderer>()->GetWindowWidth() / static_cast<float>(m_engine->Get<Renderer>()->GetWindowHeight()), 0.01f, 100.0f);
	Add(std::move(camera));

	return true;
}

void FramebufferScene::Update()
{
	Scene::Update();

	{
		auto model = Get<Model>("model1");
		model->m_transform.rotation = model->m_transform.rotation * glm::angleAxis(glm::radians(45.0f) * g_timer.dt(), glm::vec3(0, 1, 0));
	}

	{
		auto model = Get<Model>("model2");
		model->m_transform.rotation = model->m_transform.rotation * glm::angleAxis(glm::radians(45.0f) * g_timer.dt(), glm::vec3(0, 1, 0));
	}

	// set shader uniforms
	auto shader = m_engine->Resources()->Get<Program>("phong_shader");

	auto light = Get<Light>("light");
	light->m_transform.translation = light->m_transform.translation * glm::angleAxis(glm::radians(45.0f) * g_timer.dt(), glm::vec3(0, 1, 0));
	light->SetShader(shader.get());

	// gui
	GUI::Update(m_engine->GetEvent());
	GUI::Begin(m_engine->Get<Renderer>());

	m_engine->Get<Editor>()->UpdateGUI();

	GUI::End();
}

void FramebufferScene::Draw()
{
	RenderToTexture();
	RenderScene();

	GUI::Draw();
	m_engine->Get<Renderer>()->SwapBuffer();
}

void FramebufferScene::RenderToTexture()
{
	auto framebuffer = m_engine->Resources()->Get<Framebuffer>("framebuffer");
	framebuffer->Bind();

	auto camera = Get<Camera>("camera");
	camera->SetProjection(45.0f, 1.0f, 0.01f, 100.0f);

	m_engine->Get<Renderer>()->SetViewport(0, 0, 512, 512);
	m_engine->Get<Renderer>()->ClearBuffer();

	auto model = Get<Model>("model1");
	model->Draw();

	framebuffer->Unbind();
}

void FramebufferScene::RenderScene()
{
	auto camera = Get<Camera>("camera");
	camera->SetProjection(45.0f, m_engine->Get<Renderer>()->GetWindowWidth() / static_cast<float>(m_engine->Get<Renderer>()->GetWindowHeight()), 0.01f, 100.0f);

	m_engine->Get<Renderer>()->RestoreViewport();
	m_engine->Get<Renderer>()->ClearBuffer();
	
	auto model = Get<Model>("model2");
	model->Draw();
}
