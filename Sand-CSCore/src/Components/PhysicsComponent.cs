using System.Runtime.CompilerServices;
	
namespace Sand
{

	public enum PhysicsBodyType
	{
		Static, Kinematic, Dynamic
	}

	public sealed class PhysicsComponent : Component
	{
		public PhysicsBodyType Type
		{
			get => (PhysicsBodyType)GetType_Native(actor.ID);
			set => SetMass_Native(actor.ID, (int)value);
		}
		/// <summary>
		/// The mass of this physics body in kilograms.
		/// </summary>
		public float Mass
		{
			get => GetMass_Native(actor.ID);
			set => SetMass_Native(actor.ID, value);
		}
		public float GravityScale
		{
			get => GetGravityScale_Native(actor.ID);
			set => SetGravityScale_Native(actor.ID, value);
		}
		public bool FixedRotation
		{
			get => GetFixedRotation_Native(actor.ID);
			set => SetFixedRotation_Native(actor.ID, value);
		}

		/// <summary>
		/// Applies a linear impulse at the center of this physics body.
		/// </summary>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public void ApplyLinearImpulse(Vector2 impulse, bool wake = true)
		{
			ApplyLinearImpulse_Native(actor.ID, ref impulse, wake);
		}
		/// <summary>
		/// Applies a force ta the center of this physics body.
		/// </summary>
		/// <param name="impulse"></param>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public void ApplyForce(Vector2 force, bool wake = true)
		{
			ApplyForce_Native(actor.ID, ref force, wake);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void SetType_Native(uint id, int type);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern int GetType_Native(uint id);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void SetMass_Native(uint id, float mass);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float GetMass_Native(uint id);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void SetGravityScale_Native(uint id, float gravityScale);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float GetGravityScale_Native(uint id);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void SetFixedRotation_Native(uint id, bool fixedRotation);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool GetFixedRotation_Native(uint id);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void ApplyLinearImpulse_Native(uint id, ref Vector2 impulse, bool wake);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void ApplyForce_Native(uint id, ref Vector2 force, bool wake);
	}

}
