#pragma once

#include <string>

namespace Sand
{

	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter, const char* initialDirectory = "assets");
		static std::string SaveFile(const char* filter, const char* initialDirectory = "assets", const std::string& defaultFileExtension = ".sscene");
	};

}