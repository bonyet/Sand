#include "Sandbox2D.h"
#include <imgui/imgui.h>

using namespace Sand;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
{
}

void Sandbox2D::OnAttach()
{
	SAND_PROFILE_FUNCTION();
	RenderCommand::Init();
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Timestep ts)
{
	SAND_PROFILE_FUNCTION();

	RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.8f, 1.0f });
	RenderCommand::Clear();
}

void Sandbox2D::OnGuiRender()
{
	SAND_PROFILE_FUNCTION();

	ImGui::Begin("hi");

	ImGui::End();
}

void Sandbox2D::OnEvent(Event& e)
{
}