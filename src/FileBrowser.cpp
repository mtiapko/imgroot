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

void FileBrowser::draw_home_icon(float width, float height, uint32_t color) const noexcept
{
	this->draw_none_icon(width, height);

	// min .....* p0       //
	//     .   / \         //
	//     .  /   \        //
	//  p3 . *     *   p1  //
	//     ./|p5   |\.     //
	//  p4 * | *-+ | * p2  //
	//       | | | | .     //
	//       *-+-*-*.. max //
	//     p6   p7 p8      //

	const auto item_size = ImVec2 { width, height };

	const auto icon_min = ImGui::GetItemRectMin();
	const auto icon_max = icon_min + item_size;

	const auto p0 = ImVec2 { icon_min.x + width * 0.5f, icon_min.y };

	const auto p1 = ImVec2 { p0.x + width * 0.3f, p0.y + height * 0.3f };
	const auto p2 = ImVec2 { icon_max.x, p1.y + height * 0.2f };

	const auto p3 = ImVec2 { p0.x - width * 0.3f, p0.y + height * 0.3f };
	const auto p4 = ImVec2 { icon_min.x, p3.y + height * 0.2f };

	const auto p5 = ImVec2 { p0.x - width * 0.1f, p4.y + height * 0.1f };

	const auto p6 = ImVec2 { p3.x, icon_max.y };
	const auto p7 = ImVec2 { p0.x + width * 0.1f, icon_max.y };
	const auto p8 = ImVec2 { p1.x, icon_max.y };

	const auto roof_points = { p4, p0, p2 };
	const auto wall_points = { p3, p6, p8, p1 };

	auto draw_list = ImGui::GetWindowDrawList();
	draw_list->AddPolyline(roof_points.begin(), roof_points.size(), color, false, 1.0f);
	draw_list->AddPolyline(wall_points.begin(), wall_points.size(), color, false, 1.0f);

	draw_list->AddRectFilled(p5, p7, color);
}

void FileBrowser::draw_desktop_icon(float width, float height, uint32_t color) const noexcept
{
	this->draw_none_icon(width, height);

	// min *-------------+     //
	//     |             |     //
	//     |             |     //
	//     |             |     //
	//     +-------------* p0  //
	//       p1   *      .     //
	//           / \  p3 .     //
	//       p2 *---*..... max //

	const auto item_size = ImVec2 { width, height };

	const auto icon_min = ImGui::GetItemRectMin();
	const auto icon_max = icon_min + item_size;

	const auto p0 = ImVec2 { icon_max.x, icon_min.y + height * 0.75f };

	const auto p1 = ImVec2 { icon_min.x + width * 0.5f, p0.y };
	const auto p2 = ImVec2 { p1.x - width * 0.2f, icon_max.y };
	const auto p3 = ImVec2 { p1.x + width * 0.2f, icon_max.y };

	auto draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRect(icon_min, p0, color, 2.0f);
	draw_list->AddTriangleFilled(p1, p2, p3, color);
}

void FileBrowser::draw_recent_icon(float width, float height, uint32_t color) const noexcept
{
	this->draw_none_icon(width, height);

	//   p1  *` ' `   p2   //
	//      ` \   * `      //
	//    `    \ /    `    //
	//   -      *      -   //
	//    `     p0    `    //
	//      `       `      //
	//        ` ' `        //

	const auto radius = width * 0.5f;
	const auto icon_min = ImGui::GetItemRectMin();

	const auto p0 = ImVec2 { icon_min.x + radius, icon_min.y + radius };

	const auto p1 = ImVec2 { p0.x - width * 0.2f, p0.y - radius * 0.65f };
	const auto p2 = ImVec2 { p0.x + width * 0.3f, p0.y - radius * 0.15f };

	const auto points = { p1, p0, p2 };

	auto draw_list = ImGui::GetWindowDrawList();
	draw_list->AddCircle(p0, radius, color);
	draw_list->AddPolyline(points.begin(), points.size(), color, false, 1.0f);
}

void FileBrowser::draw_trash_icon(float width, float height, uint32_t color) const noexcept
{
	const auto item_size = ImVec2 { width, height };
	this->draw_none_icon(width, height);

	width *= 0.8f;
	const auto icon_size = ImVec2 { width, item_size.y };

	// min ......*---+           //
	//     .  p0 |   |           //
	//  p1 *-----+---*-----* p3  //
	//              p2           //
	//          p6    p10        //
	//  p4 *    *     *    * p5  //
	//     |    |     |    |     //
	//     |    |     |    |     //
	//     |    *     *    |     //
	//     |    p7    p11  |     //
	// p12 *---------------* max //

	const auto icon_min = ImGui::GetItemRectMin() + ImVec2 { (item_size.x - width) * 0.5f, 0.0f };
	const auto icon_max = icon_min + icon_size;

	const auto p0 = ImVec2 { icon_min.x + icon_size.x * 0.3f, icon_min.y };

	const auto p1 = ImVec2 { icon_min.x, icon_min.y + height * 0.15f };
	const auto p2 = ImVec2 { icon_max.x - icon_size.x * 0.3f, p1.y };
	const auto p3 = ImVec2 { icon_max.x, p1.y };

	const auto p4 = ImVec2 { icon_min.x, p1.y + height * 0.2f };
	const auto p5 = ImVec2 { icon_max.x, p4.y };

	const auto x_offset = icon_size.x * 0.3f;
	const auto y_offset = icon_size.y * 0.15f;

	const auto p6 = ImVec2 { p4.x + x_offset, p4.y };
	const auto p7 = ImVec2 { p6.x, icon_max.y - y_offset };

	const auto p8 = ImVec2 { p6.x + x_offset, p6.y };
	const auto p9 = ImVec2 { p8.x, p7.y };

	const auto p12 = ImVec2 { icon_min.x, icon_max.y };

	const auto points = { p4, p12, icon_max, p5 };

	auto draw_list = ImGui::GetWindowDrawList();
	draw_list->AddLine(p1, p3, color);
	draw_list->AddRect(p0, p2, color);

	draw_list->AddPolyline(points.begin(), points.size(), color, false, 1.0f);
	draw_list->AddLine(p6, p7, color);
	draw_list->AddLine(p8, p9, color);
}

void FileBrowser::draw_dir_icon(float width, float height, uint32_t color) const noexcept
{
	const auto item_size = ImVec2 { width, height };
	this->draw_none_icon(width, height);

	height *= 0.9f;
	const auto icon_size = ImVec2 { item_size.x, height };

	//          min *------* p0
	//              |       \        p2
	//              |        *-------*
	//              |        p1      |
	//              |                |
	//              |                |
	// min.x, max.y *----------------* max

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

	//          min *----------*.... p2
	//              |          |\...
	//              |          | \..
	//              |          |  \.
	//              |       p0 *---* p1
	//              |              |
	//              |              |
	//              |              |
	//              |              |
	// min.x, max.y *--------------* max

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

void FileBrowser::show_item_context_menu() const noexcept
{
	if (ImGui::MenuItem("Rename")) {
	}

	ImGui::Separator();
	this->show_general_context_menu();
}

void FileBrowser::show_general_context_menu() const noexcept
{
	if (ImGui::BeginMenu("Create New ...")) {
		if (ImGui::MenuItem("Empty File")) {
		}
		if (ImGui::MenuItem("Directory")) {
		}

		ImGui::Separator();

		if (ImGui::MenuItem("C++ File")) {
		}
		if (ImGui::MenuItem("C++ Header File")) {
		}

		ImGui::EndMenu();
	}
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
	const auto icon_color = 0xFF999999;
	const auto icon_size = ImGui::GetTextLineHeight();

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode("Bookmarks")) {
		this->draw_home_icon(icon_size, icon_size, icon_color);
		ImGui::Text("Home");

		this->draw_desktop_icon(icon_size, icon_size, icon_color);
		ImGui::Text("Desktop");

		this->draw_recent_icon(icon_size, icon_size, icon_color);
		ImGui::Text("Recent");

		this->draw_trash_icon(icon_size, icon_size, icon_color);
		ImGui::TextDisabled("Trash");

		ImGui::TreePop();
	}
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

				ImGui::PushID(p.path().filename().c_str());
				if (ImGui::BeginPopupContextItem("general context menu")) {
					this->unselect_item();

					this->show_general_context_menu();
					ImGui::EndPopup();
				}

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
				if (ImGui::BeginPopupContextItem("item context menu")) {
					m_selected_item_id = id;

					this->show_item_context_menu();
					ImGui::EndPopup();
				}

				ImGui::PopID();
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
