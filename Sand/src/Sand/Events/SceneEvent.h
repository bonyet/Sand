#pragma once

#include "Sand/Events/Event.h"
#include "Sand/Scene/Scene.h"

namespace Sand
{

	class SceneEndPlayEvent : public Event
	{
	public:
		SceneEndPlayEvent(Scene* const scene)
			: m_Scene(scene)
		{
		}

		Scene* const GetScene() const { return m_Scene; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "SceneEndPlayEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(SceneEndPlay)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		Scene* m_Scene = nullptr;
	};

}