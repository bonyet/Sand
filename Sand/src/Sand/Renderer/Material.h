#pragma once

#include "Sand/Core/Base.h"
#include "Shader.h"

namespace Sand
{

	class Material
	{
	public:
		Material(const Ref<Shader>& shader);
		~Material();

		void SetShader(const Ref<Shader>& shader) { m_Shader = shader; }
		Ref<Shader> GetShader() const { return m_Shader; }

		inline void SetInt(const std::string& name, int value)                        { m_Shader->SetInt(name, value); }
		inline void SetIntArray(const std::string& name, int* values, uint32_t count) { m_Shader->SetIntArray(name, values, count); }
		inline void SetFloat(const std::string& name, float value)                    { m_Shader->SetFloat(name, value); }
		inline void SetFloat2(const std::string& name, const glm::vec2& value)        { m_Shader->SetFloat2(name, value); }
		inline void SetFloat3(const std::string& name, const glm::vec3& value)        { m_Shader->SetFloat3(name, value); }
		inline void SetFloat4(const std::string& name, const glm::vec4& value)        { m_Shader->SetFloat4(name, value); }
		inline void SetMat4(const std::string& name, const glm::mat4& value)          { m_Shader->SetMat4(name, value); }

		static Ref<Material> Create(const Ref<Shader>& shader);
	private:
		Ref<Shader> m_Shader;
	};

}