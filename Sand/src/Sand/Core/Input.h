#pragma once

#include "Sand/Core/Keycodes.h"
#include "Sand/Core/Mousecodes.h"

#include <bitset>
#include "glm/glm.hpp"

namespace Sand 
{

	class Input
	{
	private:
		class InputState
		{
		public:
			void Clear()
			{
				KeysDownThisFrame.reset();
				MouseThisFrame.reset();

				KeysReleasedThisFrame.reset();
				MouseReleasedThisFrame.reset();
			}
		public:
			std::bitset<349u> KeysDown, KeysDownThisFrame, KeysReleasedThisFrame;
			std::bitset<4u> MouseStates, MouseThisFrame, MouseReleasedThisFrame;
		};
	public:
		static bool IsKeyPressed(Keycode key);
		static bool WasKeyPressed(Keycode key);

		static bool IsKeyPressed(Mousecode button);
		static bool WasKeyPressed(Mousecode button);

		static bool WasKeyReleased(Keycode key);
		static bool WasKeyReleased(Mousecode key);

		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	private:
		static void OnKeyPressed(Keycode key);
		static void OnKeyReleased(Keycode key);
		static void OnMouseButtonPressed(Mousecode button);
		static void OnMouseButtonReleased(Mousecode button);

		static void Update()
		{
			state.Clear();
		}
	private:
		static InputState state;

		friend class WindowsWindow;
		friend class EditorLayer;
		friend class Application;
	};

}
