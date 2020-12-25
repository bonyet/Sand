#include "sandpch.h"
#include "Material.h"
#include "Sand/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLMaterial.h"

namespace Sand 
{
	Ref<Material> Material::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    SAND_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLMaterial>();
		}

		SAND_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Ref<Material> Material::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    SAND_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLMaterial>(filepath);
		}

		SAND_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Material> Material::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    SAND_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLMaterial>(name, vertexSrc, fragmentSrc);
		}

		SAND_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}