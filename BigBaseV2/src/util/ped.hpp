#pragma once
#include "natives.hpp"
#include "gta/joaat.hpp"
#include "pointers.hpp"
#include "script.hpp"

namespace big::ped
{
	inline void steal_outfit(const Ped target)
	{
		Ped ped = self::ped;
		
		if (ENTITY::GET_ENTITY_MODEL(ped) != ENTITY::GET_ENTITY_MODEL(target)) {
			g_notification_service->push("Error", "Model mismatch, use steal identity instead.");
			return;
		}
		for (int i = 0; i < 12; i++) {
			PED::SET_PED_COMPONENT_VARIATION
			(
				ped,
				i,
				PED::GET_PED_DRAWABLE_VARIATION(target, i),
				PED::GET_PED_TEXTURE_VARIATION(target, i),
				PED::GET_PED_PALETTE_VARIATION(target, i)
			);
		}
	}

	inline int spawn(std::string_view model, Vector3 location, float heading, bool is_networked = true)
	{
		if (const Hash hash = rage::joaat(model.data()); hash)
		{
			for (uint8_t i = 0; !STREAMING::HAS_MODEL_LOADED(hash) && i < 100; i++)
			{
				STREAMING::REQUEST_MODEL(hash);

				script::get_current()->yield();
			}
			if (!STREAMING::HAS_MODEL_LOADED(hash))
			{
				g_notification_service->push_warning("Spawn", "Failed to spawn model, did you give an incorrect model?");

				return -1;
			}

			*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
			Ped ped= PED::CREATE_PED(0, hash, location.x, location.y, location.z, heading, is_networked, false);
			*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;

			script::get_current()->yield();

			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

			if (*g_pointers->m_is_session_started)
			{
				DECORATOR::DECOR_SET_INT(ped, "MPBitset", 0);
				ENTITY::SET_ENTITY_CLEANUP_BY_ENGINE_(ped, true);
			}

			return ped;
		}

		return -1;
	}

	inline void steal_identity(const Ped target)
	{
		Ped ped = self::ped;

		const int max_health = ENTITY::GET_ENTITY_MAX_HEALTH(ped);
		const int current_health = ENTITY::GET_ENTITY_HEALTH(ped);
		const int current_armor = PED::GET_PED_ARMOUR(ped);

		PLAYER::SET_PLAYER_MODEL(self::id , ENTITY::GET_ENTITY_MODEL(target));
		PED::CLONE_PED_TO_TARGET(target, ped);
		ENTITY::SET_ENTITY_MAX_HEALTH(ped, max_health);
		ENTITY::SET_ENTITY_HEALTH(ped, current_health, 0);
		PED::SET_PED_ARMOUR(ped, current_armor);
	}
}