#include "sandpch.h"
#include "Input.h"

#include "Sand/Core/Application.h"
#include <GLFW/glfw3.h>
#include <../imgui/imgui.h>

namespace Sand 
{
	Input::InputState Input::state;

	bool Input::IsKeyPressed(Keycode key)
	{
		return state.KeysDown[static_cast<uint32_t>(key)];
	}

	bool Input::WasKeyPressed(Keycode key)
	{
		return state.KeysDownThisFrame[static_cast<uint32_t>(key)];
	}

	bool Input::IsKeyPressed(Mousecode button)
	{
		return state.MouseStates[static_cast<uint32_t>(button)];
	}

	bool Input::WasKeyPressed(Mousecode button)
	{
		return state.MouseThisFrame[static_cast<uint32_t>(button)];
	}

	bool Input::WasKeyReleased(Keycode key)
	{
		return state.KeysReleasedThisFrame[static_cast<uint32_t>(key)];
	}

	bool Input::WasKeyReleased(Mousecode key)
	{
		return state.MouseReleasedThisFrame[static_cast<uint32_t>(key)];
	}

	glm::vec2 Input::GetMousePosition()
	{
		double x, y;
		glfwGetCursorPos(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()), &x, &y);
		return glm::vec2(x, y);
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}

	void Input::OnKeyPressed(Keycode key)
	{
		if (static_cast<uint32_t>(key) > state.KeysDown.size()) {
			SAND_CORE_WARN("Attempted to respond to input for key {0}", static_cast<uint32_t>(key));
			return;
		}
		state.KeysReleasedThisFrame[static_cast<uint32_t>(key)] = 0;
		state.KeysDownThisFrame[static_cast<uint32_t>(key)] = 1;
		state.KeysDown[static_cast<uint32_t>(key)] = 1;
	}

	void Input::OnKeyReleased(Keycode key)
	{
		if (static_cast<uint32_t>(key) > state.KeysDown.size()) {
			SAND_CORE_WARN("Attempted to respond to input for key {0}", static_cast<uint32_t>(key));
			return;
		}
		state.KeysReleasedThisFrame[static_cast<uint32_t>(key)] = 1;
		state.KeysDownThisFrame[static_cast<uint32_t>(key)] = 0;
		state.KeysDown[static_cast<uint32_t>(key)] = 0;
	}

	void Input::OnMouseButtonPressed(Mousecode button)
	{
		if (static_cast<uint32_t>(button) > state.MouseStates.size()) {
			SAND_CORE_WARN("Attempted to respond to input for mouse button {0}", static_cast<uint32_t>(button));
			return;
		}
		state.MouseReleasedThisFrame[static_cast<uint32_t>(button)] = 0;
		state.MouseThisFrame[static_cast<uint32_t>(button)] = 1;
		state.MouseStates[static_cast<uint32_t>(button)] = 1;
	}

	void Input::OnMouseButtonReleased(Mousecode button)
	{
		if (static_cast<uint32_t>(button) > state.MouseStates.size()) {
			SAND_CORE_WARN("Attempted to respond to input for mouse button {0}", static_cast<uint32_t>(button));
			return;
		}
		state.MouseReleasedThisFrame[static_cast<uint32_t>(button)] = 1;
		state.MouseThisFrame[static_cast<uint32_t>(button)] = 0;
		state.MouseStates[static_cast<uint32_t>(button)] = 0;
	}

}