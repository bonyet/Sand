#pragma once

#include "Sand/Renderer/Material.h"

namespace Sand 
{

	class OpenGLMaterial : public Material
	{
	public:
		OpenGLMaterial();
		OpenGLMaterial(const std::string& filepath);
		OpenGLMaterial(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		virtual ~OpenGLMaterial() {}
	};

}