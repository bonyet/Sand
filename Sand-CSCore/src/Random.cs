using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace Sand
{
	
	/// <summary>
	/// A trivial wrapper around a System.Random object, with added convenience functions
	/// </summary>
	public static class PsuedoRandom
	{
		private static Random random;

		/// <summary>
		/// Returns a random integer greater or equal to <paramref name="min"/> and less than <paramref name="max"/>
		/// </summary>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static int RandomInteger(int min, int max)
		{
			return random.Next(min, max);
		}
		/// <summary>
		/// Returns a random double greater or equal to <paramref name="min"/> and less than <paramref name="max"/>
		/// </summary>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static double RandomDouble(double min, double max)
		{
			return random.NextDouble() * (max - min) + min;
		}

		/// <summary>
		/// Convenience function to pick a random <typeparamref name="T"/> from an array
		/// </summary>
		/// <returns>A random element - type of <typeparamref name="T"/> - within <paramref name="array"/></returns>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static T RandomFromArray<T>(T[] array)
		{
			return array[RandomInteger(0, array.Length - 1)];
		}
		/// <summary>
		/// Convenience function to pick a random element from a <typeparamref name = "List"/> of <typeparamref name="T"/>
		/// </summary>
		/// <returns>A random element - type of <typeparamref name="T"/> - within <paramref name="list"/></returns>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static T RandomFromList<T>(List<T> list)
		{
			return list[RandomInteger(0, list.Count - 1)];
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static Random GetRandom() => random;
	}

}
