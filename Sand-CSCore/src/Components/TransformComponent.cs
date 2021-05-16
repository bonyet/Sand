using System.Runtime.CompilerServices;

namespace Sand
{

	/// <summary>
	/// Represents an actor's transformation in the scene.
	/// </summary>
	public sealed class TransformComponent : Component
	{
		public void SetPosition(Vector2 position)
		{
			SetPosition_Native(actor.ID, ref position);
		}
		public void SetRotation(float rotation)
		{
			SetRotation_Native(actor.ID, ref rotation);
		}
		public void SetScale(Vector2 scale)
		{
			SetScale_Native(actor.ID, ref scale);
		}

		public Vector2 GetPosition()
		{
			GetPosition_Native(actor.ID, out Vector2 pos);
			return pos;
		}
		public float GetRotation()
		{
			GetRotation_Native(actor.ID, out float rotation);
			return rotation;
		}
		public Vector2 GetScale()
		{
			GetScale_Native(actor.ID, out Vector2 scale);
			return scale;
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void SetPosition_Native(uint actorID, ref Vector2 position);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void SetRotation_Native(uint actorID, ref float rotation);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void SetScale_Native(uint actorID, ref Vector2 scale);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void GetPosition_Native(uint actorID, out Vector2 position);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void GetRotation_Native(uint actorID, out float rotation);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void GetScale_Native(uint actorID, out Vector2 scale);
	}

}
