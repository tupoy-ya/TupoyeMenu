/**
 * @file view_weapons.cpp
 * 
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
 * YimMenu is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#include "core/data/bullet_impact_types.hpp"
#include "core/data/custom_weapons.hpp"
#include "core/data/special_ammo_types.hpp"
#include "fiber_pool.hpp"
#include "gta/joaat.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "services/gta_data/gta_data_service.hpp"
#include "services/persist_weapons/persist_weapons.hpp"
#include "views/view.hpp"

namespace big
{
	void view::weapons()
	{
		ImGui::Text("Ammo");

		ImGui::BeginGroup();
		components::command_checkbox<"infammo">();
		components::command_checkbox<"infrange">();
		ImGui::Checkbox("Increase C4 Limit (Max = 50)", &g.weapons.increased_c4_limit);
		ImGui::Checkbox("Allow Weapons In Interiors", &g.weapons.interior_weapon);
		components::command_checkbox<"rapidfire">();

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		components::command_checkbox<"alwaysfullammo">();
		components::command_checkbox<"infclip">();
		ImGui::Checkbox("Increase Flare Limit (Max = 50)", &g.weapons.increased_flare_limit);
		components::command_checkbox<"vehallweapons">();

		ImGui::Checkbox("Enable Special Ammo", &g.weapons.ammo_special.toggle);
		components::options_modal("Special ammo", [] {
			eAmmoSpecialType selected_ammo   = g.weapons.ammo_special.type;
			eExplosionTag selected_explosion = g.weapons.ammo_special.explosion_tag;

			if (ImGui::BeginCombo("Special Ammo", SPECIAL_AMMOS[(int)selected_ammo].name))
			{
				for (const auto& special_ammo : SPECIAL_AMMOS)
				{
					if (ImGui::Selectable(special_ammo.name, special_ammo.type == selected_ammo))
					{
						g.weapons.ammo_special.type = special_ammo.type;
					}

					if (special_ammo.type == selected_ammo)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (ImGui::BeginCombo("Bullet Impact", BULLET_IMPACTS[selected_explosion]))
			{
				for (const auto& [type, name] : BULLET_IMPACTS)
				{
					if (ImGui::Selectable(name, type == selected_explosion))
					{
						g.weapons.ammo_special.explosion_tag = type;
					}

					if (type == selected_explosion)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}
		});

		ImGui::EndGroup();

		ImGui::SeparatorText("Misc");

		components::command_checkbox<"norecoil">();
		ImGui::SameLine();
		components::command_checkbox<"nospread">();

		components::button("Get All Weapons", [] {
			for (const auto& [_, weapon] : g_gta_data_service->weapons())
			{
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(self::ped, weapon.m_hash, 9999, false);
			}

			constexpr auto parachute_hash = RAGE_JOAAT("GADGET_PARACHUTE");
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(self::ped, parachute_hash, 0, true);
		});
		ImGui::SameLine();
		components::button("Remove Current Weapon", [] {
			Hash weaponHash;
			WEAPON::GET_CURRENT_PED_WEAPON(self::ped, &weaponHash, 1);
			if (weaponHash != RAGE_JOAAT("WEAPON_UNARMED"))
			{
				WEAPON::REMOVE_WEAPON_FROM_PED(self::ped, weaponHash);
			}
		});

		components::command_checkbox<"incrdamage">();
		ImGui::InputFloat("Damage", &g.weapons.increased_damage, .1, 10, "%.1f");

		ImGui::SeparatorText("Custom Weapons");

		ImGui::Checkbox("Custom Gun only fires when weapon is out", &g.self.custom_weapon_stop);
		CustomWeapon selected = g.weapons.custom_weapon;

		if (ImGui::BeginCombo("Weapon", custom_weapons[(int)selected].name))
		{
			for (const custom_weapon& weapon : custom_weapons)
			{
				if (ImGui::Selectable(weapon.name, weapon.id == selected))
				{
					g.weapons.custom_weapon = weapon.id;
				}

				if (weapon.id == selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}

		switch (selected)
		{
		case CustomWeapon::GRAVITY_GUN:
			ImGui::Checkbox("Launch on release", &g.weapons.gravity_gun.launch_on_release);
			break;
		case CustomWeapon::VEHICLE_GUN:
			// this some ugly ass looking code
			static char vehicle_gun[12];
			std::memcpy(vehicle_gun, g.weapons.vehicle_gun_model.c_str(), 12);
			if (ImGui::InputTextWithHint("Shooting Model", "Name of the vehicle model", vehicle_gun, sizeof(vehicle_gun)))
			{
				g.weapons.vehicle_gun_model = vehicle_gun;
			}
			if (ImGui::IsItemActive())
			{
				g_fiber_pool->queue_job([] {
					PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
				});
			}

			break;
		case CustomWeapon::PAINT_GUN:
			ImGui::Checkbox("Rainbow Color", &g.weapons.paintgun.rainbow);
			ImGui::SliderFloat("Rainbow Speed", &g.weapons.paintgun.speed, 0.f, 10.f);
			if (!g.weapons.paintgun.rainbow) { ImGui::ColorEdit4("Paint Gun Color", g.weapons.paintgun.col); }
		}

		if (ImGui::CollapsingHeader("Ammunation"))
		{
			static Hash selected_weapon_hash, selected_weapon_attachment_hash{};
			static std::string selected_weapon, selected_weapon_attachment;
			ImGui::PushItemWidth(300);
			if (ImGui::BeginCombo("Weapons", selected_weapon.c_str()))
			{
				for (auto& weapon : g_gta_data_service->weapons())
				{
					bool is_selected = weapon.second.m_hash == selected_weapon_hash;
					if (weapon.second.m_display_name != "NULL" && ImGui::Selectable(weapon.second.m_display_name.c_str(), is_selected, ImGuiSelectableFlags_None))
					{
						selected_weapon                 = weapon.second.m_display_name;
						selected_weapon_hash            = weapon.second.m_hash;
						selected_weapon_attachment_hash = {};
						selected_weapon_attachment.clear();
					}
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			ImGui::SameLine();
			components::button("Give Weapon", [] {
				WEAPON::GIVE_WEAPON_TO_PED(self::ped, selected_weapon_hash, 9999, false, true);
			});
			ImGui::SameLine();
			components::button("Remove Weapon", [] {
				WEAPON::REMOVE_WEAPON_FROM_PED(self::ped, selected_weapon_hash);
			});

			ImGui::PushItemWidth(250);
			if (ImGui::BeginCombo("Attachments", selected_weapon_attachment.c_str()))
			{
				weapon_item weapon = g_gta_data_service->weapon_by_hash(selected_weapon_hash);
				if (!weapon.m_attachments.empty())
				{
					for (std::string attachment : weapon.m_attachments)
					{
						weapon_component attachment_component   = g_gta_data_service->weapon_component_by_name(attachment);
						std::string attachment_name = attachment_component.m_display_name;
						Hash attachment_hash        = attachment_component.m_hash;
						if (attachment_hash == NULL)
						{
							attachment_name = attachment;
							attachment_hash = rage::joaat(attachment);
						}
						bool is_selected         = attachment_hash == selected_weapon_attachment_hash;
						std::string display_name = attachment_name.append("##").append(std::to_string(attachment_hash));
						if (ImGui::Selectable(display_name.c_str(), is_selected, ImGuiSelectableFlags_None))
						{
							selected_weapon_attachment      = attachment_name;
							selected_weapon_attachment_hash = attachment_hash;
						}
						if (is_selected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
				}

				ImGui::EndCombo();
			}
			ImGui::SameLine();
			components::button("Add to Weapon", [] {
				WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(self::ped, selected_weapon_hash, selected_weapon_attachment_hash);
			});
			ImGui::SameLine();
			components::button("Remove from Weapon", [] {
				WEAPON::REMOVE_WEAPON_COMPONENT_FROM_PED(self::ped, selected_weapon_hash, selected_weapon_attachment_hash);
			});
			ImGui::PopItemWidth();

			static const char* default_tints[]{"Black tint", "Green tint", "Gold tint", "Pink tint", "Army tint", "LSPD tint", "Orange tint", "Platinum tint"};
			static const char* mk2_tints[]{"Classic Black", "Classic Grey", "Classic Two - Tone", "Classic White", "Classic Beige", "Classic Green", "Classic Blue", "Classic Earth", "Classic Brown & Black", "Red Contrast", "Blue Contrast", "Yellow Contrast", "Orange Contrast", "Bold Pink", "Bold Purple & Yellow", "Bold Orange", "Bold Green & Purple", "Bold Red Features", "Bold Green Features", "Bold Cyan Features", "Bold Yellow Features", "Bold Red & White", "Bold Blue & White", "Metallic Gold", "Metallic Platinum", "Metallic Grey & Lilac", "Metallic Purple & Lime", "Metallic Red", "Metallic Green", "Metallic Blue", "Metallic White & Aqua", "Metallic Red & Yellow"};
			static int tint;

			if (selected_weapon.ends_with("Mk II"))
			{
				ImGui::Combo("Tints", &tint, mk2_tints, IM_ARRAYSIZE(mk2_tints));
			}
			else
			{
				ImGui::Combo("Tints", &tint, default_tints, IM_ARRAYSIZE(default_tints));
			}
			ImGui::SameLine();
			components::button("Apply", [] {
				WEAPON::SET_PED_WEAPON_TINT_INDEX(self::ped, selected_weapon_hash, tint);
			});
		}
		if (ImGui::CollapsingHeader("Persist Weapons"))
		{
			ImGui::Checkbox("Enabled##persist_weapons", &g.persist_weapons.enabled);

			static std::string selected_loadout = g.persist_weapons.weapon_loadout_file;
			ImGui::PushItemWidth(250);
			if (ImGui::BeginListBox("Saved Loadouts", ImVec2(200, 200)))
			{
				for (std::string filename : persist_weapons::list_weapon_loadouts())
				{
					if (components::selectable(filename, filename == selected_loadout))
					{
						selected_loadout = filename;
					}
				}
				ImGui::EndListBox();
			}
			ImGui::SameLine();
			ImGui::BeginGroup();
			static std::string input_file_name;
			components::input_text_with_hint("Weapon Loadout Filename", "Loadout Name", input_file_name);
			components::button("Save Loadout", [] {
				persist_weapons::save_weapons(input_file_name);
				input_file_name.clear();
			});
			ImGui::SameLine();
			components::button("Load Loadout", [] {
				persist_weapons::give_player_loadout(selected_loadout);
			});
			ImGui::SameLine();
			components::button("Set Loadout", [] {
				persist_weapons::set_weapon_loadout(selected_loadout);
			});
			ImGui::Text(std::format("Current Loadout: {}:", g.persist_weapons.weapon_loadout_file).data());
			ImGui::EndGroup();
			ImGui::PopItemWidth();
		}
		if (ImGui::CollapsingHeader("Weapon Hotkeys"))
		{
			ImGui::Checkbox("Enabled##weapon_hotkeys", &g.weapons.enable_weapon_hotkeys);
			components::help_marker("This will select the next weapon in the hotkey list.\r\nThe first weapon in the list is the first weapon it will select, then the second is the one it will select after and so on.\r\nAfter the end of the list, it will wrap back to the first weapon.");

			static int selected_key = 0;
			const char* const keys[]{"1", "2", "3", "4", "5", "6"};

			ImGui::PushItemWidth(250);
			ImGui::Combo("Key", &selected_key, keys, IM_ARRAYSIZE(keys));
			ImGui::PopItemWidth();

			if (!g.weapons.weapon_hotkeys[selected_key].empty())
			{
				int counter{};
				for (auto& weapon_hash : g.weapons.weapon_hotkeys[selected_key])
				{
					ImGui::PushID(counter);
					weapon_item weapon = g_gta_data_service->weapon_by_hash(weapon_hash);
					ImGui::PushItemWidth(300);
					if (ImGui::BeginCombo("Weapons", weapon.m_display_name.c_str()))
					{
						for (auto& weapon : g_gta_data_service->weapons())
						{
							if (weapon.second.m_display_name == "NULL")
							{
								continue;
							}
							bool is_selected = weapon.second.m_hash == weapon_hash;
							if (ImGui::Selectable(weapon.second.m_display_name.c_str(), is_selected, ImGuiSelectableFlags_None))
							{
								weapon_hash = weapon.second.m_hash;
							}
							if (is_selected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}
					ImGui::SameLine();
					components::button("Set To Current Weapon", [&weapon_hash] {
						WEAPON::GET_CURRENT_PED_WEAPON(self::ped, &weapon_hash, NULL);
						if (weapon_hash == NULL)
						{
							WEAPON::GET_CURRENT_PED_VEHICLE_WEAPON(self::ped, &weapon_hash);
						}
					});
					ImGui::SameLine();
					if (ImGui::Button("Remove Weapon"))
					{
						g.weapons.weapon_hotkeys[selected_key].erase(g.weapons.weapon_hotkeys[selected_key].begin() + counter);
					}
					ImGui::PopID();
					ImGui::PopItemWidth();
					counter++;
				}
			}

			if (ImGui::Button("Add Weapon"))
			{
				g.weapons.weapon_hotkeys[selected_key].push_back(RAGE_JOAAT("WEAPON_UNARMED"));
			}
		}
	}
}
