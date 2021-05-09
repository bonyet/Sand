using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Sand
{

	[StructLayout(LayoutKind.Sequential)]
	public struct Vector3
	{
		public static readonly Vector3 Zero    = new Vector3(0.0f, 0.0f, 0.0f);
		public static readonly Vector3 One     = new Vector3(1.0f, 1.0f, 1.0f);
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
		public Vector3(double scalar)
		{
			x = y = z = (float)scalar;
		}
		public Vector3(double _x, double _y, double _z)
		{
			x = (float)_x;
			y = (float)_y;
			z = (float)_z;
		}
		public Vector3(Vector2 vec2)
		{
			x = vec2.x;
			y = vec2.y;
			z = 0.0f;
		}
		public Vector3(Vector2 vec2, float _z)
		{
			x = vec2.x;
			y = vec2.y;
			z = _z;
		}

		public float Magnitude { get => (float)GetMagnitude(); }
		public float SqrMagnitude { get => (float)GetMagnitude(); }

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		private double GetMagnitude()
		{
			return Math.Sqrt((x * x) + (y * y) + (z * z));
		}
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		private double GetSqrMagnitude()
		{
			return (x * x) + (y * y) + (z * z);
		}

		/// <summary>
		/// Linearly interpolates between <paramref name="a"/> and <paramref name="b"/>, by <paramref name="t"/>
		/// </summary>
		/// <returns>A new vector that has been interpolated towards <paramref name="b"/></returns>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vector3 Lerp(Vector3 a, Vector3 b, float t)
		{
			t = Math.Clamp01(t);
			return new Vector3(
				a.x + (b.x - a.x) * t, 
				a.y + (b.y - a.y) * t, 
				a.z + (b.z - a.z) * t
			);
		}

		/// <summary>
		/// Moves <paramref name="current"/> towards <paramref name="target"/>, moving no further than <paramref name="maxDistanceDelta"/> each invocation
		/// </summary>
		/// <returns>A new vector that has been interpolated towards <paramref name="target"/></returns>
		public static Vector3 MoveTowards(Vector3 current, Vector3 target, float maxDistanceDelta)
		{
			float toX = target.x - current.x;
			float toY = target.y - current.y;
			float toZ = target.z - current.z;

			float sqrDist = toX * toX + toY * toY + toZ * toZ;

			if (sqrDist == 0 || (maxDistanceDelta >= 0 && sqrDist <= maxDistanceDelta * maxDistanceDelta))
				return target;

			float distance = (float)Math.Sqrt(sqrDist);

			return new Vector3(
				current.x + toX / distance * maxDistanceDelta, 
				current.y + toY / distance * maxDistanceDelta, 
				current.z + toZ / distance * maxDistanceDelta
			);
		}

		/// <summary>
		/// Gradually moves a vector towards another over time
		/// </summary>
		/// <returns>A new vector that has been moved towards the target according to <paramref name="smoothTime"/>, <paramref name="maxSpeed"/>, and <paramref name="deltaTime"/>.</returns>
		public static Vector3 SmoothDamp(Vector3 current, Vector3 target, ref Vector3 currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
		{
			// Based on Game Programming Gems 4 Chapter 1.10
			smoothTime = System.Math.Max(0.0001f, smoothTime);
			float omega = 2.0f / smoothTime;

			float x = omega * deltaTime;
			float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

			float change_x = current.x - target.x;
			float change_y = current.y - target.y;
			float change_z = current.z - target.z;
			Vector3 originalTo = target;

			// Clamp maximum speed
			float maxChange = maxSpeed * smoothTime;

			float maxChangeSq = maxChange * maxChange;
			float sqrmag = change_x * change_x + change_y * change_y + change_z * change_z;
			if (sqrmag > maxChangeSq)
			{
				var mag = (float)Math.Sqrt(sqrmag);
				change_x = change_x / mag * maxChange;
				change_y = change_y / mag * maxChange;
				change_z = change_z / mag * maxChange;
			}

			target.x = current.x - change_x;
			target.y = current.y - change_y;
			target.z = current.z - change_z;

			float temp_x = (currentVelocity.x + omega * change_x) * deltaTime;
			float temp_y = (currentVelocity.y + omega * change_y) * deltaTime;
			float temp_z = (currentVelocity.z + omega * change_z) * deltaTime;

			currentVelocity.x = (currentVelocity.x - omega * temp_x) * exp;
			currentVelocity.y = (currentVelocity.y - omega * temp_y) * exp;
			currentVelocity.z = (currentVelocity.z - omega * temp_z) * exp;

			float output_x = target.x + (change_x + temp_x) * exp;
			float output_y = target.y + (change_y + temp_y) * exp;
			float output_z = target.z + (change_z + temp_z) * exp;

			// Prevent overshooting
			float origMinusCurrent_x = originalTo.x - current.x;
			float origMinusCurrent_y = originalTo.y - current.y;
			float origMinusCurrent_z = originalTo.z - current.z;
			float outMinusOrig_x = output_x - originalTo.x;
			float outMinusOrig_y = output_y - originalTo.y;
			float outMinusOrig_z = output_z - originalTo.z;

			if (origMinusCurrent_x * outMinusOrig_x + origMinusCurrent_y * outMinusOrig_y + origMinusCurrent_z * outMinusOrig_z > 0.0f)
			{
				output_x = originalTo.x;
				output_y = originalTo.y;
				output_z = originalTo.z;

				currentVelocity.x = (output_x - originalTo.x) / deltaTime;
				currentVelocity.y = (output_y - originalTo.y) / deltaTime;
				currentVelocity.z = (output_z - originalTo.z) / deltaTime;
			}

			return new Vector3(output_x, output_y, output_z);
		}

		/// <summary>
		/// Makes this vector have a magnitude of 1.0
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
		public static bool Equal(Vector3 a, Vector3 b, double epsilon)
		{
			return Math.Abs(a.x - b.x) <= epsilon && Math.Abs(a.y - b.y) <= epsilon && Math.Abs(a.z - b.z) <= epsilon;
		}

		// Operators
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vector3 operator +(Vector3 a, Vector3 b)
		{
			return new Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
		}
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vector3 operator -(Vector3 a, Vector3 b)
		{
			return new Vector3(a.x - b.x, a.y - b.y, a.x - b.x);
		}
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vector3 operator *(Vector3 a, float b)
		{
			return new Vector3(a.x * b, a.y * b, a.z * b);
		}
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vector3 operator /(Vector3 a, double divisor)
		{
			return new Vector3(a.x / divisor, a.y / divisor, a.z / divisor);
		}
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Vector3 operator -(Vector3 a)
		{
			return new Vector3(-a.x, -a.y, -a.z);
		}

		// Conversion operators
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static implicit operator Vector2(Vector3 a)
		{
			return new Vector2(a);
		}

		public override string ToString()
		{
			return $"({x}, {y}, {z})";
		}

	}

}
