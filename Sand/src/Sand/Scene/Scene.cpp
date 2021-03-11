#include "sandpch.h"
#include "Scene.h"

#include "Sand/Renderer/Renderer2D.h"
#include "Sand/Core/Application.h"
#include "Sand/Renderer/Renderer2D.h"

#include "../Scripting/ScriptEngine.h"

#include "Components.h"

namespace Sand
{
	Scene::Scene()
	{
		mPhysicsWorld.SetGravity({ 0.0f, -10.0f });
	}

	Scene::~Scene()
	{
	}

	Actor Scene::CreateActor(const std::string& name)
	{
		SAND_PROFILE_FUNCTION();

		Actor actor = { mRegistry.create(), this };
		auto& transform = actor.AddComponent<TransformComponent>();
		auto& tag = actor.AddComponent<TagComponent>();
		tag.Name = name.empty() ? "Actor" : name;

		return actor;
	}

	void Scene::DestroyActor(Actor& actor)
	{
		SAND_PROFILE_FUNCTION();

		mRegistry.destroy(actor);
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		// Render geometry

		{
			Renderer2D::Begin(camera);

			auto group = mRegistry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
			for (auto actor : group)
			{
				auto [sprite, transform] = group.get<SpriteRendererComponent, TransformComponent>(actor);

				// Render the sprite
				{
					bool hasTextureComponent = mRegistry.has<TextureComponent>(actor);
					if (hasTextureComponent)
					{
						auto& texComponent = mRegistry.get<TextureComponent>(actor);
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
		mPlaymode = true;

		mPhysicsWorld.InitializeBodies(this);
		ScriptEngine::CreateAll();
	}

	void Scene::EndPlay()
	{
		mPlaymode = false;

		ScriptEngine::DestroyAll();
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		mPhysicsWorld.Step(this, ts);

		// scritps
		{
			ScriptEngine::UpdateAll(ts);
		}

		Camera* primaryCamera = nullptr;
		glm::mat4 camTransform;

		auto group = mRegistry.group<TransformComponent, CameraComponent>();
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

			auto group = mRegistry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
			for (auto actor : group)
			{
				auto [sprite, transform] = group.get<SpriteRendererComponent, TransformComponent>(actor);
				
				// Render the sprite
				{
					bool hasTextureComponent = mRegistry.has<TextureComponent>(actor);
					if (hasTextureComponent)
					{
						auto& texComponent = mRegistry.get<TextureComponent>(actor);
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
		auto view = mRegistry.view<CameraComponent>();
		for (auto actor : view) {
			const auto& camera = view.get<CameraComponent>(actor);
			if (camera.Primary)
				return Actor{ actor, this };
		}
		return {};
	}

	Actor Scene::FindActor(const std::string& name)
	{
		auto view = mRegistry.view<TagComponent>();
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
		mViewportWidth = x;
		mViewportHeight = y;

		auto view = mRegistry.view<CameraComponent>();
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
		component.Camera.SetViewportSize(mViewportWidth, mViewportHeight);
	}
	template<>
	void Scene::OnComponentAdded<PhysicsComponent>(Actor actor, PhysicsComponent& component)
	{
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