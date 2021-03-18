using System;
using System.Runtime.InteropServices;

namespace Sand
{

	/// <summary>
	/// Represents an RGBA color.
	/// </summary>
	[StructLayout(LayoutKind.Sequential)]
	public struct Color
	{
		public static readonly Color White = new Color(1.0f, 1.0f, 1.0f, 1.0f);
		public static readonly Color Black = new Color(0.0f, 0.0f, 0.0f, 1.0f);

		public float r, g, b, a;

		public Color(float value)
		{
			r = g = b = a = value;
		}
		public Color(float _r, float _g, float _b, float _a)
		{
			r = _r;
			g = _g;
			b = _b;
			a = _a;
		}
		public Color(Math.Vector4 values)
		{
			r = values.x;
			g = values.y;
			b = values.z;
			a = values.w;
		}
	}

}
