using System.Runtime.InteropServices;

namespace Sand.Math
{

	[StructLayout(LayoutKind.Sequential)]
	public struct Vector3
	{
		public static readonly Vector3 Zero    = new Vector3(0.0f, 0.0f, 0.0f);
		public static readonly Vector3 Forward = new Vector3(0.0f, 0.0f,-1.0f);
		public static readonly Vector3 Right   = new Vector3(1.0f, 0.0f, 0.0f);
		public static readonly Vector3 Up      = new Vector3(0.0f, 1.0f, 0.0f);

		public float x, y, z;

		public Vector3(float scalar)
		{
			x = y = z = scalar;
		}
		public Vector3(float _x, float _y, float _z)
		{
			x = _x;
			y = _y;
			z = _z;
		}
	}

}
