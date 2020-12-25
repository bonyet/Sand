#include "sandpch.h"
#include "OpenGLMaterial.h"

#include <glad\glad.h>
#include <string>

namespace Sand 
{
	OpenGLMaterial::OpenGLMaterial()
	{
	}
	OpenGLMaterial::OpenGLMaterial(const std::string& filepath)
	{
		m_Shader = Shader::Create(filepath);
	}

	OpenGLMaterial::OpenGLMaterial(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		m_Shader = Shader::Create(name, vertexSrc, fragmentSrc);
	}

}