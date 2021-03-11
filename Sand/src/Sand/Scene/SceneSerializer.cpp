#include "sandpch.h"
#include "SceneSerializer.h"

#include "Actor.h"

#include "Components.h"

#include <fstream>

#include <yaml-cpp/yaml.h>

namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}
		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}
		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Sand
{

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: mScene(scene)
	{
	}

	static void SerializeActor(YAML::Emitter& out, Actor actor)
	{
		out << YAML::BeginMap; // Actor
		out << YAML::Key << "Actor" << YAML::Value << "123456789"; // actor id goes here		

		if (actor.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			auto& tag = actor.GetComponent<TagComponent>().Name;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; // TagComponent
		}
		if (actor.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& transform = actor.GetComponent<TransformComponent>();
			out << YAML::Key << "Position" << YAML::Value << transform.Position;
			out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transform.Scale;

			out << YAML::EndMap; // TransformComponent
		}
		if (actor.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cameraComponent = actor.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap; // Camera

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			out << YAML::EndMap; // CameraComponent
		}

		if (actor.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent

			auto& spriteRenderer = actor.GetComponent<SpriteRendererComponent>();

			out << YAML::Key << "Color" << YAML::Value << spriteRenderer.Color;

			out << YAML::EndMap; // SpriteRendererComponent
		}
		if (actor.HasComponent<TextureComponent>())
		{
			out << YAML::Key << "TextureComponent";
			out << YAML::BeginMap; // TextureComponent

			auto& texture = actor.GetComponent<TextureComponent>();

			out << YAML::Key << "TextureFilepath" << YAML::Value << (texture.Texture ? texture.Texture->GetPath() : "");
			out << YAML::Key << "TilingFactor" << YAML::Value << texture.TilingFactor;

			out << YAML::EndMap; // TextureComponent
		}
		if (actor.HasComponent<PhysicsComponent>())
		{
			out << YAML::Key << "PhysicsComponent";
			out << YAML::BeginMap; // TextureComponent

			auto& physics = actor.GetComponent<PhysicsComponent>();

			out << YAML::Key << "Some random shit.";

			out << YAML::EndMap; // PhysicsComponent
		}

		out << YAML::EndMap; // Actor
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Unnamed Scene";
		out << YAML::Key << "Actors" << YAML::Value << YAML::BeginSeq;
		mScene->mRegistry.each([&](auto entityID)
		{
			Actor actor = { entityID, mScene.get() };
			if (!actor)
				return;

			SerializeActor(out, actor);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		// Not implemented
		SAND_CORE_ASSERT(false, "SerializeRuntime() not implemented.");
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data;
		try 
		{ 
			data = YAML::LoadFile(filepath); 
		}
		catch (const YAML::ParserException& ex)
		{
			SAND_CORE_ERROR("Failed to load YAML file '{0}'\n {1}", filepath, ex.what());
			return false;
		}

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		SAND_CORE_TRACE("Deserializing Scene '{0}'", sceneName);

		auto actors = data["Actors"];
		if (actors)
		{
			for (auto actor : actors)
			{
				uint64_t uuid = actor["Actor"].as<uint64_t>(); // todo

				std::string name;
				auto tagComponent = actor["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				SAND_CORE_TRACE("Deserialized actor with ID = {0}, name = {1}", uuid, name);

				Actor deserializedActor = mScene->CreateActor(name);

				auto transformComponent = actor["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedActor.GetComponent<TransformComponent>(); // always has transform
					tc.Position = transformComponent["Position"].as<glm::vec2>();
					tc.Rotation = transformComponent["Rotation"].as<float>();
					tc.Scale = transformComponent["Scale"].as<glm::vec2>();
				}
				auto cameraComponent = actor["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedActor.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}
				auto spriteRendererComponent = actor["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedActor.AddComponent<SpriteRendererComponent>();

					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
				}
				auto textureComponent = actor["TextureComponent"];
				if (textureComponent)
				{
					auto& tc = deserializedActor.AddComponent<TextureComponent>();

					std::string path = textureComponent["TextureFilepath"].as<std::string>();
					tc.Texture = !path.empty() ? Texture2D::Create(path) : nullptr;
					tc.TilingFactor = textureComponent["TilingFactor"].as<float>();
				}
				auto physicsComponent = actor["PhysicsComponent"];
				if (physicsComponent)
				{
					deserializedActor.AddComponent<PhysicsComponent>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// Not implemented
		SAND_CORE_ASSERT(false, "DeserializeRuntime() not implemented.");
		return false;
	}


}