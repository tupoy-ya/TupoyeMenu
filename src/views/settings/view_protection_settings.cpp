#include "views/view.hpp"

namespace big
{
	static inline void set_all_protections(bool state)
	{
		for (size_t i = (size_t)&g.protections; i <= (size_t) & (g.protections.kick_rejoin); i++)
			*(bool*)i = state;
	}

	static inline void reset_protections()
	{
		g.protections = {};
	}

	void view::protection_settings()
	{
		ImGui::BeginGroup();
		ImGui::Checkbox("Bounty", &g.protections.script_events.bounty);
		ImGui::Checkbox("CEO Money", &g.protections.script_events.ceo_money);
		ImGui::Checkbox("Force Mission", &g.protections.script_events.force_mission);
		ImGui::Checkbox("Force Teleport", &g.protections.script_events.force_teleport);
		ImGui::Checkbox("GTA Banner", &g.protections.script_events.gta_banner);
		ImGui::Checkbox("MC Teleport", &g.protections.script_events.mc_teleport);
		ImGui::Checkbox("Teleport To Warehouse", &g.protections.script_events.teleport_to_warehouse);
#ifndef ENABLE_SOCIALCLUB
		ImGui::BeginDisabled(true);
#endif // ENABLE_SOCIALCLUB
		ImGui::Checkbox("Block RID Joining", &g.protections.rid_join);
		ImGui::SameLine(); components::help_marker("This will block anyone trying to join, kick or crash you with your Rockstar ID, including your friends");
#ifndef ENABLE_SOCIALCLUB
		ImGui::EndDisabled();
		ImGui::SameLine(); components::help_marker("This feature is currently disabled, Enable `ENABLE_SOCIALCLUB` to enable.");
#endif // ENABLE_SOCIALCLUB
		ImGui::Checkbox("Admin Check", &g.protections.admin_check);
		ImGui::Checkbox("Start Activity", &g.protections.script_events.start_activity);
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::Checkbox("Send to Cutscene", &g.protections.script_events.send_to_cutscene);
		ImGui::Checkbox("Send to Location", &g.protections.script_events.send_to_location);
		ImGui::Checkbox("Sound Spam", &g.protections.script_events.sound_spam);
		ImGui::Checkbox("Personal Vehicle Destroyed", &g.protections.script_events.personal_vehicle_destroyed);
		ImGui::Checkbox("Remote Off Radar", &g.protections.script_events.remote_off_radar);
		ImGui::Checkbox("Rotate Cam", &g.protections.script_events.rotate_cam);
		ImGui::Checkbox("Receive Pickup", &g.protections.receive_pickup);
		ImGui::SameLine(); components::help_marker("This prevents any pickup from the ground such as unwanted money drops.\nAttention: Normal pickups are also no longer possible.");
		ImGui::Checkbox("Kick Rejoin", &g.protections.kick_rejoin);
		ImGui::Checkbox("Block Traffic Manipulation", &g.protections.stop_traffic);
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::Checkbox("Start Activity", &g.protections.script_events.start_activity);
		ImGui::Checkbox("Spectate", &g.protections.script_events.spectate);
		ImGui::Checkbox("Vehicle Kick", &g.protections.script_events.vehicle_kick);
		ImGui::Checkbox("Wanted Level", &g.protections.script_events.clear_wanted_level);
		ImGui::Checkbox("Request Controll", &g.protections.request_control);
		components::command_checkbox<"forcerelays">();
		ImGui::Checkbox("Desync Kick", &g.protections.desync_kick);
		ImGui::Checkbox("Give collectible", &g.protections.script_events.give_collectible);
		ImGui::EndGroup();

		ImGui::SeparatorText("Options");
		ImGui::BeginGroup();
		if (ImGui::Button("Enable All Protections"))
			set_all_protections(true);
		ImGui::SameLine();
		if (ImGui::Button("Disable All Protections"))
			set_all_protections(false);
		ImGui::SameLine();
		if (ImGui::Button("Reset Protections"))
			reset_protections();
		ImGui::EndGroup();
	};
}
