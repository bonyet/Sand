#include "sandpch.h"
#include "Scene.h"

#include "Sand/Renderer/Renderer2D.h"
#include "Sand/Core/Application.h"
#include "SceneRenderer.h"
#include "Components.h"

#include <glad\glad.h>
#include "Physics/PhysicsWorld.h"

namespace Sand
{

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		SAND_PROFILE_FUNCTION();

		Entity entity = { m_Registry.create(), this };
		auto& transform = entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Name = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::DestroyEntity(Entity& entity)
	{
		SAND_PROFILE_FUNCTION();

		m_Registry.destroy(entity);
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		// Render geometry
		{
			SceneRenderer::Begin(camera);

			auto group = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
			for (auto entity : group)
			{
				auto [sprite, transform] = group.get<SpriteRendererComponent, TransformComponent>(entity);

				SceneRenderer::Submit(transform.GetTransform(), sprite.Color, (uint32_t)entity);
			}

			SceneRenderer::End();
		}
	}

	void Scene::BeginPlay()
	{
		m_Playmode = true;

		m_Registry.view<Rigidbody2DComponent>().each([=](auto entity, auto& body)
		{
			Entity bodyEntity = Entity{ entity, this };
			auto& transform = bodyEntity.GetComponent<TransformComponent>();
			body.Create();
		});

		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
		{
			nsc.Instance = nsc.InstantiateScript();
			nsc.Instance->m_Entity = Entity{ entity, this };
			nsc.Instance->OnCreate();
		});
	}

	void Scene::EndPlay()
	{
		m_Playmode = false;

		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
		{
			nsc.Instance->OnDestroy();
			nsc.DestroyScript(&nsc);
		});
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// scritps
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				nsc.Instance->OnUpdate(ts);
			});
		}

		Camera* primaryCamera = nullptr;
		glm::mat4 camTransform;

		auto group = m_Registry.group<TransformComponent, CameraComponent>();
		for (auto entity : group)
		{
			auto [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);				
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
			m_Registry.view<Rigidbody2DComponent>().each([=](auto entity, auto& body)
			{
				body.UpdateTransform();
			});
		}

		if (primaryCamera)
		{
			SceneRenderer::Begin(*primaryCamera, camTransform);

			auto group = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
			for (auto entity : group)
			{
				auto [sprite, transform] = group.get<SpriteRendererComponent, TransformComponent>(entity);
				
				SceneRenderer::Submit(transform.GetTransform(), sprite.Color, (uint32_t)entity);
			}

			SceneRenderer::End();
		}
	}

	void Scene::DrawIDBuffer(Ref<Framebuffer> target, EditorCamera& camera)
	{
		target->Bind();
		// Render to ID buffer
		{
			Renderer2D::BeginScene(camera);

			auto group = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
			for (auto entity : group)
			{
				auto [sprite, transform] = group.get<SpriteRendererComponent, TransformComponent>(entity);

				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (uint32_t)entity);
			}

			Renderer2D::EndScene();
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	Entity Scene::FindEntity(const std::string& name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			auto& tag = view.get(entity);

			if (tag.Name == name)
				return Entity{ entity, this };
		}

		SAND_CORE_ERROR("Entity '{0}' not present in scene", name);

		return {};
	}

	void Scene::OnViewportResize(float x, float y)
	{
		m_ViewportWidth = x;
		m_ViewportHeight = y;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(x, y);
		}
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
		component.owner = entity;
	}
	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
		component.owner = entity;
	}
	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
		component.owner = entity;
	}
	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.owner = entity;
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}
	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{
		component.owner = entity;
	}
	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
		component.owner = entity;
	}

}