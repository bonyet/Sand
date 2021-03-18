#pragma once

#include "Sand/Core/Time.h"
#include "Sand/Scene/Actor.h"

namespace Sand
{

	enum class ScriptDataType
	{
		Unknown,

		Float, Double, Int, UInt,
		Vector2, Vector3, Vector4,

		Color,
	};

	class Scene;

	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void SetActiveScene(Scene* const scene);

		static void ReloadClientAssembly();

		static void AddModule(uint32_t actorID, const std::string& moduleName);
		static void DeactivateModule(Actor actor);
		static bool ModuleActive(Actor actor);
		static bool ModuleExists(const std::string& moduleName);

		static struct ScriptData* GetScriptData(uint32_t actor, const std::string& moduleName);

		static ScriptDataType MonoTypeToScriptDataType(void* type);
		static bool MonoFieldIsPublic(void* field);

		// void* is actually MonoClass! wow! same but different   !
		static std::vector<void*> GetCachedClientScripts();
	private:
		static void CreateAll();
		static void UpdateAll(Timestep ts);
		static void DestroyAll();

		static void LoadClientAssembly();
	private:
		static void SetupInternalCalls();

		friend class Scene;
	};

}