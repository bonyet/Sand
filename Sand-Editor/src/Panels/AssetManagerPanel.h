#pragma once

#include "EditorPanel.h"

namespace Sand
{

	class AssetManagerPanel : public EditorPanel
	{
	public:
		AssetManagerPanel() = default;

		virtual void OnGuiRender() override;
	};

}