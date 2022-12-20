#include "fiber_pool.hpp"
#include "util/entity.hpp"
#include "util/local_player.hpp"
#include "views/view.hpp"
#include "core/data/hud_component_names.hpp"
#include "util/scripts.hpp"

namespace big
{
	void view::self()
	{
		components::button("Suicide", [] {
			ENTITY::SET_ENTITY_HEALTH(self::ped, 0, 0);
		});

		ImGui::SameLine();

		components::button("Heal", [] {
			local_player::heal_player();
		});

		ImGui::SameLine();

		components::button("Fill Inventory", [] {
			local_player::fill_inventory();
		});

		ImGui::SameLine();

		components::button("MAX BEER", [] {
			std::string mpPrefix = local_player::get_mp_prefix();
			STATS::STAT_SET_INT(rage::joaat(mpPrefix + "NUMBER_OF_BOURGE_BOUGHT"), INT32_MAX, true);
		});

		ImGui::SameLine();

		components::button("Skip Cutscene", [] {
			CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
		});

		ImGui::SameLine();

		components::button("Clean Player", [] {
			entity::clean_ped(self::ped);
		});

		ImGui::Separator();

		components::sub_title("General");

		ImGui::BeginGroup();

		ImGui::Checkbox("God Mode", &g.self.god_mode);
		ImGui::Checkbox("Off Radar", &g.self.off_radar);
		ImGui::Checkbox("Free Cam", &g.self.free_cam);
		ImGui::Checkbox("Disable Phone", &g.tunables.disable_phone);
		ImGui::Checkbox("Unlimited Oxygen", &g.self.unlimited_oxygen);
		ImGui::Checkbox("Fast Respawn", &g.self.fast_respawn);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("No Clip", &g.self.noclip);
		ImGui::Checkbox("No Ragdoll", &g.self.no_ragdoll);
		ImGui::Checkbox("Super Run", &g.self.super_run);
		ImGui::Checkbox("No Idle Kick", &g.tunables.no_idle_kick);
		ImGui::Checkbox("No Water Collision", &g.self.no_water_collision);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Invisibility", &g.self.invisibility);
		if (g.self.invisibility) {
			ImGui::Checkbox("Locally Visible", &g.self.local_visibility);
		}
		ImGui::Checkbox("Keep Player Clean", &g.self.clean_player);
		ImGui::Checkbox("No Collision", &g.self.no_collision);
		ImGui::Checkbox("Mobile Radio", &g.self.mobile_radio);

		ImGui::Checkbox("Dance Mode", &g.self.dance_mode);
		ImGui::SameLine(); components::help_marker("Hold Right DPAD or E to enter dance mode");

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Phone Anim", &g.tunables.phone_anim);
		ImGui::Checkbox("Jump Ragdoll", &g.self.allow_ragdoll);
		ImGui::Checkbox("Disable Help Text", &g.tunables.disable_help_text);
		ImGui::Checkbox("Always Control", &g.tunables.always_control);

		ImGui::EndGroup();

		ImGui::Separator();

		components::sub_title("Proofs");

		if (ImGui::Button("Check all"))
		{
			g.self.proof_bullet = true;
			g.self.proof_fire = true;
			g.self.proof_collision = true;
			g.self.proof_melee = true;
			g.self.proof_explosion = true;
			g.self.proof_steam = true;
			g.self.proof_drown = true;
			g.self.proof_water = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("Uncheck all"))
		{
			g.self.proof_bullet = false;
			g.self.proof_fire = false;
			g.self.proof_collision = false;
			g.self.proof_melee = false;
			g.self.proof_explosion = false;
			g.self.proof_steam = false;
			g.self.proof_drown = false;
			g.self.proof_water = false;
		}

		ImGui::BeginGroup();

		ImGui::Checkbox("Bullet", &g.self.proof_bullet);
		ImGui::Checkbox("Fire", &g.self.proof_fire);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Collision", &g.self.proof_collision);
		ImGui::Checkbox("Melee", &g.self.proof_melee);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Explosion", &g.self.proof_explosion);
		ImGui::Checkbox("Steam", &g.self.proof_steam);

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Drown", &g.self.proof_drown);
		ImGui::Checkbox("Water", &g.self.proof_water);

		ImGui::EndGroup();

		ImGui::Separator();

		components::sub_title("Police");

		ImGui::Checkbox("Never Wanted", &g.self.never_wanted);

		if (!g.self.never_wanted)
		{
			ImGui::Checkbox("Force Wanted Level", &g.self.force_wanted_level);
			ImGui::Text("Wanted Level");
			if (
				ImGui::SliderInt("###wanted_level", &g.self.wanted_level, 0, 5) &&
				!g.self.force_wanted_level &&
				g_local_player != nullptr
				) {
				g_local_player->m_player_info->m_wanted_level = g.self.wanted_level;
			}
		}

		ImGui::Separator();

		components::sub_title("HUD");

		ImGui::BeginGroup();

		ImGui::Checkbox("Hide Radar", &g.self.hide_radar);

		ImGui::SameLine();

		ImGui::Checkbox("Hide Ammo", &g.self.hide_ammo);

		ImGui::SameLine();

		ImGui::Checkbox("Force show HUD", &g.self.force_show_hud);

		ImGui::Combo("##hud_comp_combo", &g.self.selected_hud_component, hud_component_names, (int)HudComponents::HUD_WEAPONS);
		ImGui::SameLine();
		components::button("Hide", [] {
			g.self.hud_components_states[g.self.selected_hud_component] = true;
		});
		ImGui::SameLine();
		components::button("Show", [] {
			g.self.hud_components_states[g.self.selected_hud_component] = false;
		});

		components::button("Hide all", [] {
			g.self.hide_radar = true;
			g.self.hide_ammo = true;
			for (int i = 0; i < (int)HudComponents::HUD_WEAPONS; i++)
			{
				g.self.hud_components_states[i] = true;
			}
		});
		ImGui::SameLine();
		components::button("Show all", [] {
			g.self.hide_radar = false;
			g.self.hide_ammo = false;
			for (int i = 0; i < (int)HudComponents::HUD_WEAPONS; i++)
			{
				g.self.hud_components_states[i] = false;
			}
		});
		ImGui::SameLine();

		ImGui::Checkbox("Force show HUD element", &g.self.force_show_hud_element);
		ImGui::SameLine(); components::help_marker("To force show a HUD specific element, click Hide all then click Show on the desired element.");

		ImGui::EndGroup();

		g.self.proof_mask = 0;
		if (g.self.god_mode)
		{
			g.self.proof_mask |= static_cast<int>(eEntityProofs::GOD);
		}
		if (g.self.proof_bullet)
		{
			g.self.proof_mask |= static_cast<int>(eEntityProofs::BULLET);
		}
		if (g.self.proof_fire)
		{
			g.self.proof_mask |= static_cast<int>(eEntityProofs::FIRE);
		}
		if (g.self.proof_collision)
		{
			g.self.proof_mask |= static_cast<int>(eEntityProofs::COLLISION);
		}
		if (g.self.proof_melee)
		{
			g.self.proof_mask |= static_cast<int>(eEntityProofs::MELEE);
		}
		if (g.self.proof_explosion)
		{
			g.self.proof_mask |= static_cast<int>(eEntityProofs::EXPLOSION);
		}
		if (g.self.proof_steam)
		{
			g.self.proof_mask |= static_cast<int>(eEntityProofs::STEAM);
		}
		if (g.self.proof_drown)
		{
			g.self.proof_mask |= static_cast<int>(eEntityProofs::DROWN);
		}
		if (g.self.proof_water)
		{
			g.self.proof_mask |= static_cast<int>(eEntityProofs::WATER);
		}
	}
}
