#pragma once

#include <string>
#include <glm/glm.hpp>

#include <vector>
#include "Sand/Core/Time.h"

namespace Sand
{

	class Debug
	{
	public:
		static constexpr glm::vec3 InfoMessageColor{ 0.8f, 0.8f, 0.8f };
		static constexpr glm::vec3 WarningMessageColor{ 1.0f, 0.1f, 0.9f };
		static constexpr glm::vec3 ErrorMessageColor{ 1.0f, 0.2f, 0.2f };
	public:
		static void PrintInfo(const std::string& value)
		{
			m_DebugMessages.push_back({ Time::GetSystemTime(), InfoMessageColor, value });
		}
		static void PrintInfo(const char* value)
		{
			m_DebugMessages.push_back({ Time::GetSystemTime(), InfoMessageColor, value });
		}
		static void PrintInfo(int value)
		{
			m_DebugMessages.push_back({ Time::GetSystemTime(), InfoMessageColor, std::to_string(value) });
		}
		static void PrintInfo(long value)
		{
			m_DebugMessages.push_back({ Time::GetSystemTime(), InfoMessageColor, std::to_string(value) });
		}
		static void PrintInfo(float value)
		{
			m_DebugMessages.push_back({ Time::GetSystemTime(), InfoMessageColor, std::to_string(value) });
		}
		static void PrintInfo(double value)
		{
			m_DebugMessages.push_back({ Time::GetSystemTime(), InfoMessageColor, std::to_string(value) });
		}

		static void PrintWarning(const std::string& value)
		{
			m_DebugMessages.push_back({ Time::GetSystemTime(), WarningMessageColor, value });
		}
		static void PrintWarning(const char* value)
		{
			m_DebugMessages.push_back({ Time::GetSystemTime(), WarningMessageColor, value });
		}
		static void PrintWarning(int value)
		{
			m_DebugMessages.push_back({ Time::GetSystemTime(), WarningMessageColor, std::to_string(value) });
		}
		static void PrintWarning(long value)
		{
			m_DebugMessages.push_back({ Time::GetSystemTime(), WarningMessageColor, std::to_string(value) });
		}
		static void PrintWarning(float value)
		{
			m_DebugMessages.push_back({ Time::GetSystemTime(), WarningMessageColor, std::to_string(value) });
		}
		static void PrintWarning(double value)
		{
			m_DebugMessages.push_back({ Time::GetSystemTime(), WarningMessageColor, std::to_string(value) });
		}

		static void PrintError(const std::string& value)
		{
			m_DebugMessages.push_back({ Time::GetSystemTime(), ErrorMessageColor, value });
		}
		static void PrintError(const char* value)
		{
			m_DebugMessages.push_back({ Time::GetSystemTime(), ErrorMessageColor, value });
		}
		static void PrintError(int value)
		{
			m_DebugMessages.push_back({ Time::GetSystemTime(), ErrorMessageColor, std::to_string(value) });
		}
		static void PrintError(long value)
		{
			m_DebugMessages.push_back({ Time::GetSystemTime(), ErrorMessageColor, std::to_string(value) });
		}
		static void PrintError(float value)
		{
			m_DebugMessages.push_back({ Time::GetSystemTime(), ErrorMessageColor, std::to_string(value) });
		}
		static void PrintError(double value)
		{
			m_DebugMessages.push_back({ Time::GetSystemTime(), ErrorMessageColor, std::to_string(value) });
		}

		static std::vector<std::tuple<std::string, glm::vec3, std::string>>& GetMessages() { return m_DebugMessages; }
	private:
		static std::vector<std::tuple<std::string, glm::vec3, std::string>> m_DebugMessages;
	};

}