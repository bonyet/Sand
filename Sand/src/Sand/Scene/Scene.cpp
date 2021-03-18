#include "sandpch.h"
#include "Sand/Events/SceneEvent.h" // also includes Scene.h

#include "Sand/Renderer/Renderer2D.h"
#include "Sand/Core/Application.h"
#include "Sand/Renderer/Renderer2D.h"

#include "../Scripting/ScriptEngine.h"

#include "Components.h"
#include "Sand\Renderer\RenderCommand.h"

namespace Sand
{
	Scene::Scene()
	{
		m_CurrentRegistry = &m_Registry;

		ScriptEngine::SetActiveScene(this);
	}

	Scene::~Scene()
	{
	}

	Actor Scene::CreateActor(const std::string& name)
	{
		SAND_PROFILE_FUNCTION();

		Actor actor = { m_CurrentRegistry->create(), this };
		auto& transform = actor.AddComponent<TransformComponent>();
		auto& tag = actor.AddComponent<TagComponent>();
		tag.Name = name.empty() ? "Actor" : name;

		return actor;
	}

	void Scene::DestroyActor(Actor& actor)
	{
		SAND_PROFILE_FUNCTION();

		m_CurrentRegistry->destroy(actor);
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		SAND_PROFILE_FUNCTION();

		// Render geometry

		{
			Renderer2D::Begin(camera);

			auto group = m_CurrentRegistry->group<SpriteRendererComponent>(entt::get<TransformComponent>);
			for (auto actor : group)
			{
				auto [sprite, transform] = group.get<SpriteRendererComponent, TransformComponent>(actor);

				// Render the sprite
				{
					bool hasTextureComponent = m_CurrentRegistry->has<TextureComponent>(actor);
					if (hasTextureComponent)
					{
						auto& texComponent = m_CurrentRegistry->get<TextureComponent>(actor);
						if (texComponent.IsTextured()) {
							Renderer2D::DrawQuad(transform.GetTransform(), (uint32_t)actor, texComponent.Texture, texComponent.TilingFactor, sprite.Color);
						}
						else
						{
							// TODO: make good
							constexpr glm::vec4 MISSING_TEXTURE_COLOR = { 1.0f, 0.0f, 1.0f, 1.0f };
							Renderer2D::DrawQuad(transform.GetTransform(), MISSING_TEXTURE_COLOR, (uint32_t)actor);
						}
					}
					else if (!hasTextureComponent) 
					{
						Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (uint32_t)actor);
					}
				}
			}

			Renderer2D::End();
		}
	}

	void Scene::BeginPlay()
	{
		SAND_PROFILE_FUNCTION();
		
		m_Playmode = true;

		SwitchToRuntimeRegistry();

		m_PhysicsWorld.Create({ 0.0f, -10.0f });

		// Create all physics bodies
		{
			auto view = m_CurrentRegistry->view<PhysicsComponent>();
			for (auto& entity : view)
			{
				auto& component = view.get<PhysicsComponent>(entity);

				const auto& tc = m_CurrentRegistry->get<TransformComponent>(entity);

				PhysicsBodyDescription description = {};
				description.Position = tc.Position;
				description.Scale = tc.Scale;
				description.Rotation = tc.Rotation;

				component.Body.Create(description, &m_PhysicsWorld);
			}
		}

		ScriptEngine::CreateAll();
	}

	void Scene::EndPlay()
	{
		SAND_PROFILE_FUNCTION();

		m_Playmode = false;


		// Destroy all physics bodies
		{
			auto view = m_CurrentRegistry->view<PhysicsComponent>();
			for (auto& entity : view)
			{
				auto& component = view.get<PhysicsComponent>(entity);
				component.Body.Destroy();
			}
		}

		m_PhysicsWorld.Destroy();

		ScriptEngine::DestroyAll();

		SwitchToEditorRegistry();

		Application::Get().OnEvent(SceneEndPlayEvent{ this });
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		SAND_PROFILE_FUNCTION();

		// Update physics
		m_PhysicsWorld.Step();

		// Update all entities' physics stuff
		{
			SAND_PROFILE_SCOPE("Update PhysicsComponents - Scene::OnUpdateRuntime()");

			auto view = m_CurrentRegistry->view<PhysicsComponent>();
			for (auto& entity : view)
			{
				auto& component = view.get<PhysicsComponent>(entity);
				auto& tc = m_CurrentRegistry->get<TransformComponent>(entity);

				tc.Position = component.Body.GetPosition();
				tc.Rotation = component.Body.GetRotation();
			}
		}

		// Update scripts
		ScriptEngine::UpdateAll(ts);

		// Update animations
		auto view = m_CurrentRegistry->view<AnimatorComponent>();
		for (auto& entity : view)
		{
			SAND_PROFILE_SCOPE("Update AnimatorComponents - Scene::OnUpdateRuntime()");

			auto& animator = view.get<AnimatorComponent>(entity);

			animator.Animator.OnUpdate({ entity, this }, ts);
		}

		// Camera stuff and rendering
		Camera* primaryCamera = nullptr;
		glm::mat4 camTransform;

		auto group = m_CurrentRegistry->group<TransformComponent, CameraComponent>();
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

		if (primaryCamera)
		{
			Renderer2D::Begin(*primaryCamera, camTransform);

			auto group = m_CurrentRegistry->group<SpriteRendererComponent>(entt::get<TransformComponent>);
			for (auto actor : group)
			{
				auto [sprite, transform] = group.get<SpriteRendererComponent, TransformComponent>(actor);
				
				// Render the sprite
				{
					bool hasTextureComponent = m_CurrentRegistry->has<TextureComponent>(actor);
					if (hasTextureComponent)
					{
						auto& texComponent = m_CurrentRegistry->get<TextureComponent>(actor);
						if (texComponent.IsTextured()) {
							Renderer2D::DrawQuad(transform.GetTransform(), (uint32_t)actor, texComponent.Texture, texComponent.TilingFactor, sprite.Color);
						}
						else
						{
							// TODO: make good
							constexpr glm::vec4 MISSING_TEXTURE_COLOR = { 1.0f, 0.0f, 1.0f, 1.0f };
							Renderer2D::DrawQuad(transform.GetTransform(), MISSING_TEXTURE_COLOR, (uint32_t)actor);
						}
					}
					else if (!hasTextureComponent) {
						Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (uint32_t)actor);
					}
				}
			}

			Renderer2D::End();
		}
	}

	Actor Scene::GetPrimaryCameraActor()
	{
		SAND_PROFILE_FUNCTION();

		auto view = m_CurrentRegistry->view<CameraComponent>();
		for (auto actor : view) {
			const auto& camera = view.get<CameraComponent>(actor);
			if (camera.Primary)
				return Actor{ actor, this };
		}
		return {};
	}

	bool Scene::ContainsActor(Actor actor)
	{
		bool contains = false;

		m_CurrentRegistry->each([&](auto entity)
		{
			if (actor.mEntityHandle == entity)
				contains = true;
		});

		return contains;
	}

	Actor Scene::FindActor(const std::string& name)
	{
		SAND_PROFILE_FUNCTION();

		auto view = m_CurrentRegistry->view<TagComponent>();
		for (auto actor : view)
		{
			auto& tag = view.get<TagComponent>(actor);

			if (tag.Name == name)
				return Actor{ actor, this };
		}

		return {};
	}
	int Scene::FindActorID(const std::string& name)
	{
		SAND_PROFILE_FUNCTION();

		auto view = m_CurrentRegistry->view<TagComponent>();
		for (auto actor : view)
		{
			auto& tag = view.get<TagComponent>(actor);

			if (tag.Name == name)
				return (int)actor;
		}

		return -1; // Not found
	}

	Actor Scene::DuplicateActor(Actor original)
	{
		Actor clone = { m_CurrentRegistry->create(), this };

		return clone;
	}

	void Scene::SwitchToRuntimeRegistry()
	{
		m_RuntimeRegistry.clear();

		// coppy all entities
		m_RuntimeRegistry.assign(m_Registry.data(), m_Registry.data() + m_Registry.size(), m_Registry.destroyed());

		// copy all components
		{
			auto view = m_Registry.view<TagComponent>();
			m_RuntimeRegistry.insert<TagComponent>(view.data(), view.data() + view.size(), view.raw(), view.raw() + view.size());
		}
		{
			auto view = m_Registry.view<TransformComponent>();
			m_RuntimeRegistry.insert<TransformComponent>(view.data(), view.data() + view.size(), view.raw(), view.raw() + view.size());
		}
		{
			auto view = m_Registry.view<SpriteRendererComponent>();
			m_RuntimeRegistry.insert<SpriteRendererComponent>(view.data(), view.data() + view.size(), view.raw(), view.raw() + view.size());
		}
		{
			auto view = m_Registry.view<CameraComponent>();
			m_RuntimeRegistry.insert<CameraComponent>(view.data(), view.data() + view.size(), view.raw(), view.raw() + view.size());
		}
		{
			auto view = m_Registry.view<PhysicsComponent>();
			m_RuntimeRegistry.insert<PhysicsComponent>(view.data(), view.data() + view.size(), view.raw(), view.raw() + view.size());
		}
		{
			auto view = m_Registry.view<ScriptComponent>();
			m_RuntimeRegistry.insert<ScriptComponent>(view.data(), view.data() + view.size(), view.raw(), view.raw() + view.size());
		}
		{
			auto view = m_Registry.view<TextureComponent>();
			m_RuntimeRegistry.insert<TextureComponent>(view.data(), view.data() + view.size(), view.raw(), view.raw() + view.size());
		}
		{
			auto view = m_Registry.view<AnimatorComponent>();
			m_RuntimeRegistry.insert<AnimatorComponent>(view.data(), view.data() + view.size(), view.raw(), view.raw() + view.size());
		}

		m_CurrentRegistry = &m_RuntimeRegistry;
	}

	void Scene::SwitchToEditorRegistry()
	{
		m_RuntimeRegistry.clear();

		m_CurrentRegistry = &m_Registry;
	}

	void Scene::OnViewportResize(float x, float y)
	{
		m_ViewportWidth = x;
		m_ViewportHeight = y;

		auto view = m_CurrentRegistry->view<CameraComponent>();
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
	void Scene::OnComponentAdded<TextureComponent>(Actor actor, TextureComponent& component)
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
	void Scene::OnComponentAdded<NativeScriptComponent>(Actor actor, NativeScriptComponent& component)
	{
		component.owner = actor;
	}
	template<>
	void Scene::OnComponentAdded<PhysicsComponent>(Actor actor, PhysicsComponent& component)
	{
		component.owner = actor;
	}
	template<>
	void Scene::OnComponentAdded<AnimatorComponent>(Actor actor, AnimatorComponent& component)
	{
		component.owner = actor;

		auto& animation = component.Animator.GetAnimation();

		animation.Append(glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::radians(5.0f),  glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		animation.Append(glm::vec2(2.0f, 2.0f), glm::vec2(1.0f, 1.0f), glm::radians(10.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		animation.Append(glm::vec2(3.0f, 3.0f), glm::vec2(1.0f, 1.0f), glm::radians(15.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		animation.Append(glm::vec2(4.0f, 4.0f), glm::vec2(1.0f, 1.0f), glm::radians(20.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Actor actor, ScriptComponent& component)
	{
		component.owner = actor;
	}

}