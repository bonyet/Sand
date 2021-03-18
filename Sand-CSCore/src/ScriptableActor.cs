using System.Runtime.CompilerServices;

namespace Sand
{

	public class ScriptableActor
	{
		private Actor actor;

		public TransformComponent Transformation { get => actor.Transformation; }
		public TagComponent Tag { get => actor.Tag; }

		internal void Initialize(uint entityID)
		{
			actor = new Actor(entityID);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public T GetComponent<T>() where T : Component, new()
		{
			return actor.GetComponent<T>();
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public bool HasComponent<T>() where T : Component
		{
			return actor.HasComponent<T>();
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public T AddComponent<T>() where T : Component, new()
		{
			return actor.AddComponent<T>();
		}

	}

}
