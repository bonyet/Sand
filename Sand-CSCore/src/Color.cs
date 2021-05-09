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
		public static readonly Color Red   = new Color(1.0f, 0.0f, 0.0f, 1.0f);
		public static readonly Color Green = new Color(0.0f, 1.0f, 0.0f, 1.0f);
		public static readonly Color Blue  = new Color(0.0f, 0.0f, 1.0f, 1.0f);

		public float r, g, b, a;

		/// <summary>
		/// Creates a <c>Color</c> from a single float. Assigns <c>r, g, b, a</c> to <paramref name="value"/>
		/// </summary>
		/// <param name="values"></param>
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
		/// <summary>
		/// Creates a <c>Color</c> from a Vector4. Assigns <c>r, g, b, a</c> to <c>x, y, z, w</c>, respectively.
		/// </summary>
		/// <param name="values"></param>
		public Color(Vector4 values)
		{
			r = values.x;
			g = values.y;
			b = values.z;
			a = values.w;
		}
		/// <summary>
		/// Creatse a <c>Color</c> from a Vector3 and sets <c>a</c> to 0.
		/// </summary>
		/// <param name="values"></param>
		public Color(Vector3 values)
		{
			r = values.x;
			g = values.y;
			b = values.z;
			a = 0.0f;
		}
	}

}
