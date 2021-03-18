#pragma once

#include "EditorPanel.h"
#include <string>

namespace Sand
{

	class AssetManagerPanel : public EditorPanel
	{
	public:
		enum class FileType
		{
			Unknown = -1,

			Folder, Text, Script, Scene, Font, Shader, Image
		};
	public:
		AssetManagerPanel();
		virtual void OnGuiRender() override;

		void SetPath(const std::string& path);
		std::string& GetPath() { return mFilepath; }
	private:
		std::string mFilepath;
	};

}