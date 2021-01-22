#include <Sand.h>
#include <Sand/Core/EntryPoint.h>

#include "Sandbox2D.h"

namespace Sand 
{

	class SandboxApp : public Application
	{
	public:
		SandboxApp()
			: Application("Sandbox")
		{
			PushLayer(new Sandbox2D());
		}

		~SandboxApp()
		{
		}
	};

	Sand::Application* Sand::CreateApplication()
	{
		return new SandboxApp();
	}

}