#include "sandpch.h"
#include "Scene.h"

#include "Sand/Renderer/Renderer2D.h"
#include "Sand/Core/Application.h"
#include "Sand/Renderer/Renderer2D.h"
#include "Components.h"
#include "Sand/Scripting/ScriptComponent.h"

namespace Sand
{

	Scene::~Scene()
	{
	}

	Actor Scene::CreateActor(const std::string& name)
	{
		SAND_PROFILE_FUNCTION();

		Actor actor = { m_Registry.create(), this };
		auto& transform = actor.AddComponent<TransformComponent>();
		auto& tag = actor.AddComponent<TagComponent>();
		tag.Name = name.empty() ? "Actor" : name;

		return actor;
	}

	void Scene::DestroyActor(Actor& actor)
	{
		SAND_PROFILE_FUNCTION();

		m_Registry.destroy(actor);
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		// Render geometry

		{
			Renderer2D::Begin(camera);

			auto group = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
			for (auto actor : group)
			{
				auto [sprite, transform] = group.get<SpriteRendererComponent, TransformComponent>(actor);

				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (uint32_t)actor);
			}

			Renderer2D::End();
		}
	}

	void Scene::BeginPlay()
	{
		m_Playmode = true;

		m_Registry.view<ScriptComponent>().each([=](auto actor, auto& sc)
		{
			sc.OnCreate();
		});
	}

	void Scene::EndPlay()
	{
		m_Playmode = false;

		m_Registry.view<ScriptComponent>().each([=](auto actor, auto& sc)
		{
			sc.OnDestroy();
		});
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// scritps
		{
			m_Registry.view<ScriptComponent>().each([=](auto actor, auto& sc)
			{
				sc.OnUpdate((float)ts);
			});
		}

		Camera* primaryCamera = nullptr;
		glm::mat4 camTransform;

		auto group = m_Registry.group<TransformComponent, CameraComponent>();
		for (auto actor : group)
		{
			auto [transform, camera] = group.get<TransformComponent, CameraComponent>(actor);				
			if (camera.Primary)
			{
				primaryCamera = &camera.Camera;
				camTransform = transform;
				break;
			}
		}

		{
			PhysicsWorld::Step();
			// rigidbodies
			m_Registry.view<Rigidbody2DComponent>().each([=](auto actor, auto& body)
			{
				body.UpdateTransform();
			});
		}

		if (primaryCamera)
		{
			Renderer2D::Begin(*primaryCamera, camTransform);

			auto group = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
			for (auto actor : group)
			{
				auto [sprite, transform] = group.get<SpriteRendererComponent, TransformComponent>(actor);
				
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (uint32_t)actor);
			}

			Renderer2D::End();
		}
	}

	Actor Scene::GetPrimaryCameraActor()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto actor : view) {
			const auto& camera = view.get<CameraComponent>(actor);
			if (camera.Primary)
				return Actor{ actor, this };
		}
		return {};
	}

	Actor Scene::FindActor(const std::string& name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto actor : view)
		{
			auto& tag = view.get(actor);

			if (tag.Name == name)
				return Actor{ actor, this };
		}

		SAND_CORE_ERROR("Actor '{0}' not present in scene", name);

		return {};
	}

	void Scene::OnViewportResize(float x, float y)
	{
		m_ViewportWidth = x;
		m_ViewportHeight = y;

		auto view = m_Registry.view<CameraComponent>();
		for (auto actor : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(actor);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(x, y);
		}
	}

	template<typename T>
	void Scene::OnComponentAdded(Actor actor, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Actor actor, TagComponent& component)
	{
		component.owner = actor;
	}
	template<>
	void Scene::OnComponentAdded<TransformComponent>(Actor actor, TransformComponent& component)
	{
		component.owner = actor;
	}
	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Actor actor, SpriteRendererComponent& component)
	{
		component.owner = actor;
	}
	template<>
	void Scene::OnComponentAdded<CameraComponent>(Actor actor, CameraComponent& component)
	{
		component.owner = actor;
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}
	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Actor actor, BoxCollider2DComponent& component)
	{
		component.owner = actor;
	}
	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Actor actor, Rigidbody2DComponent& component)
	{
		component.owner = actor;
	}
	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Actor actor, NativeScriptComponent& component)
	{
		component.owner = actor;
	}
	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Actor actor, ScriptComponent& component)
	{
		component.owner = actor;
	}

}