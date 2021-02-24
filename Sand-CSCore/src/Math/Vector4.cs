using System.Runtime.InteropServices;

namespace Sand.Math
{

	[StructLayout(LayoutKind.Sequential)]
	public struct Vector4
	{
		public static readonly Vector3 Zero    = new Vector3(0.0f, 0.0f, 0.0f);

		public float x, y, z, w;

		public Vector4(float scalar)
		{
			x = y = z = w = scalar;
		}
		public Vector4(float _x, float _y, float _z, float _w)
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}
	}

}
