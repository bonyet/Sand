using System;
using System.Runtime.CompilerServices;

namespace Sand.Scene
{

	public class Scene
	{
		public static Actor FindActorByName(string name)
		{
			int found = FindActorByName_Native(name);
			if (found >= 0)
				return new Actor((uint)found);

			Log.Error($"Actor '{name}' not present in scene.");
			return null;
		}

		public static uint GetNumberOfActors()
		{
			return GetNumberOfActors_Native();
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern uint GetNumberOfActors_Native();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern int FindActorByName_Native(string name);
	}

}
