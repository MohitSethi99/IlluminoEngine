#include <IlluminoEngine.h>
#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/glm/gtx/norm.hpp>

#include "Illumino/Core/Application.h"
#include "Illumino/Renderer/GraphicsContext.h"
#include "Window.h"

namespace IlluminoEngine
{
	static std::vector<Ref<Mesh>> s_Meshes;
	constexpr float EPSILON = 1.17549435E-38f;

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
		SetTheme();
	}

	void EditorLayer::OnAttach()
	{
		RenderTextureSpec spec;
		spec.Width = 1920;
		spec.Height = 1080;
		m_RenderTexture = RenderTexture::Create(spec);

		m_EditorCamera = CreateRef<EditorCamera>(glm::radians(45.0f), (float)spec.Width / (float)spec.Height, 0.001f, 1000.0f);

		// temp
		s_Meshes.push_back(CreateRef<Mesh>("Assets/Meshes/sponza/sponza.assbin"));
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		uint32_t width = m_ViewportSizeMax.x - m_ViewportSizeMin.x;
		uint32_t height = m_ViewportSizeMax.y - m_ViewportSizeMin.y;
		const RenderTextureSpec& spec = m_RenderTexture->GetSpecification();
		if (width != 0 && height != 0 && (width != spec.Width || height != spec.Height))
		{
			m_RenderTexture->Resize(width, height);
			m_EditorCamera->SetViewportSize(width, height);
		}

		m_MousePosition = *((glm::vec2*) &(ImGui::GetMousePos()));
		const glm::vec3& position = m_EditorCamera->GetPosition();
		float yaw = m_EditorCamera->GetYaw();
		float pitch = m_EditorCamera->GetPitch();

		bool moved = false;
		if (m_ViewportHovered && ImGui::IsMouseDown(ImGuiMouseButton_Right))
		{
			const glm::vec2 change = (m_MousePosition - m_LastMousePosition) * m_MouseSensitivity;
			yaw += change.x;
			pitch = glm::clamp(pitch - change.y, -89.9f, 89.9f);

			glm::vec3 moveDirection = glm::vec3(0.0f);
			if (ImGui::IsKeyDown(ImGuiKey_W))
			{
				moved = true;
				moveDirection += m_EditorCamera->GetForward() * ts.GetSeconds();
			}
			else if (ImGui::IsKeyDown(ImGuiKey_S))
			{
				moved = true;
				moveDirection -= m_EditorCamera->GetForward() * ts.GetSeconds();
			}
			if (ImGui::IsKeyDown(ImGuiKey_D))
			{
				moved = true;
				moveDirection += m_EditorCamera->GetRight() * ts.GetSeconds();
			}
			else if (ImGui::IsKeyDown(ImGuiKey_A))
			{
				moved = true;
				moveDirection -= m_EditorCamera->GetRight() * ts.GetSeconds();
			}

			if (glm::length2(moveDirection) > EPSILON)
				m_MoveDirection = glm::normalize(moveDirection);
		}

		m_MoveVelocity += (moved ? 1.0f : -1.0f) * ts;
		m_MoveVelocity *= glm::pow(m_MoveDampeningFactor, ts);
		if (m_MoveVelocity > 0.0f)
		{
			float maxMoveSpeed = m_MaxMoveSpeed * (ImGui::IsKeyDown(ImGuiKey_LeftShift) ? 3.0f : 1.0f);
			m_EditorCamera->SetPosition(position + (m_MoveDirection * m_MoveVelocity * maxMoveSpeed));
		}

		m_EditorCamera->SetYaw(yaw);
		m_EditorCamera->SetPitch(pitch);
		m_LastMousePosition = m_MousePosition;

		m_EditorCamera->OnUpdate(ts);
	}

	void EditorLayer::OnImGuiRender()
	{
		bool b = true;
		ImGui::ShowDemoWindow(&b);

		m_RenderTexture->Bind();
		{
			SceneRenderer::BeginScene(*m_EditorCamera);
			for (size_t i = 0; i < s_Meshes.size(); ++i)
			{
				auto& submeshes = s_Meshes[i]->GetSubmeshes();
				for (Submesh& submesh : submeshes)
					SceneRenderer::SubmitMesh(submesh, glm::mat4(1.0f));
			}
			SceneRenderer::EndScene();
		}
		m_RenderTexture->Unbind();

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//// Viewport ////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Viewport");
		m_ViewportSizeMin = ImGui::GetWindowContentRegionMin();
		m_ViewportSizeMax = ImGui::GetWindowContentRegionMax();
		uint32_t width = m_ViewportSizeMax.x - m_ViewportSizeMin.x;
		uint32_t height = m_ViewportSizeMax.y - m_ViewportSizeMin.y;

		m_ViewportHovered = ImGui::IsWindowHovered();

		uint64_t textureID = m_RenderTexture->GetRendererID();
		ImGui::Image((ImTextureID)textureID, { (float)width, (float)height });
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorLayer::SetTheme()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/opensans/OpenSans-Regular.ttf", 18.0f);
		io.Fonts->AddFontFromFileTTF("Assets/Fonts/opensans/OpenSans-Bold.ttf", 18.0f);
		io.Fonts->Build();
		
		ImVec4* colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_Text] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.16f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.17f, 0.18f, 0.20f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.22f, 0.24f, 0.25f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.16f, 0.17f, 0.18f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.16f, 0.17f, 0.18f, 1.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.14f, 0.15f, 0.16f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.84f, 0.34f, 0.17f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.59f, 0.24f, 0.12f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.13f, 0.14f, 0.16f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.14f, 0.16f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.13f, 0.14f, 0.16f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.13f, 0.14f, 0.16f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.13f, 0.14f, 0.16f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.75f, 0.30f, 0.15f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
		colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.19f, 0.20f, 0.22f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.84f, 0.34f, 0.17f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.59f, 0.24f, 0.12f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.22f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.84f, 0.34f, 0.17f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.59f, 0.24f, 0.12f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.2f, 0.21f, 0.23f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.30f, 0.15f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.59f, 0.24f, 0.12f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.84f, 0.34f, 0.17f, 0.14f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.84f, 0.34f, 0.17f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.59f, 0.24f, 0.12f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.84f, 0.34f, 0.17f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.68f, 0.28f, 0.14f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.13f, 0.14f, 0.16f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.17f, 0.18f, 0.20f, 1.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.19f, 0.20f, 0.22f, 1.00f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.75f, 0.30f, 0.15f, 1.00f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.75f, 0.30f, 0.15f, 1.00f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.75f, 0.30f, 0.15f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

		ImGuiStyle* style = &ImGui::GetStyle();

		style->WindowMenuButtonPosition = 0;

		style->WindowPadding = ImVec2(4.0f, 3.0f);
		style->FramePadding = ImVec2(4.0f, 4.0f);
		style->ItemSpacing = ImVec2(6.0f, 3.0f);
		style->IndentSpacing = 12;
		style->ScrollbarSize = 12;
		style->GrabMinSize = 9;

		style->WindowBorderSize = 1.0f;
		style->ChildBorderSize = 1.0f;
		style->PopupBorderSize = 3.0f;
		style->FrameBorderSize = 1.0f;
		style->TabBorderSize = 1.0f;
		style->WindowRounding = 6.0f;
		style->ChildRounding = 0.0f;
		style->FrameRounding = 4.0f;
		style->PopupRounding = 4.0f;
		style->ScrollbarRounding = 12.0f;
		style->GrabRounding = 2.0f;
		style->TabRounding = 6.0f;
	}
}
