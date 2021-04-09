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
}

void Sandbox2D::OnDetach()
{
}

static float moveSpeed = 10.0f;
static glm::vec3 worldSpace = glm::vec3(1.0f);

void Sandbox2D::OnUpdate(Timestep ts)
{
	SAND_PROFILE_FUNCTION();

	RenderCommand::SetClearColor({ 0.6f, 0.4f, 0.5f, 1.0f });
	RenderCommand::Clear();

	glm::mat4 view = glm::translate(glm::mat4(1.0f), { m_Position.x, m_Position.y, 0.0f });

	glm::vec2 mousePosition = Input::GetMousePosition();

	glm::mat4 quadTransform = glm::translate(glm::mat4(1.0f), worldSpace);

	Renderer2D::Begin(m_Projection, view);

	Renderer2D::DrawQuad(quadTransform, { 0.2f, 0.8f, 0.3f, 1.0f }, 0);

	Renderer2D::End();

	// Input
	if (Input::IsKeyPressed(Keycode::A))
		m_Position.x += moveSpeed * ts;
	if (Input::IsKeyPressed(Keycode::D))
		m_Position.x -= moveSpeed * ts;
	if (Input::IsKeyPressed(Keycode::W))
		m_Position.y += moveSpeed * ts;
	if (Input::IsKeyPressed(Keycode::S))
		m_Position.y -= moveSpeed * ts;
}

void Sandbox2D::OnGuiRender()
{
	SAND_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	if (ImGui::DragFloat("ortho size", &m_OrthographicSize, 0.1f, 1.0f, 1000.0f)) {
		const Window& wnd = Application::Get().GetWindow();
		RecalculateProjection((float)wnd.GetWidth(), (float)wnd.GetHeight());
	}
	ImGui::DragFloat("speed", &moveSpeed, 0.5f, 1.0f, 100.0f);

	ImGui::Spacing();
	ImGui::Text("%f, %f, %f", worldSpace.x, worldSpace.y, worldSpace.z);

	ImGui::End();
}

void Sandbox2D::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResizeEvent>(SAND_BIND_EVENT_FN(Sandbox2D::OnWindowResize));
}

bool Sandbox2D::OnWindowResize(Sand::WindowResizeEvent& e)
{
	RecalculateProjection((float)e.GetWidth(), (float)e.GetHeight());

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
