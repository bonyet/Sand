using System;
using System.Runtime.CompilerServices;

namespace Sand
{

	public class Actor
	{
		public uint ID { get; }

		public TransformComponent Transformation { get; private set;  }
		public TagComponent Tag { get; private set; }

		internal Actor(uint id)
		{
			ID = id;

			Transformation = GetComponent<TransformComponent>();
			Tag = GetComponent<TagComponent>();
		}

		public T GetComponent<T>() where T : Component, new()
		{
			if (!HasComponent<T>())
			{
				Log.Error($"{typeof(T).Name} not present on entity {ID}");
				return null;
			}
			
			T component = new T();
			component.actor = this;
			return component;
		}

		public bool HasComponent<T>() where T : Component
		{
			return HasComponent_Native(ID, typeof(T).Name);
		}

		public T AddComponent<T>() where T : Component, new()
		{
			if (HasComponent<T>())
			{
				Log.Error($"{typeof(T).Name} already present on entity {ID}");
				return null;
			}

			AddComponent_Native(ID, typeof(T).Name);
			return GetComponent<T>();
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool HasComponent_Native(uint id, string typeName);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void AddComponent_Native(uint id, string typeName);
	}

}
