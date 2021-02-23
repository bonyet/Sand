using Sand;

namespace Client
{

	class Test
	{
		public void OnCreate()
		{
			Log.Warn("Hello from C# :)");
		}

		public void OnUpdate(float timestep)
		{
			Log.Warn("Bone apple tea from C# -_-");
		}

		public void OnDestroy()
		{
			Log.Warn("Goodbye from C# :(");
		}
	}

	class Test2
	{
		public void OnCreate()
		{
			Log.Warn("aaaa from C# :)");
		}

		public void OnUpdate(float timestep)
		{
		}

		public void OnDestroy()
		{
			Log.Warn("aaaa f erom C# :(");
		}
	}

}
