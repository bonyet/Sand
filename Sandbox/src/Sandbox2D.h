#pragma once

#include "Sand.h"
#include "Sand/Events/ApplicationEvent.h"

#include <glm/glm.hpp>

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

	void GetInput(Sand::Timestep ts);
private:
	glm::mat4 m_CameraProjection{ glm::mat4(1.0f) };
	glm::vec3 m_CameraPos{ 0.0f, 0.0f, 10.0f };
	float m_CameraMoveSpeed = 5.0f;
};