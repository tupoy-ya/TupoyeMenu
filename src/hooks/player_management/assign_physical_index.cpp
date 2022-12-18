#include "hooking.hpp"
#include "fiber_pool.hpp"
#include "services/players/player_service.hpp"
#include "services/player_database/player_database_service.hpp"
#include "util/notify.hpp"
#include "util/session.hpp"
#include "packet.hpp"
#include "gta_util.hpp"
#include <network/Network.hpp>

namespace big
{
	void* hooks::assign_physical_index(CNetworkPlayerMgr* netPlayerMgr, CNetGamePlayer* player, uint8_t new_index)
	{
		const auto* net_player_data = player->get_net_data();

		if (new_index == static_cast<uint8_t>(-1))
		{
			g->m_spoofed_peer_ids.erase(player->get_net_data()->m_host_token);
			g_player_service->player_leave(player);

			if (net_player_data)
			{
				if (g->notifications.player_leave.log)
					LOG(INFO) << "Player left '" << net_player_data->m_name
						<< "' freeing slot #" << (int)player->m_player_id
						<< " with Rockstar ID: " << net_player_data->m_gamer_handle_2.m_rockstar_id;

				if (g->notifications.player_leave.notify)
					g_notification_service->push("Player Left", std::format("{} freeing slot #{} with Rockstar ID: {}", net_player_data->m_name, player->m_player_id, net_player_data->m_gamer_handle_2.m_rockstar_id));
			}

			return g_hooking->get_original<hooks::assign_physical_index>()(netPlayerMgr, player, new_index);
		}

		const auto result = g_hooking->get_original<hooks::assign_physical_index>()(netPlayerMgr, player, new_index);
		g_player_service->player_join(player);
		if (net_player_data)
		{
			if (g->notifications.player_join.above_map && *g_pointers->m_is_session_started) // prevent loading screen spam
				notify::player_joined(player);

			if (g->notifications.player_join.log)
				LOG(INFO) << "Player joined '" << net_player_data->m_name
					<< "' allocating slot #" << (int)player->m_player_id
					<< " with Rockstar ID: " << net_player_data->m_gamer_handle_2.m_rockstar_id;

			if (g->notifications.player_join.notify)
				g_notification_service->push("Player Joined", std::format("{} taking slot #{} with Rockstar ID: {}", net_player_data->m_name, player->m_player_id, net_player_data->m_gamer_handle_2.m_rockstar_id));

			auto id = player->m_player_id;
			g_fiber_pool->queue_job([id]
			{
				if (auto plyr = g_player_service->get_by_id(id))
				{
					if(plyr->get_net_game_player()->m_is_rockstar_dev || plyr->get_net_game_player()->m_is_rockstar_qa)
					{
						//session::add_infraction(plyr, Infraction::ROCKSTAR_ADMIN_FLAG);

						LOG(WARNING) << std::format("Name: {}, m_crew_rank_title: {}, m_is_rockstar_dev: {}, m_is_rockstar_qa: {}, m_is_cheater: {}",
							plyr->get_name(),
							plyr->get_net_game_player()->m_crew_rank_title,
							plyr->get_net_game_player()->m_is_rockstar_dev,
							plyr->get_net_game_player()->m_is_rockstar_qa,
							plyr->get_net_game_player()->m_is_cheater
						);
					}

					if (auto entry = g_player_database_service->get_player_by_rockstar_id(plyr->get_net_data()->m_gamer_handle_2.m_rockstar_id))
					{
						plyr->is_modder = entry->is_modder;
						plyr->is_rockstar_admin = entry->is_rockstar_admin;
						plyr->block_join = entry->block_join;
						plyr->block_join_reason = plyr->block_join_reason;

						if(entry->is_rockstar_admin)
						{
							std::string text = std::format("R* Admin: {} #{} joined", entry->name, entry->rockstar_id);
							g_notification_service->push_error("R* Admin Joined", text);
							LOG(WARNING) << text;
						}

						if (strcmp(plyr->get_name(), entry->name.data()))
						{
							g_notification_service->push("Players", std::format("{} changed their name to {}", entry->name, plyr->get_name()));
							entry->name = plyr->get_name();
							g_player_database_service->save();
						}
					}

					if (auto snplyr = plyr->get_session_player())
					{
						packet msg{};
						msg.write_message(rage::eNetMessage::MsgSessionEstablishedRequest);
						msg.write<uint64_t>(gta_util::get_network()->m_game_session_ptr->m_rline_session.m_session_id, 64);
						msg.send(snplyr->m_msg_id);
					}
				}
			});
		}
		return result;
	}
}
