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

	m_CameraProjection = glm::perspective(glm::radians(45.0f), (float)16 / 9, 0.1f, 1000.0f);
}


void Sandbox2D::OnDetach()
{
}

void Sandbox2D::GetInput(Timestep ts)
{
	if (Input::IsKeyPressed(Keycode::W)) {
		m_CameraPos.y += m_CameraMoveSpeed * ts;
	}
	if (Input::IsKeyPressed(Keycode::A)) {
		m_CameraPos.x -= m_CameraMoveSpeed * ts;
	}
	if (Input::IsKeyPressed(Keycode::S)) {
		m_CameraPos.y -= m_CameraMoveSpeed * ts;
	}
	if (Input::IsKeyPressed(Keycode::D)) {
		m_CameraPos.x += m_CameraMoveSpeed * ts;
	}

	if (Input::WasKeyPressed(Keycode::Escape))
		Application::Get().Close();
}

void Sandbox2D::OnUpdate(Sand::Timestep ts)
{
	SAND_PROFILE_FUNCTION();

	GetInput(ts);

	glm::mat4 camTransform = glm::translate(glm::mat4(1.0f), m_CameraPos) *
		glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f });

	Renderer2D::ResetStats();
	
	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	RenderCommand::Clear();

	//Renderer2D::BeginScene(m_CameraProjection, camTransform);
	//
	//Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	//
	//Renderer2D::EndScene();

	if (Input::IsKeyPressed(Keycode::C)) {
		Renderer::BeginScene(m_CameraProjection, camTransform);

		Renderer::DrawCube({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

		Renderer::EndScene();
	}
}

void Sandbox2D::OnGuiRender()
{
	SAND_PROFILE_FUNCTION();

	ImGui::Begin("Soggy Log");

	Renderer2D::Statistics stats = Renderer2D::GetStats();
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Spacing();
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Separator();

	ImGui::Text("%.3fms (%.1ffps)", 1000 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
}

void Sandbox2D::OnEvent(Sand::Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResizeEvent>(SAND_BIND_EVENT_FN(OnWindowResize));
	dispatcher.Dispatch<MouseScrolledEvent>(SAND_BIND_EVENT_FN(OnMouseScrolled));
}

bool Sandbox2D::OnMouseScrolled(MouseScrolledEvent& e)
{
	m_CameraPos.z -= e.GetYOffset();
	m_CameraMoveSpeed = m_CameraPos.z / 2;

	return false;
}

bool Sandbox2D::OnWindowResize(WindowResizeEvent& e)
{
	m_CameraProjection = glm::perspective(glm::radians(45.0f), (float)e.GetWidth() / e.GetHeight(), 0.1f, 1000.0f);

	return false;
}