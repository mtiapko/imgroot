#include <cstdint>
#include "imgroot/FileBrowser.h"
#include "imgui.h"

namespace ImGroot
{

void FileBrowser::show_nav_menu() noexcept
{
	if (ImGui::ArrowButton("Back", ImGuiDir_Left))
		m_current_dir = m_current_dir.parent_path();

	ImGui::SameLine();
	ImGui::TextUnformatted(m_current_dir.c_str());
}

void FileBrowser::show_side_menu() noexcept
{
	ImGui::Text("Favorites");
	ImGui::Text("* Home");
	ImGui::Text("* Desktop");
	ImGui::Text("* Recent");
}

void FileBrowser::show_dir_browser_list() noexcept
{
	if (ImGui::BeginChild("Dir Browser", { 0.0f, -ImGui::GetStyle().CellPadding.y })) {
		auto dir_iter = std::filesystem::directory_iterator(m_current_dir);
		for (auto& p: dir_iter) {
			if (ImGui::Selectable(p.path().filename().c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					if (p.is_directory())
						m_current_dir = p.path();
				}
			}
		}
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
