#pragma once

#include "ScriptData.h"

namespace Sand
{

	enum class ScriptFieldType
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

		static bool IsModuleRegistered(uint32_t actorID);
		static void RegisterModule(uint32_t actorID, const std::string& moduleName);
		static void UnregisterModule(uint32_t actorID);

		static ScriptFieldType MonoTypeToScriptDataType(MonoType* type);
		static bool MonoFieldIsPublic(MonoClassField* field);

		static MonoImage* GetMonoImage();
		static std::vector<std::string>& GetClientScriptNames();

		static ScriptData& GetScriptDataFromActor(uint32_t actorID);
	private:
		static void LoadClientAssembly();
		static void RegisterInternalCalls();

		friend class Scene;
	};

}