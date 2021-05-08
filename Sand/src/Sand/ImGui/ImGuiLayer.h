#pragma once

#include "Sand/Core/Layer.h"

#include "Sand/Events/ApplicationEvent.h"
#include "Sand/Events/KeyEvent.h"
#include "Sand/Events/MouseEvent.h"

namespace Sand 
{

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void SetFontScale(float scale);

		void BlockEvents(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = false;
	};

}