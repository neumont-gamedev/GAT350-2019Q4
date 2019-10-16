#include "engine.h"
#include "input/input.h"
#include "renderer/renderer.h"

bool Engine::Initialize()
{
	std::shared_ptr<Input> input = std::make_shared<Input>(this);
	input->Initialize();
	m_systems.push_back(input);

	std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(this);
	renderer->Initialize(800, 600);
	m_systems.push_back(renderer);

	return true;
}

void Engine::Shutdown()
{
	for (std::shared_ptr<System> system : m_systems)
	{
		system->Shutdown();
	}
}

void Engine::Update()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		//quit = true;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
		{
			//quit = true;
		}
	}
	SDL_PumpEvents();

	g_timer.tick();

	for (std::shared_ptr<System> system : m_systems)
	{
		system->Update();
	}
}
