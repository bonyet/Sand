#pragma once

#include "Sand/Core/Base.h"
#include "Sand/Renderer/Shader.h"

namespace Sand 
{

	class Material
	{
	public:
		virtual ~Material() = default;

		static Ref<Material> Create();
		static Ref<Material> Create(const std::string& filepath);
		static Ref<Material> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		const Ref<Shader>& GetShader() const { return m_Shader; };
		void SetShader(const Ref<Shader>& shader) { m_Shader = shader; };
	protected:
		Ref<Shader> m_Shader;
	public:
		glm::vec4 Color = { 1, 1, 1, 1 };
	};

}