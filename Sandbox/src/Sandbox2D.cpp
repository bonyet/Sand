#include "Sandbox2D.h"

using namespace Sand;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
{
}

void Sandbox2D::OnAttach()
{
	SAND_PROFILE_FUNCTION();
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Sand::Timestep ts)
{
	SAND_PROFILE_FUNCTION();
}

void Sandbox2D::OnGuiRender()
{
	SAND_PROFILE_FUNCTION();

	ImGui::Begin("Ok");
	ImGui::Text("%.3f (%.1ffps)", 1000 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void Sandbox2D::OnEvent(Sand::Event& e)
{
}