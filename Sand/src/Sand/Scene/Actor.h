#pragma once

#include "Scene.h"

namespace Sand
{

	class Actor
	{
	public:
		Actor() = default;
		Actor(entt::entity handle, Scene* scene);
		Actor(const Actor& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			SAND_CORE_ASSERT(!HasComponent<T>(), "Actor already has component.");

			T& component = m_Scene->m_CurrentRegistry->emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);

			return component;
		}
		template<typename T>
		T& GetComponent()
		{
#ifdef _DEBUG
			const char* name = typeid(T).name();
#endif
			SAND_CORE_ASSERT(HasComponent<T>(), "Actor does not have component.");
			return m_Scene->m_CurrentRegistry->get<T>(m_EntityHandle);
		}
		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_CurrentRegistry->has<T>(m_EntityHandle);
		}
		template<typename T>
		void RemoveComponent()
		{
			SAND_CORE_ASSERT(HasComponent<T>(), "Actor does not have component.");
			
			m_Scene->OnComponentRemoved<T>(*this, m_Scene->m_CurrentRegistry->get<T>(m_EntityHandle));
			m_Scene->m_CurrentRegistry->remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		bool operator==(const Actor& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
		bool operator!=(const Actor& other) const { return !(*this == other); }

		Scene* GetScene() const { return m_Scene; }
	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;

		friend class Scene;
	};

}