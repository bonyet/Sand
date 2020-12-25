#pragma once

#include <string>

namespace Sand
{

	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter, const std::string& defaultFileExtension = ".sscene");
	};

}