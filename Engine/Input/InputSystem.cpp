#include "InputSystem.h"

void nc::InputSystem::Startup()
{
	// get pointer to sdl keyboard states
	const Uint8* keyboardStateSDL = SDL_GetKeyboardState(&numKeys);
	// resize of keyboard state using numKeys for size
	keyboardState.resize(numKeys);
	// copy sdl keyboard state to keyboard state vector
	std::copy(keyboardStateSDL, keyboardStateSDL + numKeys, keyboardState.begin());
	// set previous keyboard state to keyboard state
	prevKeyboardState = keyboardState;
}

void nc::InputSystem::Shutdown()
{
}

void nc::InputSystem::Update(float dt)
{

	prevKeyboardState = keyboardState;
	const Uint8* keyboardStateSDL = SDL_GetKeyboardState(nullptr); 
	std::copy(keyboardStateSDL, keyboardStateSDL + numKeys, keyboardState.begin());

	prevMouseButtonState = mouseButtonState;
	int x, y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);
	mousePosition = nc::Vector2{ x,y };
	mouseButtonState[0] = buttons & SDL_BUTTON_LMASK;
	mouseButtonState[1] = buttons & SDL_BUTTON_MMASK;
	mouseButtonState[2] = buttons & SDL_BUTTON_RMASK;

}

nc::InputSystem::eKeyState nc::InputSystem::GetKeyState(int id)
{
	eKeyState state = eKeyState::IDLE; 
	bool keyDown = IsKeyDown(id);
	bool prevKeyDown = IsPreviousKeyDown(id);
	if (keyDown) 
	{ state = (prevKeyDown) ? eKeyState::HELD : eKeyState::PRESSED; }
	else 
	{ state = (prevKeyDown) ? eKeyState::RELEASE : eKeyState::IDLE; }
	return state;
}

nc::InputSystem::eKeyState nc::InputSystem::GetButtonState(int id)
{
	eKeyState state = eKeyState::IDLE;
	bool keyDown = IsButtonDown(id);
	bool prevKeyDown = IsPreviousButtonDown(id);
	if (keyDown)
	{
		state = (prevKeyDown) ? eKeyState::HELD : eKeyState::PRESSED;
	}
	else
	{
		state = (prevKeyDown) ? eKeyState::RELEASE : eKeyState::IDLE;
	}
	return state;
}

//bool nc::InputSystem::IsKeyDown(int id) const
//{
//	return keyboardState[id];
//}
//
//bool nc::InputSystem::IsPreviousKeyDown(int id) const
//{
//	return prevKeyboardState[id];
//}
