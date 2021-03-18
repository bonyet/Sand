using Sand;
using Sand.Math;
using Sand.Scene;

namespace Client
{

	public class Test : ScriptableActor
	{
		private Vector2 position;
		public Color color;

		private void OnCreate()
		{
			color = Color.White;

			position = Transformation.GetPosition();

			Log.Info(Tag.Name);
		}

		private void OnUpdate(float timestep)
		{
			if (Input.IsKeyPressed(Keycode.W))
				position.y += 0.1f;
			else if (Input.IsKeyPressed(Keycode.S))
				position.y -= 0.1f;
			if (Input.IsKeyPressed(Keycode.D))
				position.x += 0.1f;
			else if (Input.IsKeyPressed(Keycode.A))
				position.x -= 0.1f;

			Transformation.SetPosition(position);
			
			GetComponent<SpriteRendererComponent>().SetColor(color);
		}

		private void OnDestroy()
		{
		}
	}

}