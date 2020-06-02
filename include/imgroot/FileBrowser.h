#ifndef __IM_GROOT_FILE_BROWSER_H__
#define __IM_GROOT_FILE_BROWSER_H__

#include <filesystem>

namespace ImGroot
{

class FileBrowser
{
private:
	std::filesystem::path m_current_dir = std::filesystem::current_path();

private:
	void show_nav_menu() noexcept;
	void show_side_menu() noexcept;
	void show_dir_browser_list() noexcept;

public:
	void render() noexcept;
};

}

#endif // !__IM_GROOT_FILE_BROWSER_H__
