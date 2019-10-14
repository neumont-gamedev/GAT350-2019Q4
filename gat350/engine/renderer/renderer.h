#pragma once

#include "../system.h"

class Renderer : public System
{
public:
	Renderer() {}
	~Renderer() {}

	bool Initialize();
	bool Initialize(u32 width, u32 height, bool fullscreen = false);
	void Shutdown();
	void Update();

	void ClearBuffer();
	void SwapBuffer();

private:
	SDL_Window* m_window = nullptr;
	SDL_GLContext m_context;
};
