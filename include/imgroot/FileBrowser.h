#ifndef __IM_GROOT_FILE_BROWSER_H__
#define __IM_GROOT_FILE_BROWSER_H__

#include <filesystem>

namespace ImGroot
{

class FileBrowser
{
private:
	std::filesystem::path m_current_dir = std::filesystem::current_path();
	uint32_t m_selected_item_id = std::numeric_limits<uint32_t>::max();

private:
	void unselect_item() noexcept;

private:
	void draw_none_icon(float width, float height) const noexcept;

	void draw_home_icon(float width, float height, uint32_t color) const noexcept;
	void draw_desktop_icon(float width, float height, uint32_t color) const noexcept;
	void draw_recent_icon(float width, float height, uint32_t color) const noexcept;
	void draw_trash_icon(float width, float height, uint32_t color) const noexcept;

	void draw_image_file_icon(float width, float height, uint32_t color) const noexcept;
	void draw_file_icon(float width, float height, uint32_t color) const noexcept;
	void draw_dir_icon(float width, float height, uint32_t color) const noexcept;

	enum class ButtonState
	{
		NONE = 0,
		HOVERED,
		CLICKED
	};

	ButtonState draw_wide_button(bool selected, uint32_t hovered_color, uint32_t active_color) const noexcept;

private:
	void show_item_context_menu() const noexcept;
	void show_general_context_menu() const noexcept;

	void show_nav_menu() noexcept;
	void show_side_menu() noexcept;
	void show_dir_browser_list() noexcept;

public:
	void render() noexcept;
};

}

#endif // !__IM_GROOT_FILE_BROWSER_H__
