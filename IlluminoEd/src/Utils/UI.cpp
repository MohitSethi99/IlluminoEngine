#include "UI.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "EditorTheme.h"

namespace IlluminoEngine
{
	static int s_UIContextID = 0;
	static uint32_t s_Counter = 0;
	static char s_IDBuffer[16];
	static Ref<Texture2D> s_BlackTexture;

	void UI::Init()
	{
		glm::vec4 blackColor = glm::vec4(0.0f);
		s_BlackTexture = Texture2D::Create(1, 1, glm::value_ptr(blackColor));
	}

	void UI::Shutdown()
	{
		s_BlackTexture = nullptr;
	}

	void UI::PushID()
	{
		ImGui::PushID(s_UIContextID++);
		s_Counter = 0;
	}

	void UI::PopID()
	{
		ImGui::PopID();
		s_UIContextID--;
	}
	
	void UI::BeginPropertyGrid(const char* label, bool rightAlignNextColumn)
	{
		PushID();
		ImGui::TableNextRow();
        ImGui::TableNextColumn();

		ImGui::PushID(label);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y * 0.5f);
		ImGui::Text(label);
		ImGui::TableNextColumn();

		if (rightAlignNextColumn)
			ImGui::SetNextItemWidth(-FLT_MIN);
	}
	
	void UI::EndPropertyGrid()
	{
		ImGui::PopID();
		PopID();
	}

	void UI::BeginProperties()
	{
		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);

		ImGuiTableFlags flags = ImGuiTableFlags_Resizable
			| ImGuiTableFlags_BordersInner
			| ImGuiTableFlags_BordersOuterH
			| ImGuiTableFlags_PadOuterX
			| ImGuiTableFlags_SizingStretchSame;

		ImVec2 cellPadding = ImGui::GetStyle().CellPadding;
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { cellPadding.x * 4, cellPadding.y });
		ImGui::BeginTable(s_IDBuffer, 2, flags);
	}

	void UI::EndProperties()
	{
		ImGui::EndTable();
		ImGui::PopStyleVar();
	}

	bool UI::Property(const char* label, eastl::string& value)
	{
		bool modified = false;

		BeginPropertyGrid(label);

		char buffer[256];
		strcpy(buffer, value.c_str());

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::InputText(s_IDBuffer, buffer, 256))
		{
			value = buffer;
			modified = true;
		}

		EndPropertyGrid();

		return modified;
	}

	void UI::Property(const char* label, const char* value)
	{
		BeginPropertyGrid(label);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		ImGui::InputText(s_IDBuffer, (char*)value, 256, ImGuiInputTextFlags_ReadOnly);

		EndPropertyGrid();
	}

	bool UI::Property(const char* label, int32_t& value)
	{
		bool modified = false;

		BeginPropertyGrid(label);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragInt(s_IDBuffer, &value))
			modified = true;

		EndPropertyGrid();

		return modified;
	}

	bool UI::Property(const char* label, uint32_t& value)
	{
		bool modified = false;

		BeginPropertyGrid(label);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragInt(s_IDBuffer, (int*)&value))
			modified = true;

		EndPropertyGrid();

		return modified;
	}

	bool UI::Property(const char* label, int32_t& value, int32_t min, int32_t max)
	{
		bool modified = false;

		BeginPropertyGrid(label);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::SliderInt(s_IDBuffer, &value, min, max))
			modified = true;

		EndPropertyGrid();

		return modified;
	}

	bool UI::Property(const char* label, uint32_t& value, uint32_t min, uint32_t max)
	{
		bool modified = false;

		BeginPropertyGrid(label);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::SliderInt(s_IDBuffer, (int*)&value, min, max))
			modified = true;

		EndPropertyGrid();

		return modified;
	}

	bool UI::Property(const char* label, float& value, float delta)
	{
		bool modified = false;

		BeginPropertyGrid(label);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat(s_IDBuffer, &value, delta))
			modified = true;

		EndPropertyGrid();

		return modified;
	}

	bool UI::Property(const char* label, float& value, float min, float max, const char* fmt)
	{
		bool modified = false;

		BeginPropertyGrid(label);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::SliderFloat(s_IDBuffer, &value, min, max, fmt))
			modified = true;

		EndPropertyGrid();

		return modified;
	}

	bool UI::Property(const char* label, float& value, float delta, float min, float max, const char* fmt)
	{
		bool modified = false;

		BeginPropertyGrid(label);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat(s_IDBuffer, &value, delta, min, max, fmt))
			modified = true;

		EndPropertyGrid();

		return modified;
	}

	bool UI::Property(const char* label, glm::vec2& value, float delta)
	{
		bool modified = false;

		BeginPropertyGrid(label);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat2(s_IDBuffer, glm::value_ptr(value), delta))
			modified = true;

		EndPropertyGrid();

		return modified;
	}

	bool UI::Property(const char* label, glm::vec3& value, float delta)
	{
		bool modified = false;

		BeginPropertyGrid(label);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat3(s_IDBuffer, glm::value_ptr(value), delta))
			modified = true;

		EndPropertyGrid();

		return modified;
	}

	bool UI::Property(const char* label, glm::vec4& value, float delta)
	{
		bool modified = false;

		BeginPropertyGrid(label);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat4(s_IDBuffer, glm::value_ptr(value), delta))
			modified = true;

		EndPropertyGrid();

		return modified;
	}

	bool UI::Property(const char* label, bool& flag)
	{
		bool modified = false;

		BeginPropertyGrid(label);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::Checkbox(s_IDBuffer, &flag))
			modified = true;

		EndPropertyGrid();

		return modified;
	}

	bool UI::Property(const char* label, int& value, const char** dropdownStrings, size_t count)
	{
		bool modified = false;

		const char* current = dropdownStrings[(int)value];
		
		BeginPropertyGrid(label);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);

		if(ImGui::BeginCombo(s_IDBuffer, current))
		{
			for (int i = 0; i < count; i++)
			{
				bool isSelected = current == dropdownStrings[i];
				if(ImGui::Selectable(dropdownStrings[i], isSelected))
				{
					current = dropdownStrings[i];
					value = i;
					modified = true;
				}

				if(isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		UI::EndPropertyGrid();

		return modified;
	}

	bool UI::PropertyColor3(const char* label, glm::vec3& color)
	{
		bool modified = false;

		BeginPropertyGrid(label);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::ColorEdit3(s_IDBuffer, glm::value_ptr(color)))
			modified = true;

		EndPropertyGrid();

		return modified;
	}

	bool UI::PropertyColor4(const char* label, glm::vec4& color)
	{
		bool modified = false;

		BeginPropertyGrid(label);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::ColorEdit4(s_IDBuffer, glm::value_ptr(color)))
			modified = true;

		EndPropertyGrid();

		return modified;
	}

	bool UI::PropertyColor4as3(const char* label, glm::vec4& color)
	{
		bool modified = false;

		BeginPropertyGrid(label);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		_itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::ColorEdit3(s_IDBuffer, glm::value_ptr(color)))
			modified = true;

		EndPropertyGrid();

		return modified;
	}

	bool UI::Property(const char* label, Ref<Texture2D>& texture, uint64_t overrideTextureID)
	{
		bool changed = false;

		const ImVec2 buttonSize = { 80, 80 };
		const ImVec2 xButtonSize = { buttonSize.x / 4.0f, 80 };
		
		BeginPropertyGrid(label, buttonSize.y);

		ImGui::SetCursorPos({ ImGui::GetContentRegionMax().x - buttonSize.x - xButtonSize.x, ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.25f, 0.25f, 0.25f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.35f, 0.35f, 0.35f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.25f, 0.25f, 0.25f, 1.0f });
		
		ImTextureID texId = (ImTextureID) overrideTextureID;
		if (overrideTextureID == 0)
			texId = (ImTextureID) (texture == nullptr ? s_BlackTexture->GetRendererID() : texture->GetRendererID());
		ImGui::ImageButton(texId, buttonSize, { 0, 0 }, { 1, 1 }, 0);
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const char* path = (const char*)payload->Data;
				texture = Texture2D::Create(path);
				changed = true;
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
		if(ImGui::Button("x", xButtonSize ))
		{
			texture = nullptr;
			changed = true;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();

		EndPropertyGrid();

		return changed;
	}

	void UI::DrawVec3Control(const char* label, glm::vec3& values, float resetValue, float columnWidth)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[1];

		BeginPropertyGrid(label, false);

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImVec2 innerItemSpacing = ImGui::GetStyle().ItemInnerSpacing;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, innerItemSpacing);

		// X
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", buttonSize))
				values.x = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::PopStyleVar();
		}

		ImGui::SameLine();

		// Y
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Y", buttonSize))
				values.y = resetValue;
			ImGui::PopFont();

			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::PopStyleVar();
		}

		ImGui::SameLine();

		// Z
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Z", buttonSize))
				values.z = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::PopStyleVar();
		}
		
		ImGui::PopStyleVar();
		EndPropertyGrid();
	}

	void UI::DrawRowsBackground(int row_count, float line_height, float x1, float x2, float y_offset, uint32_t col_even, uint32_t col_odd)
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		float y0 = ImGui::GetCursorScreenPos().y + (float)(int)y_offset;

		int row_display_start;
		int row_display_end;
		ImGui::CalcListClipping(row_count, line_height, &row_display_start, &row_display_end);
		for (int row_n = row_display_start; row_n < row_display_end; row_n++)
		{
			ImU32 col = (row_n & 1) ? col_odd : col_even;
			if ((col & IM_COL32_A_MASK) == 0)
				continue;
			float y1 = y0 + (line_height * row_n);
			float y2 = y1 + line_height;
			draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), col);
		}
	}
}
