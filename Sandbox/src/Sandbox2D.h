#pragma once

#include "Sand.h"
#include "Sand/Events/ApplicationEvent.h"

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
	bool OnWindowResize(Sand::WindowResizeEvent& e);
	bool OnMouseScrolled(Sand::MouseScrolledEvent& e);

	void RecalculateProjection(float width, float height);
private:
	// Camera stuff
	glm::mat4 m_Projection = glm::mat4(1.0f);
	glm::vec2 m_Position = glm::vec2(0.0f);
	float m_OrthographicSize = 50.0f;
};