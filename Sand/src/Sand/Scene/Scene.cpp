#include "sandpch.h"
#include "Scene.h"

#include "Sand/Renderer/Renderer2D.h"
#include "Sand/Core/Application.h"

#include "Components.h"

#include <glad\glad.h>

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

	Entity Scene::DuplicateEntity(Entity original)
	{
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

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// scritps
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				});

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

		if (primaryCamera)
		{
			Renderer2D::BeginScene(*primaryCamera, camTransform);

			auto group = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
			for (auto entity : group)
			{
				auto [sprite, transform] = group.get<SpriteRendererComponent, TransformComponent>(entity);
				
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (uint32_t)entity);
			}

			Renderer2D::EndScene();
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

	int Scene::Pixel(int x, int y)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT1);

		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
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
	}
	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}
	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}

}