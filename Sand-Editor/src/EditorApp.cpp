#include <Sand.h>
#include <Sand/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Sand 
{

	class SandEditor : public Application
	{
	public:
		SandEditor()
			: Application("Sand Editor")
		{
			PushLayer(new EditorLayer());
		}

		~SandEditor()
		{
		}
	};

	Sand::Application* Sand::CreateApplication()
	{
		return new SandEditor();
	}

}