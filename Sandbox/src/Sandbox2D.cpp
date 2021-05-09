#include "Sandbox2D.h"
#include <imgui/imgui.h>

using namespace Sand;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
{
}

static Ref<Texture2D> checkerboardTexture = nullptr;

void Sandbox2D::OnAttach()
{
	SAND_PROFILE_FUNCTION();

	RecalculateProjection(1280.0f, 720.0f);

	checkerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Timestep ts)
{
	SAND_PROFILE_FUNCTION();

	Renderer2D::ResetStats();

	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	RenderCommand::Clear();

	glm::mat4 view = glm::translate(glm::mat4(1.0f), { m_Position.x, m_Position.y, 0.0f });

	Renderer2D::Begin(m_Projection, view);

	for (float y = 0; y < 200.0f; y += 1.0f)
	{
		for (float x = 0; x < 200.0f; x += 1.0f)
		{
			Renderer2D::DrawQuad({ x, y }, { 0.8f, 0.8f }, checkerboardTexture, 1.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
		}
	}

	Renderer2D::End();

	float speedMultiplier = Input::IsKeyPressed(Keycode::LeftShift) ? 50.0f : 8.0f;

	if (Input::IsKeyPressed(Keycode::W))
		m_Position.y += ts * speedMultiplier;
	if (Input::IsKeyPressed(Keycode::A))
		m_Position.x += ts * speedMultiplier;
	if (Input::IsKeyPressed(Keycode::S))
		m_Position.y -= ts * speedMultiplier;
	if (Input::IsKeyPressed(Keycode::D))
		m_Position.x -= ts * speedMultiplier;
}

void Sandbox2D::OnGuiRender()
{
	SAND_PROFILE_FUNCTION();

	ImGui::Begin("Stats");
	
	ImGui::Text("%.2fms (%.2ffps)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	Renderer2D::Statistics stats = Renderer2D::GetStats();
	ImGui::Text("Draw calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::End();
}

void Sandbox2D::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);

	dispatcher.Dispatch<WindowResizeEvent>(SAND_BIND_EVENT_FN(Sandbox2D::OnWindowResize));
	dispatcher.Dispatch<MouseScrolledEvent>(SAND_BIND_EVENT_FN(Sandbox2D::OnMouseScrolled));
}

bool Sandbox2D::OnWindowResize(WindowResizeEvent& e)
{
	RecalculateProjection((float)e.GetWidth(), (float)e.GetHeight());

	return true;
}

bool Sandbox2D::OnMouseScrolled(Sand::MouseScrolledEvent& e)
{
	m_OrthographicSize -= e.GetYOffset();

	if (m_OrthographicSize <= 1.0f)
		m_OrthographicSize = 1.0f;
	if (m_OrthographicSize >= 250.0f)
		m_OrthographicSize = 250.0f;
	
	const Window& wnd = Application::Get().GetWindow();
	RecalculateProjection(wnd.GetWidth(), wnd.GetHeight());

	return true;
}

void Sandbox2D::RecalculateProjection(float width, float height)
{
	float aspectRatio = width / height;

	float orthoLeft = m_OrthographicSize * aspectRatio * 0.5f;
	float orthoRight = -m_OrthographicSize * aspectRatio * 0.5f;
	float orthoBottom = -m_OrthographicSize * 0.5f;
	float orthoTop = m_OrthographicSize * 0.5f;

	//SAND_CORE_TRACE("L = {0}, R = {1}, B = {2}, T = {3}", orthoLeft, orthoRight, orthoBottom, orthoTop);
	m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, -1.0f, 1.0f);
}
