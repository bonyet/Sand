#include <Sand.h>
#include <Sand/Core/EntryPoint.h>

namespace Sand {

	class SandboxApp : public Application
	{
	public:
		SandboxApp()
		{
			//PushLayer(new Sandinclude/box2d());
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