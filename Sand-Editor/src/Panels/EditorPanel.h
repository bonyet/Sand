#pragma once

namespace Sand
{

	class EditorPanel
	{
	public:
		virtual ~EditorPanel() {}

		virtual void OnGuiRender() = 0 {}
		
		void Show() { m_ShowWindow = true; }
		void Hide() { m_ShowWindow = false; }
	protected:
		bool m_ShowWindow = true;
	};

}