#pragma once

#include "Sand/Events/Event.h"
#include "Sand/Core/Input.h"

namespace Sand 
{

	class KeyEvent : public Event
	{
	public:
		Keycode GetKey() const { return static_cast<Sand::Keycode>(Keycode); }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(uint32_t Keycode)
			: Keycode(Keycode) {}

		uint32_t Keycode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(uint32_t Keycode, int repeatCount)
			: KeyEvent(Keycode), m_RepeatCount(repeatCount) {}

		int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << Keycode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(uint32_t Keycode)
			: KeyEvent(Keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << Keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(uint32_t Keycode)
			: KeyEvent(Keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << Keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}