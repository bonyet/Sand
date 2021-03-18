#pragma once

#include "EditorPanel.h"
#include "Sand/Renderer/Texture.h"
#include <array>

#include <spdlog/sinks/base_sink.h>

namespace Sand
{
	 
	class ConsolePanel : public EditorPanel, public spdlog::sinks::base_sink<std::mutex>
	{
	public:
		ConsolePanel();

		virtual void OnGuiRender() override;
	protected:
		virtual void sink_it_(const spdlog::details::log_msg& msg) override;
		virtual void flush_() override;
	};

}