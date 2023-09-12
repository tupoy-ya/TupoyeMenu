#include "backend/player_command.hpp"
#include "fiber_pool.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "util/teleport.hpp"

namespace big
{
	class bring : player_command
	{
		using player_command::player_command;

		virtual void execute(player_ptr player, const command_arguments& _args, const std::shared_ptr<command_context> ctx) override
		{
			teleport::bring_player(player);
		}
	};

	class bring_all : command
	{
		using command::command;

		virtual void execute(const command_arguments& _args, const std::shared_ptr<command_context> ctx) override
		{
			for (auto& player : g_player_service->players())
				g_fiber_pool->queue_job([player]() {
					teleport::bring_player(player.second);
				});
		}
	};

	bring g_bring("bring", "Bring", "Teleports the player to you", 0, false);
	bring_all g_bring_all("bringall", "Bring All", "Teleports everyone to you, even if they are on foot", 0, false);
}