#pragma once

#include "Sand.h"
#include <imgui/imgui.h>

class Sandbox2D : public Sand::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Sand::Timestep ts) override;
	virtual void OnGuiRender() override;
	virtual void OnEvent(Sand::Event& e) override;
private:
};