using System.Runtime.CompilerServices;

namespace Sand
{

	/// <summary>
	/// Represents an orthographic camera.
	/// </summary>
	public sealed class CameraComponent : Component
	{
		/// <summary>
		/// The size of this camera. Higher values will 'zoom out' the camera, and lower values will 'zoom in'.
		/// </summary>
		public float Size
		{
			get => GetSize_Native(actor.ID);
			set => SetSize_Native(actor.ID, value);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float GetSize_Native(uint entityID);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void SetSize_Native(uint entityID, float size);
	}

}
