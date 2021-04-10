#pragma once

#include <mono/metadata/reflection.h>

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

		static MonoImage* GetMonoImage();

		static ScriptDataType MonoTypeToScriptDataType(MonoType* type);
		static bool MonoFieldIsPublic(MonoClassField* field);
	private:
		static void LoadClientAssembly();
		static void RegisterInternalCalls();

		friend class Scene;
	};

}