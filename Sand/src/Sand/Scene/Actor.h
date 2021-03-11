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

			T& component = mScene->mRegistry.emplace<T>(mEntityHandle, std::forward<Args>(args)...);
			mScene->OnComponentAdded<T>(*this, component);

			return component;
		}
		template<typename T>
		T& GetComponent()
		{
			SAND_CORE_ASSERT(HasComponent<T>(), "Actor does not have component.");
			return mScene->mRegistry.get<T>(mEntityHandle);
		}
		template<typename T>
		bool HasComponent()
		{
			return mScene->mRegistry.has<T>(mEntityHandle);
		}
		template<typename T>
		void RemoveComponent()
		{
			SAND_CORE_ASSERT(HasComponent<T>(), "Actor does not have component.");
			mScene->mRegistry.remove<T>(mEntityHandle);
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