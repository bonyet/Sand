using System.Runtime.CompilerServices;

namespace Sand
{

	public sealed class TagComponent : Component
	{
		/// <summary>
		/// The name of this actor.
		/// </summary>
		public string Name
		{
			get
			{
				return GetTag_Native(actor.ID);
			}
			set
			{
				SetTag_Native(actor.ID, value);
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void SetTag_Native(uint id, string tag);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern string GetTag_Native(uint id);
	}

}
