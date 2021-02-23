using System.Runtime.CompilerServices;

namespace Sand
{

	public class Log
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern static void Info(string msg);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern static void Warn(string msg);
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern static void Error(string msg);
	}

}
