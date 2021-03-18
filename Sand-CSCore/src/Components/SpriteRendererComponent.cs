using System.Runtime.CompilerServices;

namespace Sand
{

	public class SpriteRendererComponent : Component
	{
		public void SetColor(Color color)
		{
			SetColor_Native(color, actor.ID);
		}
		public Color GetColor()
		{
			GetColor_Native(out Color color, actor.ID);
			return color;
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void SetColor_Native(Color color, uint id);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void GetColor_Native(out Color color, uint id);
	}

}
