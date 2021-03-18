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

			T& component = mScene->m_CurrentRegistry->emplace<T>(mEntityHandle, std::forward<Args>(args)...);
			mScene->OnComponentAdded<T>(*this, component);

			return component;
		}
		template<typename T>
		T& GetComponent()
		{
			SAND_CORE_ASSERT(HasComponent<T>(), "Actor does not have component.");
			return mScene->m_CurrentRegistry->get<T>(mEntityHandle);
		}
		template<typename T>
		bool HasComponent()
		{
			return mScene->m_CurrentRegistry->has<T>(mEntityHandle);
		}
		template<typename T>
		void RemoveComponent()
		{
			SAND_CORE_ASSERT(HasComponent<T>(), "Actor does not have component.");

			// I know this is horrid but it kinda just works
			if constexpr (std::is_base_of_v<T, ScriptComponent>)
				mScene->m_CurrentRegistry->get<T>(mEntityHandle).Deactivate();

			mScene->m_CurrentRegistry->remove<T>(mEntityHandle);
		}

		operator bool() const { return mEntityHandle != entt::null; }
		operator entt::entity() const { return mEntityHandle; }
		operator uint32_t() const { return (uint32_t)mEntityHandle; }

		bool operator==(const Actor& other) const { return mEntityHandle == other.mEntityHandle && mScene == other.mScene; }
		bool operator!=(const Actor& other) const { return !(*this == other); }

		Scene* GetScene() const { return mScene; }
	private:
		entt::entity mEntityHandle = entt::null;
		Scene* mScene = nullptr;

		friend class Scene;
	};

}