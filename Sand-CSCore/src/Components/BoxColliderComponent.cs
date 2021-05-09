using System.Runtime.CompilerServices;

namespace Sand
{

	public class BoxColliderComponent : Component
	{
		public Vector2 Scale
		{
			get
			{
				GetScale_Native(actor.ID, out Vector2 scale);
				return scale;
			}
			set => SetScale_Native(actor.ID, ref value);
		}
		public float Restitution
		{
			get => GetRestitution_Native(actor.ID);
			set => SetRestitution_Native(actor.ID, value);
		}
		public float Friction
		{
			get => GetFriction_Native(actor.ID);
			set => SetFriction_Native(actor.ID, value);
		}
		public bool Observer
		{
			get => IsObserver_Native(actor.ID);
			set => SetObserver_Native(actor.ID, value);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void SetScale_Native(uint id, ref Vector2 scale);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void GetScale_Native(uint id, out Vector2 scale);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void SetRestitution_Native(uint id, float scale);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float GetRestitution_Native(uint id);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void SetFriction_Native(uint id, float friction);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float GetFriction_Native(uint id);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void SetObserver_Native(uint id, bool observer);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool IsObserver_Native(uint id);
	}

}
