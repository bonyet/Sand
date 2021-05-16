#include "SceneLayoutPanel.h"
#include "Sand/Scene/Components.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Sand
{

	static Actor s_CurrentDragDropTarget = {};

	SceneLayoutPanel::SceneLayoutPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneLayoutPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
		m_SelectionContext = {};
	}

	void SceneLayoutPanel::OnGuiRender()
	{
		if (!m_ShowWindow)
			return;

		ImGui::Begin("Scene Layout", &m_ShowWindow, ImGuiWindowFlags_HorizontalScrollbar);
		
		// Render all actors
		ImGui::SetWindowFontScale(0.9f);
		m_Context->m_CurrentRegistry->each([&](auto entityID)
		{
			Actor entity{ entityID, m_Context.get() };

			if (!entity.GetComponent<TransformComponent>().HasParent())
				DrawActorNode(entity); // The drawing of children nodes is handled in DrawActorNode
		});
		ImGui::SetWindowFontScale(1.0f);

		// Add Actor button
		{
			const float frameSizeY = 35.0f;
			ImGui::SetCursorPosY((ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y) - frameSizeY);
			ImGui::BeginChild("SceneLayoutPanel_ChildAddActorButton", { ImGui::GetWindowContentRegionWidth(), frameSizeY }, true);

			ImGui::PushStyleColor(ImGuiCol_Button, { 0.30f, 0.30f, 0.30f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.35f, 0.35f, 0.35f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.25f, 0.25f, 0.25f, 1.0f });

			const float buttonWidth = 15.0f; // Not actually 15 pixels wide, it's weird
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + buttonWidth);
			if (ImGui::Button("Create Actor", ImVec2(ImGui::GetContentRegionMax().x - buttonWidth * 2.0f, 25.0f)))
			{
				m_SelectionContext = m_Context->CreateActor("Actor");
			}
			ImGui::PopStyleColor(3);

			ImGui::EndChild();
		}

		// Tooltip for drag and drop actors
		if (m_SelectionContext && ImGui::IsMouseDown(0) && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)) 
		{
			// We don't call Begin/End tooltip in this scope because it messes up the tooltip if the else-if block doesn't get executed

			// Tooltips
			if (s_CurrentDragDropTarget)
			{
				ImGui::BeginTooltipEx(0, ImGuiTooltipFlags_OverridePreviousTooltip);

				bool invalidParentAction = m_SelectionContext.GetComponent<TransformComponent>().HasChild(s_CurrentDragDropTarget);
				ImVec4 textColor = invalidParentAction ? ImVec4(0.8f, 0.3f, 0.3f, 1.0f) : ImVec4(0.2f, 0.8f, 0.3f, 1.0f);

				ImGui::PushStyleColor(ImGuiCol_Text, textColor);
				ImGui::Text(invalidParentAction ? "Invalid action" : "Parenting '%s' to '%s'", m_SelectionContext.GetComponent<TagComponent>().Name.c_str(),
					s_CurrentDragDropTarget.GetComponent<TagComponent>().Name.c_str());
				ImGui::PopStyleColor();
			
				ImGui::EndTooltip();
			}
			else if (!s_CurrentDragDropTarget && m_SelectionContext.GetComponent<TransformComponent>().HasParent())
			{
				ImGui::BeginTooltipEx(0, ImGuiTooltipFlags_OverridePreviousTooltip);

				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.3f, 0.3f, 1.0f));
				ImGui::Text("Orphaning '%s'", m_SelectionContext.GetComponent<TagComponent>().Name.c_str());
				ImGui::PopStyleColor();

				ImGui::EndTooltip();
			}

			s_CurrentDragDropTarget = {};
		}

		// Handle drag and drop
		if (ImGui::BeginDragDropTargetCustom(ImGui::GetCurrentWindow()->WorkRect, (ImGuiID)"SLP_DND"))
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_ACTOR_SLP"))
			{
				entt::entity fromID = static_cast<entt::entity>(*static_cast<uint32_t*>(payload->Data)); // The Ultimate Cast
				Actor(fromID, Scene::GetActiveScene()).GetComponent<TransformComponent>().SetParent({});

				ImGui::ClearDragDrop();
			}
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered() && ImGui::GetIO().WantCaptureMouse) {
			// Clicked in empty space in panel
			m_SelectionContext = {};
		}

		// Create actors
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			DrawActorCreationMenu();

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void SceneLayoutPanel::DrawActorNode(Actor actor)
	{
		auto& tag = actor.GetComponent<TagComponent>().Name;

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

		ImVec4 textColor = m_SelectionContext == actor ? ImVec4(0.2f, 0.8f, 0.3f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, textColor);

		ImGuiTreeNodeFlags flags = (m_SelectionContext == actor ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed;
		if (!actor.GetComponent<TransformComponent>().HasChildren())
			flags |= ImGuiTreeNodeFlags_Leaf;

		bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<uint32_t>(actor)), flags, tag.c_str());

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoPreviewTooltip))
		{
			ImGui::SetDragDropPayload("DND_ACTOR_SLP", reinterpret_cast<uint32_t*>(&actor), sizeof(uint32_t));

			ImGui::EndDragDropSource();
		}
		
		if (ImGui::BeginDragDropTarget())
		{
			s_CurrentDragDropTarget = actor;

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_ACTOR_SLP")) 
			{
				auto fromID = static_cast<entt::entity>(*static_cast<uint32_t*>(payload->Data));

				Actor child = { fromID, Scene::GetActiveScene() };
				auto& transform = child.GetComponent<TransformComponent>();
				
				if (!transform.HasChild(actor))
					transform.SetParent(actor);
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::PopStyleColor();
		ImGui::PopStyleVar(1);

		if (ImGui::IsItemClicked())
			m_SelectionContext = actor;

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
			TransformComponent& transform = actor.GetComponent<TransformComponent>();
			for (auto& child : transform.GetChildren())
			{
				DrawActorNode(child);
			}

			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyActor(actor);
			if (m_SelectionContext == actor)
				m_SelectionContext = {};
		}
	}

	void SceneLayoutPanel::DrawActorCreationMenu()
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