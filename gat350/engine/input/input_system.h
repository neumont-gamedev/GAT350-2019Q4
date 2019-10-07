#pragma once

#include <SDL.h>

class InputSystem
{
public:
	InputSystem() {}
	~InputSystem() {}

	bool Initialize();
	void Shutdown();

	void Update();

	bool GetKey(SDL_Scancode scancode);

private:
	const Uint8* m_keyboard_state = nullptr;
};
