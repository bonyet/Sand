#pragma once

#include "Actor.h"

#include "Sand/Events/MouseEvent.h"
#include "Sand/Events/KeyEvent.h"

namespace Sand
{

	class ScriptableActor
	{
	public:
		virtual ~ScriptableActor() {}

		template<typename T>
		T& GetComponent()
		{
			return m_Actor.GetComponent<T>();
		}
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_Actor.AddComponent<T>(args);
		}
		template<typename T>
		bool HasComponent()
		{
			return m_Actor.HasComponent<T>();
		}
		template<typename T>
		void RemoveComponent()
		{
			m_Actor.RemoveComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
	private:
		Actor m_Actor;

		friend class Scene;
	};

}