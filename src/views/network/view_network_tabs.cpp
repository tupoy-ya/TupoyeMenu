#include "pointers.hpp"
#include "views/view.hpp"

namespace big
{
	void view::network_tabs()
	{
		if (ImGui::BeginTabBar("network_tabbar"))
		{
			if (ImGui::BeginTabItem("Spoofing"))
			{
				view::spoofing();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Session"))
			{
				view::session();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Missions"))
			{
				view::missions();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Player Database"))
			{
				view::player_database();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Session Browser"))
			{
				view::session_browser();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Stat Editor"))
			{
				view::stat_editor();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}
