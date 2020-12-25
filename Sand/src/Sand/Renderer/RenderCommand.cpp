#include "sandpch.h"
#include "Sand/Renderer/RenderCommand.h"

namespace Sand 
{

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}