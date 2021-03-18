using Sand;
using Sand.Math;
using Sand.Scene;

namespace Client
{

	public class CameraController : ScriptableActor
	{
		private TransformComponent target;
		private CameraComponent camera;
		private float size;

		// Follow stuff
		public float smoothSpeed;
		private Vector3 currentVelocity;

		private void OnCreate()
		{
			camera = GetComponent<CameraComponent>();

			target = Scene.FindActorByName("Actor").Transformation;
		}

		private void OnLateUpdate(float timestep)
		{
			const float maxSpeed = Math.Infinity;
			Vector2 targetPosition = Vector3.SmoothDamp(Transformation.GetPosition(), target.GetPosition(), ref currentVelocity, smoothSpeed, maxSpeed, timestep);

			Transformation.SetPosition(targetPosition);

			if (Input.IsKeyPressed(Keycode.Space))
			{
				size += 0.1f;
				camera.Size = size;
			}
			else if (Input.IsKeyPressed(Keycode.LeftShift))
			{
				size -= 0.1f;
				camera.Size = size;
			}
		}
	}

}
