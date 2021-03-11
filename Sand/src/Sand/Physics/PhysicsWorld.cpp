#include "sandpch.h"
#include "PhysicsWorld.h"

#include "Sand/Scene/Scene.h"
#include "Sand/Scene/Components.h"

#include <imgui.h>

namespace Sand
{
	void PhysicsWorld::ShowDebugWindow(Scene* const scene)
	{
		ImGui::Begin("Physics Debug");

		auto view = scene->mRegistry.view<PhysicsComponent>();
		for (auto entity : view)
		{
			Actor actor = { entity, scene };
			auto& physics = view.get<PhysicsComponent>(entity);

			ImGui::Text("Entity '%s' :: %d", actor.GetComponent<TagComponent>().Name.c_str(), (uint32_t)actor);
		}

		ImGui::End();
	}

	void PhysicsWorld::InitializeBodies(Scene* const scene)
	{
		// Create all physics bodies
		auto view = scene->mRegistry.view<PhysicsComponent>();
		for (auto entity : view)
		{
			auto& physics = view.get<PhysicsComponent>(entity);
			physics.owner = { entity, scene };

			SAND_CORE_INFO("Init {0}", physics.owner.GetComponent<TagComponent>().Name);

			physics.Init();
		}
	}

	void PhysicsWorld::Step(class Scene* const scene, Timestep timestep)
	{
		// Update all physics bodies
		auto view = scene->mRegistry.view<PhysicsComponent>();
		for (auto entity : view)
		{
			view.get<PhysicsComponent>(entity).SimulateStep(this, timestep);
		}
	}

}