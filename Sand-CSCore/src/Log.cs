using System.Runtime.CompilerServices;

namespace Sand
{

	public class Log
	{
		/// <summary>
		/// Logs <paramref name="message"/> to the editor console as info.
		/// </summary>
		/// <param name="message"></param>
		public static void Info(string message) => Info_Native(message);
		/// <summary>
		/// Logs <paramref name="message"/> to the editor console as info.
		/// </summary>
		/// <param name="message"></param>
		public static void Info(object message) => Info_Native(message.ToString());

		/// <summary>
		/// Logs <paramref name="message"/> to the editor console as a warning.
		/// </summary>
		/// <param name="message"></param>
		public static void Warn(string message) => Warn_Native(message);
		/// <summary>
		/// Logs <paramref name="message"/> to the editor console as a warning.
		/// </summary>
		/// <param name="message"></param>
		public static void Warn(object message) => Warn_Native(message.ToString());

		/// <summary>
		/// Logs <paramref name="message"/> to the editor console as an error.
		/// </summary>
		/// <param name="message"></param>
		public static void Error(string message) => Error_Native(message);
		/// <summary>
		/// Logs <paramref name="message"/> to the editor console as an error.
		/// </summary>
		/// <param name="message"></param>
		public static void Error(object message) => Error_Native(message.ToString());

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Info_Native(string msg);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Warn_Native(string msg);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Error_Native(string msg);
	}

}
