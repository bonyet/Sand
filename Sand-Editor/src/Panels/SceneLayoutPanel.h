#pragma once

#include "Sand/Core/Log.h"

#include "Sand/Scene/Scene.h"
#include "Sand/Scene/Actor.h"

#include "EditorPanel.h"

namespace Sand
{

	class SceneLayoutPanel : public EditorPanel
	{
	public:
		SceneLayoutPanel() = default;
		SceneLayoutPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);
		inline Actor GetSelectedActor() const { return m_SelectionContext; }
		inline void SetSelectedActor(Actor entity) { m_SelectionContext = entity; }

		virtual void OnGuiRender() override;
	private:
		void DrawActorNode(Actor entity);
		void DrawActorCreationMenu();
	private:
		Ref<Scene> m_Context;
		Actor m_SelectionContext;
	};

}