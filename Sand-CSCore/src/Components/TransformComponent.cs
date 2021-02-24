using System.Runtime.CompilerServices;
using Sand.Math;

namespace Sand
{

	public class TransformComponent
	{
		public void SetPosition(Vector3 position)
		{
			SetPosition_Native(position);
		}
		public void SetRotation(Vector3 rotation)
		{
			SetRotation_Native(rotation);
		}
		public void SetScale(Vector3 scale)
		{
			SetScale_Native(scale);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private extern static void SetPosition_Native(Vector3 position);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private extern static void SetRotation_Native(Vector3 rotation);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private extern static void SetScale_Native(Vector3 scale);
	}

}
