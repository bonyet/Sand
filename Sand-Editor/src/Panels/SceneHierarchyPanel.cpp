#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "Sand/Scene/Components.h"

namespace Sand
{


	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnGuiRender()
	{
		// hierarchy
		if (!m_ShowWindow)
			return;

		ImGui::Begin("Scene Hierarchy", &m_ShowWindow);

		m_Context->m_Registry.each([&](auto entityID)
		{
			Actor entity{ entityID, m_Context.get() };
			DrawActorNode(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered() && ImGui::GetIO().WantCaptureMouse) {
			// clicked in empty space in panel
			m_SelectionContext = {};
		}

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			DrawActorCreationMenu();

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawActorNode(Actor entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Name;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		
		if (ImGui::IsItemClicked())
			m_SelectionContext = entity;

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			// Display children of entity
			ImGui::TextWrapped("Anyways uhm... I bought a whole bunch of shungite, rocks, do you know what shungite is? Anybody know what shungite is. No, no Suge Knight, I think hes locked up in prison. Talking shungite. Anyways, its a 2 billion year old like rock, stone that protects against frequencies and unwanted frequencies that may be traveling in the air. So thats my story. I bought a whole bunch of stuff, put them around the la casa. Little pyramids. Stuff like that.");

			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyActor(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	void SceneHierarchyPanel::DrawActorCreationMenu()
	{
		if (ImGui::MenuItem("Create Empty Actor")) 
		{
			auto& entity = m_Context->CreateActor("Actor");

			m_SelectionContext = entity;
		}
		else if (ImGui::MenuItem("Create Square")) 
		{
			auto& entity = m_Context->CreateActor("Actor");
			entity.AddComponent<SpriteRendererComponent>();

			m_SelectionContext = entity;
		}
	}
}