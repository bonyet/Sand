#pragma once

#include "Scene.h"

namespace Sand
{

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath); //  yaml
		void SerializeRuntime(const std::string& filepath); //  binary

		bool Deserialize(const std::string& filepath); //  yaml
		bool DeserializeRuntime(const std::string& filepath); //  binary
	private:
		Ref<Scene> m_Scene;
	};

}