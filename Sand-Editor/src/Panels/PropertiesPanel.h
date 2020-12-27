#pragma once

#include "EditorPanel.h"
#include "Sand/Scene/Entity.h"

namespace Sand
{

	class PropertiesPanel : public EditorPanel
	{
	public:
		PropertiesPanel() = default;

		virtual void OnGuiRender() override;
		void SetSelection(Entity entity) { m_SelectionContext = entity; }
	private:
		void DrawComponents(Entity entity);
		void DrawComponentsMenu(Entity entity);

		Entity m_SelectionContext;
	};

}