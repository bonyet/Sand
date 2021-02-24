using Sand;

namespace Client
{

	class Test
	{
		public float Value = 10.0f;
		public int OtherThing = 5;

		public void OnCreate()
		{
		}

		public void OnUpdate(float timestep)
		{
			if (Input.IsKeyPressed(Keycode.Space))
			{
				Log.Info(OtherThing.ToString());
			}
		}

		public void OnDestroy()
		{
		}
	}

}