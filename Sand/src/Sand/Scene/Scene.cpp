#include "sandpch.h"
#include "Sand/Events/SceneEvent.h" // also includes Scene.h

#include "Sand/Renderer/Renderer2D.h"
#include "Sand/Core/Application.h"
#include "Sand/Renderer/Renderer2D.h"
#include "Sand/Renderer/RenderCommand.h"

#include "Components.h"

namespace Sand
{

	static Scene* s_ActiveScene = nullptr;

	Scene::Scene()
	{
		m_CurrentRegistry = &m_Registry;

		s_ActiveScene = this;
	}

	Scene::~Scene()
	{
	}

	Actor Scene::CreateActor(const std::string& name)
	{
		SAND_PROFILE_FUNCTION();

		Actor actor = { m_CurrentRegistry->create(), this };
		auto& tag = actor.AddComponent<TagComponent>();
		tag.Name = name.empty() ? "Actor" : name;

		actor.AddComponent<TransformComponent>();

		OnActorCreate(actor);

		return actor;
	}

	void Scene::DestroyActor(Actor& actor)
	{
		SAND_PROFILE_FUNCTION();

		OnActorDestroy(actor);
		m_CurrentRegistry->destroy(actor);
	}

	void Scene::BeginPlay()
	{
		SAND_PROFILE_FUNCTION();
		
		m_Playmode = true;

		SwitchToRuntimeRegistry();

		m_PhysicsWorld.Create({ 0.0f, -10.0f });
		Audio::Unlock();

		// Create all physics bodies
		{
			auto bodies = m_CurrentRegistry->view<PhysicsComponent>();
			for (auto& entity : bodies)
			{
				auto& component = bodies.get<PhysicsComponent>(entity);

				const auto& tc = m_CurrentRegistry->get<TransformComponent>(entity);

				PhysicsBodyDescription description = {
					tc.GetPosition(), tc.GetScale(), tc.GetRotation()
				};

				component.Body.Create(description, &m_PhysicsWorld);

				// Create the collider too, if it has one
				if (m_CurrentRegistry->has<BoxColliderComponent>(entity))
				{
					m_CurrentRegistry->get<BoxColliderComponent>(entity).Create(tc.GetScale(), component.Body.GetBody());
				}
			}
		}
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
		Audio::Lock();

		SwitchToEditorRegistry();

		Application::Get().OnEvent(SceneEndPlayEvent{ this });
	}


	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		SAND_PROFILE_FUNCTION();

		// Render geometry
		{
			Renderer2D::Begin(camera);

			// Rendering
			{
				auto group = m_CurrentRegistry->group<SpriteRendererComponent>(entt::get<TransformComponent>);
				for (auto actor : group)
				{
					auto [sprite, transform] = group.get<SpriteRendererComponent, TransformComponent>(actor);

					// Render the sprite
					bool hasTextureComponent = m_CurrentRegistry->has<TextureComponent>(actor);
					if (!hasTextureComponent)
					{
						Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (uint32_t)actor);
					}
					else
					{
						// Render textured sprite
						auto& texComponent = m_CurrentRegistry->get<TextureComponent>(actor);

						if (texComponent.IsTextured()) {
							Renderer2D::DrawQuad(transform.GetTransform(), (uint32_t)actor, texComponent.Texture, texComponent.TilingFactor, sprite.Color);
						}
						else {
							Renderer2D::DrawQuad(transform.GetTransform(), { 1.0f, 0.0f, 1.0f, 1.0f }, (uint32_t)actor);
						}
					}
				}
			}

			Renderer2D::End();
		}
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

				tc.SetPosition(component.Body.GetPosition());
				tc.SetRotation(component.Body.GetRotation());
			}
		}

		// Camera stuff and rendering
		Camera* primaryCamera = nullptr;
		glm::mat4 camTransform;

		{
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
		}

		if (primaryCamera)
		{
			Renderer2D::Begin(*primaryCamera, camTransform);

			// Rendering
			{
				auto group = m_CurrentRegistry->group<SpriteRendererComponent>(entt::get<TransformComponent>);
				for (auto actor : group)
				{
					auto [sprite, transform] = group.get<SpriteRendererComponent, TransformComponent>(actor);

					// Render the sprite
					bool hasTextureComponent = m_CurrentRegistry->has<TextureComponent>(actor);
					if (!hasTextureComponent)
					{
						Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, static_cast<uint32_t>(actor));
					}
					else 
					{
						// Render textured sprite
						auto& texComponent = m_CurrentRegistry->get<TextureComponent>(actor);

						if (texComponent.IsTextured()) {
							Renderer2D::DrawQuad(transform.GetTransform(), static_cast<uint32_t>(actor), texComponent.Texture, texComponent.TilingFactor, sprite.Color);
						}
						else {
							Renderer2D::DrawQuad(transform.GetTransform(), { 1.0f, 0.0f, 1.0f, 1.0f }, static_cast<uint32_t>(actor));
						}
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
		/* We do this instead of registry::each because a traditional for loop allows early exit via return, so 
		we don't have to continue iterating entities even if we found the one we want */
		auto data = m_CurrentRegistry->data();
		for (size_t i = 0; i < m_CurrentRegistry->size(); i++)
		{
			if (actor == data[i])
				return true;
		}
		return false;
	}

	bool Scene::IsActorIDValid(uint32_t id)
	{
		return m_Registry.valid(static_cast<entt::entity>(id));
	}

	Scene* const Scene::GetActiveScene()
	{
		return s_ActiveScene;
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
				return static_cast<int>(actor);
		}

		return -1; // Not found
	}

	Actor Scene::DuplicateActor(Actor original)
	{
		// I admit this is fairly adventurous use of the preprocessor 
#define COPY_COMPONENT(Type) if (m_CurrentRegistry->has<Type>(original)) { m_CurrentRegistry->emplace<Type>(clone, original.GetComponent<Type>()); }

		Actor clone = { m_CurrentRegistry->create(), this };

		COPY_COMPONENT(TagComponent);
		COPY_COMPONENT(TransformComponent);
		COPY_COMPONENT(SpriteRendererComponent);
		COPY_COMPONENT(CameraComponent);
		COPY_COMPONENT(PhysicsComponent);
		COPY_COMPONENT(BoxColliderComponent);
		COPY_COMPONENT(TextureComponent);
		COPY_COMPONENT(AnimatorComponent);
		COPY_COMPONENT(AudioSourceComponent);

		return clone;

#undef COPY_COMPONENT
	}

#define COPY_COMPONENT_TO_REGISTRY(Type) { auto view = m_Registry.view<Type>(); m_RuntimeRegistry.insert<Type>(view.data(), view.data() + view.size(), view.raw(), view.raw() + view.size()); }
	void Scene::SwitchToRuntimeRegistry()
	{
		m_RuntimeRegistry.clear();

		// Copy all entities
		m_RuntimeRegistry.assign(m_Registry.data(), m_Registry.data() + m_Registry.size(), m_Registry.destroyed());

		COPY_COMPONENT_TO_REGISTRY(TagComponent);
		COPY_COMPONENT_TO_REGISTRY(TransformComponent);
		COPY_COMPONENT_TO_REGISTRY(SpriteRendererComponent);
		COPY_COMPONENT_TO_REGISTRY(CameraComponent);
		COPY_COMPONENT_TO_REGISTRY(PhysicsComponent);
		COPY_COMPONENT_TO_REGISTRY(BoxColliderComponent);
		COPY_COMPONENT_TO_REGISTRY(TextureComponent);
		COPY_COMPONENT_TO_REGISTRY(AnimatorComponent);
		COPY_COMPONENT_TO_REGISTRY(AudioSourceComponent);

		m_CurrentRegistry = &m_RuntimeRegistry;
	}
#undef COPY_COMPONENT_TO_REGISTRY

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

	void Scene::OnActorCreate(Actor actor)
	{
	}

	void Scene::OnActorDestroy(Actor actor)
	{
		SAND_PROFILE_FUNCTION();

		// Properly remove children
		TransformComponent& transform = actor.GetComponent<TransformComponent>();

		if (transform.HasParent())
			transform.GetParentTransform().RemoveChild(actor);
		
		for (auto& child : transform.GetChildren())
			DestroyActor(child); // Calls OnActorDestroy too
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
	void Scene::OnComponentAdded<BoxColliderComponent>(Actor actor, BoxColliderComponent& component)
	{
		component.owner = actor;

		const auto& tc = actor.GetComponent<TransformComponent>();

		if (m_Playmode)
			component.Create(tc.GetScale(), actor.GetComponent<PhysicsComponent>().Body.GetBody());
	}
	template<>
	void Scene::OnComponentAdded<PhysicsComponent>(Actor actor, PhysicsComponent& component)
	{
		component.owner = actor;

		const auto& tc = actor.GetComponent<TransformComponent>();
		component.Description = {
			tc.GetPosition(), tc.GetScale(), tc.GetRotation()
		};

		// Initialize the physics body correctly if we are running the game
		if (m_Playmode)
		{
			component.Body.Create(component.Description, &m_PhysicsWorld);

			// Initialize the collider correctly if we are running the game
			if (actor.HasComponent<BoxColliderComponent>())
			{
				auto& collider = actor.GetComponent<BoxColliderComponent>();
				collider.Create(tc.GetScale(), component.Body.GetBody());
			}
		}
	}

	template<>
	void Scene::OnComponentAdded<AudioSourceComponent>(Actor actor, AudioSourceComponent& component)
	{
		component.owner = actor;
	}

	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Actor actor, ScriptComponent& component)
	{
		component.owner = actor;
		component.Init();
	}

	template<typename T>
	void Scene::OnComponentRemoved(Actor actor, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentRemoved<TagComponent>(Actor actor, TagComponent& component)
	{
	}
	template<>
	void Scene::OnComponentRemoved<TransformComponent>(Actor actor, TransformComponent& component)
	{
		// Properly remove children
		if (component.HasParent())
			component.GetParentTransform().RemoveChild(actor);
	}
	template<>
	void Scene::OnComponentRemoved<TextureComponent>(Actor actor, TextureComponent& component)
	{
	}
	template<>
	void Scene::OnComponentRemoved<SpriteRendererComponent>(Actor actor, SpriteRendererComponent& component)
	{
	}
	template<>
	void Scene::OnComponentRemoved<CameraComponent>(Actor actor, CameraComponent& component)
	{
	}
	template<>
	void Scene::OnComponentRemoved<NativeScriptComponent>(Actor actor, NativeScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentRemoved<BoxColliderComponent>(Actor actor, BoxColliderComponent& component)
	{
		component.Destroy();
	}

	template<>
	void Scene::OnComponentRemoved<PhysicsComponent>(Actor actor, PhysicsComponent& component)
	{
		if (actor.HasComponent<BoxColliderComponent>())
			actor.GetComponent<BoxColliderComponent>().Destroy();
	}

	template<>
	void Scene::OnComponentRemoved<AudioSourceComponent>(Actor actor, AudioSourceComponent& component)
	{
	}

	template<>
	void Scene::OnComponentRemoved<ScriptComponent>(Actor actor, ScriptComponent& component)
	{
		component.Uninit();
	}

}