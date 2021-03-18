using System.Runtime.CompilerServices;

namespace Sand
{

	using Math;

	public class Log
	{
		// Overloads
		public static void Info(Vector2 msg) => Info($"({msg.x}, {msg.y})");
		public static void Info(Vector3 msg) => Info($"({msg.x}, {msg.y}, {msg.z})");
		public static void Info(Vector4 msg) => Info($"({msg.x}, {msg.y}, {msg.z}, {msg.w})");

		public static void Warn(Vector2 msg) => Warn($"({msg.x}, {msg.y})");
		public static void Warn(Vector3 msg) => Warn($"({msg.x}, {msg.y}, {msg.z})");
		public static void Warn(Vector4 msg) => Warn($"({msg.x}, {msg.y}, {msg.z}, {msg.w})");

		public static void Error(Vector2 msg) => Error($"({msg.x}, {msg.y})");
		public static void Error(Vector3 msg) => Error($"({msg.x}, {msg.y}, {msg.z})");
		public static void Error(Vector4 msg) => Error($"({msg.x}, {msg.y}, {msg.z}, {msg.w})");

		// Base ones
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern static void Info(string msg);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern static void Warn(string msg);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern static void Error(string msg);
	}

}
