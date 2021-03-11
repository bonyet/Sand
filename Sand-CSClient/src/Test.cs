using Sand;

namespace Client
{

	class Test
	{
		public float Value = 10.0f;
		public uint OtherThing = 5;

		public void OnCreate()
		{
		}

		public void OnUpdate(float timestep)
		{
			if (Input.IsKeyPressed(Keycode.Space))
			{
				Log.Info("Stuff = " + OtherThing);
			}
		}

		public void OnDestroy()
		{
			Log.Warn("Oh no... no you can't do this to me!");
		}
	}

}