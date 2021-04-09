using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Sand.Math
{

	[StructLayout(LayoutKind.Sequential)]
	public struct Vector2
	{
		public static readonly Vector2 Up = new Vector2(0.0f, 1.0f);
		public static readonly Vector2 One = new Vector2(1.0f, 1.0f);
		public static readonly Vector2 Zero = new Vector2(0.0f, 0.0f);
		public static readonly Vector2 Right = new Vector2(1.0f, 0.0f);

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
		public Vector2(double scalar)
		{
			x = y = (float)scalar;
		}
		public Vector2(double _x, double _y)
		{
			x = (float)_x;
			y = (float)_y;
		}
		public Vector2(Vector3 vec3)
		{
			x = vec3.x;
			y = vec3.y;
		}

		public float Magnitude { get => (float)GetMagnitude(); }
		public float SqrMagnitude { get => (float)GetMagnitude(); }

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		private double GetMagnitude()
		{
			return Math.Sqrt((x * x) + (y * y));
		}
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		private double GetSqrMagnitude()
		{
			return (x * x) + (y * y);
		}

		/// <summary>
		/// Linearly interpolates between <paramref name="a"/> and <paramref name="b"/>, by <paramref name="t"/>.
		/// </summary>
		/// <returns>A new vector that has been interpolated towards <paramref name="b"/>.</returns>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vector2 Lerp(Vector2 a, Vector2 b, float t)
		{
			t = Math.Clamp01(t);
			return new Vector2(
				a.x + (b.x - a.x) * t, 
				a.y + (b.y - a.y) * t
			);
		}

		/// <summary>
		/// Moves <paramref name="current"/> towards <paramref name="target"/>, moving no further than <paramref name="maxDistanceDelta"/> each call.
		/// </summary>
		/// <returns>A new vector that has been interpolated towards <paramref name="target"/>.</returns>
		public static Vector2 MoveTowards(Vector2 current, Vector2 target, float maxDistanceDelta)
		{
			float toX = target.x - current.x;
			float toY = target.y - current.y;

			float sqrDist = toX * toX + toY * toY;

			if (sqrDist == 0 || (maxDistanceDelta >= 0 && sqrDist <= maxDistanceDelta * maxDistanceDelta))
				return target;

			float distance = (float)Math.Sqrt(sqrDist);

			return new Vector2(
				current.x + toX / distance * maxDistanceDelta, 
				current.y + toY / distance * maxDistanceDelta
			);
		}

		/// <summary>
		/// Makes this vector have a magnitude of 1.0.
		/// </summary>
		public void Normalize()
		{
			const float epsilon = 0.00001f;

			float mag = Magnitude;

			if (mag > epsilon)
				this /= mag;
			else
				this = Zero;
		}

		/// <summary>
		/// Checks if two vectors are approximately equal, using <paramref name="epsilon"/> as reference.
		/// </summary>
		/// <returns>Whether or not <paramref name="a"/> and <paramref name="b"/> are approximately equal.</returns>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static bool Equal(Vector2 a, Vector2 b, double epsilon)
		{
			return Math.Abs(a.x - b.x) <= epsilon && Math.Abs(a.y - b.y) <= epsilon;
		}

		// Operators
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vector2 operator+(Vector2 a, Vector2 b)
		{
			return new Vector2(a.x + b.x, a.y + b.y);
		}
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vector2 operator-(Vector2 a, Vector2 b)
		{
			return new Vector2(a.x - b.x, a.y - b.y);
		}
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vector2 operator*(Vector2 a, float b)
		{
			return new Vector2(a.x * b, a.y * b);
		}
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vector2 operator/(Vector2 a, double divisor)
		{
			return new Vector2(a.x / divisor, a.y / divisor);
		}
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vector2 operator-(Vector2 a)
		{
			return new Vector2(-a.x, -a.y);
		}

		// Conversion operators
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static implicit operator Vector3(Vector2 a)
		{
			return new Vector3(a);
		}

		public override string ToString()
		{
			return $"({x}, {y})";
		}

	}

}
