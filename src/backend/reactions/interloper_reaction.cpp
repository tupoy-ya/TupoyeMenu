#include "interloper_reaction.hpp"
#include "backend/player_command.hpp"
#include "fiber_pool.hpp"
#include "script.hpp"
#include "hooking.hpp"
#include "pointers.hpp"
#include "util/notify.hpp"

namespace big
{
	interloper_reaction::interloper_reaction(const char* event_name, const char* notify_message, const char* announce_message, bool blockable, bool karmaable) :
		reaction(event_name, notify_message, announce_message),
		m_blockable(blockable),
		m_karmaable(karmaable)
	{
	}

	void interloper_reaction::process(player_ptr attacker, player_ptr victim)
	{
		if (!attacker->is_valid() || !victim->is_valid())
			return;

		if (announce_in_chat)
		{
			g_fiber_pool->queue_job([attacker, victim, this]
			{
				char chat[255];
				snprintf(chat, sizeof(chat), fmt::format("{} {}", g.session.chat_output_prefix, m_announce_message).data(), attacker->get_name(), victim->get_name());

				if (g_hooking->get_original<hooks::send_chat_message>()(*g_pointers->m_send_chat_ptr, g_player_service->get_self()->get_net_data(), chat, false))
					notify::draw_chat(chat, g_player_service->get_self()->get_name(), false);
			});
		}

		process_common(attacker);
	}
}