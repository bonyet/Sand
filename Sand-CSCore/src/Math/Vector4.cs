using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Sand
{

	[StructLayout(LayoutKind.Sequential)]
	public struct Vector4
	{
		public static readonly Vector4 Zero = new Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		public static readonly Vector4 One = new Vector4(1.0f, 1.0f, 1.0f, 1.0f);

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
		public Vector4(Vector3 vec3)
		{
			x = vec3.x;
			y = vec3.y;
			z = vec3.z;
			w = 0.0f;
		}
		public Vector4(Vector3 vec3, float _w)
		{
			x = vec3.x;
			y = vec3.y;
			z = vec3.z;
			w = _w;
		}

		/// <summary>
		/// Checks if two vectors are approximately equal, using <paramref name="epsilon"/> as reference.
		/// </summary>
		/// <returns>Whether or not <paramref name="a"/> and <paramref name="b"/> are approximately equal.</returns>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static bool Equal(Vector3 a, Vector3 b, double epsilon)
		{
			return Math.Abs(a.x - b.x) <= epsilon && Math.Abs(a.y - b.y) <= epsilon && Math.Abs(a.z - b.z) <= epsilon && Math.Abs(a.z - b.z) <= epsilon;
		}

		public override string ToString()
		{
			return $"({x}, {y}, {z}, {w})";
		}
	}

}
