#pragma once
#include "gta/joaat.hpp"

namespace big
{
	// Add new values to the bottom
	enum class Infraction
	{
		DESYNC_PROTECTION,             // do not use
		BREAKUP_KICK_DETECTED,         // do not use
		LOST_CONNECTION_KICK_DETECTED, // do not use
		SPOOFED_ROCKSTAR_ID,           // do not use
		TRIGGERED_ANTICHEAT,
		TRIED_CRASH_PLAYER,
		TRIED_KICK_PLAYER,
		TRIED_TELEPORT_PLAYER,
		BLAME_EXPLOSION_DETECTED, // do not use (for now)
		ROCKSTAR_ADMIN_FLAG,
		ATTACKING_WITH_GODMODE,
		ATTACKING_WITH_INVISIBILITY,
		ATTACKING_WHEN_HIDDEN_FROM_PLAYER_LIST,
		SPOOFED_DATA,
		SPOOFED_HOST_TOKEN,
		INVALID_PLAYER_MODEL,
		SUPER_JUMP,
		UNDEAD_OTR,
	};

	inline std::unordered_map<Infraction, const char*> infraction_desc = {
	    {Infraction::DESYNC_PROTECTION, "Used desync protections"},
	    {Infraction::BREAKUP_KICK_DETECTED, "Kicked someone using breakup kick"},
	    {Infraction::LOST_CONNECTION_KICK_DETECTED, "Tried to kick someone using lost connection kick"},
	    {Infraction::SPOOFED_ROCKSTAR_ID, "Had spoofed RID"},
	    {Infraction::TRIGGERED_ANTICHEAT, "Triggered Rockstar's anticheat"},
	    {Infraction::TRIED_CRASH_PLAYER, "Tried to crash you"},
	    {Infraction::TRIED_KICK_PLAYER, "Tried to kick you"},
		{Infraction::TRIED_TELEPORT_PLAYER, "Tried to teleport you"},
	    {Infraction::BLAME_EXPLOSION_DETECTED, "Tried to blame someone for their explosion"},
		{Infraction::ROCKSTAR_ADMIN_FLAG, "Has m_is_rockstar_dev set to true"},
	    {Infraction::ATTACKING_WITH_GODMODE, "Attacked someone while using godmode"},
	    {Infraction::ATTACKING_WITH_INVISIBILITY, "Attacked someone while being invisible"},
	    {Infraction::ATTACKING_WHEN_HIDDEN_FROM_PLAYER_LIST, "Attacked someone while being hidden from the player list"},
	    {Infraction::SPOOFED_DATA, "Had spoofed data"},
	    {Infraction::SPOOFED_HOST_TOKEN, "Had spoofed their host token"},
	    {Infraction::INVALID_PLAYER_MODEL, "Had used an invalid player model"},
	    {Infraction::SUPER_JUMP, "Had used super jump"},
	    {Infraction::UNDEAD_OTR, "Had used undead OTR"},
	};
}