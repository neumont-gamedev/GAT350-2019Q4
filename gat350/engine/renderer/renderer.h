#pragma once

class Renderer
{
public:
	Renderer() {}
	~Renderer() {}

	bool Initialize();
	void Shutdown();

	void ClearBuffer();
	void SwapBuffer();

private:
	// window*
	// context
};