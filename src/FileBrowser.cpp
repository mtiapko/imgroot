#include <cstdint>
#include <iostream>
#include "imgroot/FileBrowser.h"
#include "imgui.h"

static_assert(std::is_same_v<ImU32, uint32_t>);

namespace ImGroot
{

ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) noexcept
{
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

void FileBrowser::unselect_item() noexcept
{
	m_selected_item_id = std::numeric_limits<uint32_t>::max();
}

void FileBrowser::draw_none_icon(float width, float height) const noexcept
{
	ImGui::Dummy({ width, height });
	ImGui::SameLine();
}

void FileBrowser::draw_dir_icon(float width, float height, uint32_t color) const noexcept
{
	const auto item_size = ImVec2 { width, height };
	this->draw_none_icon(width, height);

	height *= 0.9f;
	const auto icon_size = ImVec2 { item_size.x, height };

	// min       p0
	// *---------*
	// |          \            p2
	// |           *-----------*
	// |           p1          |
	// |                       |
	// |                       |
	// |                       |
	// |                       |
	// *-----------------------*
	// min.x, max.y            max

	const auto icon_min = ImGui::GetItemRectMin() + ImVec2 { 0.0f, (item_size.y - height) * 0.5f };
	const auto icon_max = icon_min + icon_size;

	const auto p0 = ImVec2 { icon_min.x + width * 0.4f, icon_min.y };
	const auto p1 = ImVec2 { p0.x + width * 0.15f, icon_min.y + height * 0.25f };
	const auto p2 = ImVec2 { icon_max.x, p1.y };

	const auto points = {
		icon_min,
		p0,
		p1,
		p2,
		icon_max,
		{ icon_min.x, icon_max.y }
	};

	auto draw_list = ImGui::GetWindowDrawList();
	draw_list->AddPolyline(points.begin(), points.size(), color, true, 1.0f);
}

void FileBrowser::draw_file_icon(float width, float height, uint32_t color) const noexcept
{
	const auto item_size = ImVec2 { width, height };
	this->draw_none_icon(width, height);

	width *= 0.7f;
	const auto icon_size = ImVec2 { width, item_size.y };
	const auto triangle_size = ImVec2 { icon_size.x * 0.4f, icon_size.x * 0.4f };

	// min          p2
	// *------------*....
	// |            |\...
	// |            | \..
	// |            |  \.
	// |         p0 *---* p1
	// |                |
	// |                |
	// |                |
	// |                |
	// |                |
	// *----------------*
	// min.x, max.y     max

	const auto icon_min = ImGui::GetItemRectMin() + ImVec2 { (item_size.x - width) * 0.5f, 0.0f };
	const auto icon_max = icon_min + icon_size;

	const auto p0 = ImVec2 { icon_max.x - triangle_size.x, icon_min.y + triangle_size.y };
	const auto p1 = ImVec2 { p0.x + triangle_size.x, p0.y };
	const auto p2 = ImVec2 { p0.x, p0.y - triangle_size.y };

	auto draw_list = ImGui::GetWindowDrawList();
	draw_list->AddTriangleFilled(p0, p1, p2, color);

	const auto points = {
		p2,
		icon_min,
		{ icon_min.x, icon_max.y },
		icon_max,
		p1
	};

	draw_list->AddPolyline(points.begin(), points.size(), color, false, 1.0f);
}

FileBrowser::ButtonState FileBrowser::draw_wide_button(bool selected,
	uint32_t hovered_color, uint32_t active_color) const noexcept
{
	const auto& style = ImGui::GetStyle();
	auto cursor_pos = ImGui::GetCursorPos();
	ImGui::Dummy({ ImGui::GetContentRegionMax().x, ImGui::GetTextLineHeight() + style.FramePadding.y * 2.0f });

	const auto item_min = ImGui::GetItemRectMin();
	const auto item_max = ImGui::GetItemRectMax();

	cursor_pos.y += style.FramePadding.y;
	ImGui::SetCursorPos(cursor_pos);

	auto draw_list = ImGui::GetWindowDrawList();

	if (ImGui::IsItemHovered()) {
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			draw_list->AddRectFilled(item_min, item_max, active_color);
			return ButtonState::CLICKED;
		}

		draw_list->AddRectFilled(item_min, item_max, hovered_color);
		return ButtonState::HOVERED;
	}

	if (selected)
		draw_list->AddRectFilled(item_min, item_max, active_color);

	return ButtonState::NONE;
}

void FileBrowser::show_nav_menu() noexcept
{
	if (ImGui::ArrowButton("Back", ImGuiDir_Left)) {
		m_current_dir = m_current_dir.parent_path();
		this->unselect_item();
	}

	ImGui::SameLine();
	ImGui::TextUnformatted(m_current_dir.c_str());
}

void FileBrowser::show_side_menu() noexcept
{
	ImGui::Text("Bookmarks");
	ImGui::Text("* Home");
	ImGui::Text("* Desktop");
	ImGui::Text("* Recent");
	ImGui::TextDisabled("* Trash");
}

void FileBrowser::show_dir_browser_list() noexcept
{
	const auto icon_color = 0xFF999999;
	const auto icon_size = ImGui::GetTextLineHeight();

	auto& style = ImGui::GetStyle();
	const auto btn_hovered_color = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_FrameBgHovered]);
	const auto btn_active_color = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_FrameBgActive]);

	if (ImGui::BeginChild("Dir Browser", { 0.0f, -style.CellPadding.y })) {
		float cellPaddingY = style.CellPadding.y;
		style.CellPadding.y = 0.0f;

		if (ImGui::BeginTable("Dir Browser Table", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg)) {
			uint32_t id = 0;
			auto dir_iter = std::filesystem::directory_iterator(m_current_dir);
			for (auto& p: dir_iter) {
				ImGui::TableNextRow();

				const bool selected = (id == m_selected_item_id);
				const auto btn_state = this->draw_wide_button(selected, btn_hovered_color, btn_active_color);
				if (btn_state == ButtonState::CLICKED) {
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
						if (p.is_directory()) {
							m_current_dir = p.path();
							this->unselect_item();
						}
					} else {
						m_selected_item_id = id;
					}
				}

				if (p.is_regular_file()) this->draw_file_icon(icon_size, icon_size, icon_color);
				else if (p.is_directory()) this->draw_dir_icon(icon_size, icon_size, icon_color);
				else this->draw_none_icon(icon_size, icon_size);

				ImGui::TextUnformatted(p.path().filename().c_str());
				++id;
			}

			ImGui::EndTable();
		}

		style.CellPadding.y = cellPaddingY;
	}

	ImGui::EndChild();
}

void FileBrowser::render() noexcept
{
	ImGui::SetNextWindowSize({ 420.0f, 280.0f }, ImGuiCond_FirstUseEver);

	if (ImGui::Begin("ImGroot")) {
		this->show_nav_menu();

		if (ImGui::BeginTable("ImGroot Main", 2, ImGuiTableFlags_Resizable)) {
			ImGui::TableSetupColumn(nullptr, 0, 0.3f);

			ImGui::TableNextRow();
			this->show_side_menu();

			ImGui::TableSetColumnIndex(1);
			this->show_dir_browser_list();

			ImGui::EndTable();
		}
	}

	ImGui::End();
}

}
