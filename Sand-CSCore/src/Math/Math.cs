using System.Runtime.CompilerServices;

namespace Sand
{

	public static class Math
	{
		public const float Infinity = float.PositiveInfinity;

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static float Abs(float number) => System.Math.Abs(number);
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static double Abs(double number) => System.Math.Abs(number);

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static float Sqrt(float number) => (float)System.Math.Sqrt(number);
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static double Sqrt(double number) => System.Math.Sqrt(number);

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static float Clamp01(float value)
		{
			if (value < 0.0f)
				return 0.0f;
			else if (value > 1.0f)
				return 1.0f;
			else
				return value;
		}
	}

}
