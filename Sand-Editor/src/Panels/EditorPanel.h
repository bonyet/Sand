#pragma once

namespace Sand
{

	class EditorPanel
	{
	public:
		virtual ~EditorPanel() {}

		virtual void OnGuiRender() = 0 {}
		
		void ShowWindow() { m_ShowWindow = true; }
		void HideWindow() { m_ShowWindow = false; }
	protected:
		bool m_ShowWindow = true;
	};

}