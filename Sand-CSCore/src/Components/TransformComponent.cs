using System.Runtime.CompilerServices;

namespace Sand
{
	using Math;

	public class TransformComponent : Component
	{
		public void SetPosition(Vector3 position)
		{
			SetPosition_Native(actor.ID, position);
		}
		public void SetRotation(Vector3 rotation)
		{
			SetRotation_Native(actor.ID, rotation);
		}
		public void SetScale(Vector3 scale)
		{
			SetScale_Native(actor.ID, scale);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private extern static void SetPosition_Native(uint actorID, Vector3 position);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private extern static void SetRotation_Native(uint actorID, Vector3 rotation);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private extern static void SetScale_Native(uint actorID, Vector3 scale);
	}

}
