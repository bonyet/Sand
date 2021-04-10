#pragma once

#include "EditorPanel.h"

#include <string>
#include <filesystem>

namespace Sand
{
	enum class AssetFileType
	{
		Unknown = -1,

		Folder, Document, Script, Scene, Font, Shader, Image
	};

	class AssetManagerPanel : public EditorPanel
	{
	public:
		AssetManagerPanel();
		virtual void OnGuiRender() override;

		void SetPath(const std::string& path);
		std::string& GetPath() { return m_Filepath; }
	private:
		std::string m_Filepath;
		std::filesystem::directory_entry m_CurrentDirectory;
	};

}