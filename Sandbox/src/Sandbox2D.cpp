#include "Sandbox2D.h"

#include "Sand/Utils/Gradient.h"
#include "Sand/ImGui/ImGuiColorGradient.h"
#include "Sand/ParticleSystems/ParticleEmitter.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace Sand;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
{
}

void Sandbox2D::OnAttach()
{
	SAND_PROFILE_FUNCTION();

	RecalculateProjection(1280.0f, 720.0f);
}

void Sandbox2D::OnDetach()
{
}

static glm::vec2 ScreenToWorldPoint(glm::vec2& screenPoint, const glm::mat4& projection, const glm::mat4& view)
{
	Window& window = Application::Get().GetWindow();
	float width = (float)window.GetWidth();
	float height = (float)window.GetHeight();

	glm::vec4 v4Point = { screenPoint, 1.0f, 1.0f };

	glm::mat4 invProjView = glm::inverse(projection * view);

	// mouse NDC
	v4Point.x = (2.0f * ((float)(v4Point.x) / (width))) - 1.0f;
	v4Point.y = 1.0f - (2.0f * ((float)(v4Point.y) / (height)));

	v4Point = v4Point * invProjView;
	v4Point /= v4Point.w;

	return v4Point;
}

void Sandbox2D::OnUpdate(Timestep ts)
{
	SAND_PROFILE_FUNCTION();
	
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position, 0.0f));

	Renderer2D::ResetStats();

	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	RenderCommand::Clear();

	glm::vec2 worldSpaceMouse = ScreenToWorldPoint(Input::GetMousePosition(), m_Projection, view);
	Renderer2D::GetShader()->SetFloat3("u_LightPosition", { worldSpaceMouse, 0.0f });

	Renderer2D::Begin(m_Projection, view);

	for (float y = 0.0f; y < 10.0f; y++)
	{
		for (float x = 0.0f; x < 10.0f; x++)
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), { x, y, 0.0f }) *
				glm::scale(glm::mat4(1.0f), { 0.8f, 0.8f, 0.0f });

			Renderer2D::DrawQuad(transform, { 1.0f, 1.0f, 1.0f, 1.0f });
		}
	}

	Renderer2D::End();

	float speedMultiplier = Input::IsKeyPressed(Keycode::LeftShift) ? 25.0f : 10.0f;

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

	ImGui::Begin("Lighting");

	static glm::vec3 lightColor{ 1.0f };
	if (ImGui::SliderFloat3("Color", &lightColor.x, 0.0f, 1.0f, "%.2f"))
	{
		Renderer2D::GetShader()->SetFloat3("u_LightColor", lightColor);
	}
	static float intensity = 1.0f;
	if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 25.0f, "%.2f"))
	{
		Renderer2D::GetShader()->SetFloat("u_LightIntensity", intensity);
	}

	ImGui::End();

	ImGui::Begin("Stats");
	
	ImGui::Text("%.2fms (%.2ffps)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	
	Renderer2D::Statistics stats = Renderer2D::GetStats();
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	static bool vsync = true;
	if (ImGui::Checkbox("Vertical sync", &vsync))
	{
		Application::Get().GetWindow().SetVSync(vsync);
	}

	if (ImGui::SliderFloat("Ortho Size", &m_OrthographicSize, 1.0f, 450.0f, "%.2f"))
	{
		const Window& wnd = Application::Get().GetWindow();
		RecalculateProjection((float)wnd.GetWidth(), (float)wnd.GetHeight());
	}
	
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
	if (m_OrthographicSize >= 450.0f)
		m_OrthographicSize = 450.0f;
	
	const Window& wnd = Application::Get().GetWindow();
	RecalculateProjection((float)wnd.GetWidth(), (float)wnd.GetHeight());

	return true;
}

void Sandbox2D::RecalculateProjection(float width, float height)
{
	float aspectRatio = width / height;

	float orthoLeft = m_OrthographicSize * aspectRatio * 0.5f;
	float orthoRight = -m_OrthographicSize * aspectRatio * 0.5f;
	float orthoBottom = -m_OrthographicSize * 0.5f;
	float orthoTop = m_OrthographicSize * 0.5f;

	m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, -1.0f, 1.0f);
}
