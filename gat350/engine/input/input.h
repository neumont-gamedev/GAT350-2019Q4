#pragma once

#include "../framework/system.h"

class Input : public System
{
public:
	OBJECT_DECLARATION(Input, System)
	~Input() {}

	bool Initialize();
	void Shutdown();
	void Update();

	bool GetKey(SDL_Scancode scancode);

private:
	const Uint8* m_keyboard_state = nullptr;
};
