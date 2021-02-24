using System.Runtime.InteropServices;

namespace Sand.Math
{

	[StructLayout(LayoutKind.Sequential)]
	public struct Vector2
	{
		public static readonly Vector2 Zero = new Vector2(0.0f, 0.0f);

		public float x, y;

		public Vector2(float scalar)
		{
			x = y = scalar;
		}
		public Vector2(float _x, float _y)
		{
			x = _x;
			y = _y;
		}
	}

}
