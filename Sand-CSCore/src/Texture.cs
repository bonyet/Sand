using System.Runtime.CompilerServices;

namespace Sand
{

	public class Texture
	{
		uint Width
		{
			get => GetWidth_Native();
			set => SetWidth_Native(value);
		}
		uint Height
		{
			get => GetHeight_Native();
			set => SetHeight_Native(value);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void SetWidth_Native(uint width);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern uint GetWidth_Native();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void SetHeight_Native(uint height);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern uint GetHeight_Native();
	}

}
