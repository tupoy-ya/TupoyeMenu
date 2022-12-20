#include "views/view.hpp"
#include "util/teleport.hpp"
#include "util/vehicle.hpp"

namespace big
{
	void view::view_player_troll()
	{
		std::string title = std::format("Player Troll Options: {}", g_player_service->get_selected()->get_name());
		ImGui::Text(title.c_str());

		components::button("Teleport", []
		{
			teleport::to_player(g_player_service->get_selected()->id());
		});

		ImGui::SameLine();

		components::button("Bring", []
		{
			teleport::bring_player(g_player_service->get_selected());
		});

		components::button("Teleport into Vehicle", []
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_player_service->get_selected()->id()), false);
			teleport::into_vehicle(veh);
		});

		components::button("Remote Control Vehicle", []
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_player_service->get_selected()->id()), false);
			if (veh == 0)
			{
				if (g.player.spectating)
					g_notification_service->push_warning("Remote Control", "Player not in a vehicle");
				else
					g_notification_service->push_warning("Remote Control", "Player not in a vehicle, try spectating the player");
				return;
			}

			vehicle::remote_control_vehicle(veh);
			g.player.spectating = false;
		});

		components::button("Godmode kill", [] { teleport::godmode_kill(g_player_service->get_selected()); });
		ImGui::SameLine(); components::help_marker("Teleports players vehicle into killbox. \nBlocked by most internal menus.");
	}
}