#include "scene.h"

void Scene::Update()
{
	for (const auto& actor : m_actors)
	{
		actor->Update();
	}
}

void Scene::Draw()
{
	for (const auto& actor : m_actors)
	{
		actor->Draw();
	}
}

void Scene::Add(std::unique_ptr<Actor> actor)
{
	m_actors.push_back(std::move(actor));
}