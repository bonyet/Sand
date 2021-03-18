#pragma once

#include "EditorPanel.h"
#include "Sand/Scene/Actor.h"

namespace Sand
{

	class PropertiesPanel : public EditorPanel
	{
	public:
		PropertiesPanel() = default;

		virtual void OnGuiRender() override;
		void SetSelection(Actor actor) { m_SelectionContext = actor; }
		Actor GetSelection() const { return m_SelectionContext; }
	private:
		void DrawComponents(Actor actor);
		void DrawComponentsMenu(Actor actor);

		Actor m_SelectionContext;
	};

}