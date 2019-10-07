#include "input_system.h"

bool InputSystem::Initialize()
{
	m_keyboard_state = SDL_GetKeyboardState(NULL);

	return true;
}

void InputSystem::Shutdown()
{
}

void InputSystem::Update()
{
}

bool InputSystem::GetKey(SDL_Scancode scancode)
{
	return m_keyboard_state[scancode];
}
