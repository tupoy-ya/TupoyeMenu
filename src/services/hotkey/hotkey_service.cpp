#include "hotkey_service.hpp"
#include "fiber_pool.hpp"
#include "util/teleport.hpp"
#include "util/local_player.hpp"
#include "hotkey_functions.hpp"
#include "renderer.hpp"

#include "network/ChatData.hpp"
#include "pointers.hpp"

namespace big
{
    hotkey_service::hotkey_service()
    {
        register_hotkey("waypoint", g->settings.hotkeys.teleport_waypoint, teleport::to_waypoint);
        register_hotkey("objective", g->settings.hotkeys.teleport_objective, teleport::to_objective);
        register_hotkey("noclip", g->settings.hotkeys.noclip, hotkey_funcs::toggle_noclip);
        register_hotkey("heal", g->settings.hotkeys.heal, local_player::heal_player);
        register_hotkey("fill_inventory", g->settings.hotkeys.fill_inventory, local_player::fill_inventory);
        register_hotkey("skip_cutscene", g->settings.hotkeys.skip_cutscene, CUTSCENE::STOP_CUTSCENE_IMMEDIATELY);
        register_hotkey("off_radar", g->settings.hotkeys.skip_cutscene, hotkey_funcs::toggle_off_radar);

        g_renderer->add_wndproc_callback([this](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
        {
            wndproc(static_cast<eKeyState>(msg), wparam);
        });

        g_hotkey_service = this;
    }

    hotkey_service::~hotkey_service()
    {
        g_hotkey_service = nullptr;
    }

    void hotkey_service::register_hotkey(const std::string_view name, key_t key, hotkey_func func, eKeyState state, std::optional<std::chrono::high_resolution_clock::duration> cooldown)
    {
        m_hotkeys[state == eKeyState::RELEASE].emplace(key, hotkey(rage::joaat(name), key, func, cooldown));
    }

    bool hotkey_service::update_hotkey(const std::string_view name, const key_t key)
    {
        static auto update_hotkey_map = [](hotkey_map& hotkey_map, rage::joaat_t name_hash, key_t new_key) -> bool
        {
            for (auto it = hotkey_map.begin(); it != hotkey_map.end(); ++it)
            {
                auto hotkey = it->second;
                if (hotkey.name_hash() != name_hash)
                    continue;

                hotkey_map.erase(it);
                hotkey.set_key(new_key);
                hotkey_map.emplace(new_key, hotkey);

                return true;
            }
            return false;
        };

        const auto name_hash = rage::joaat(name);
        return update_hotkey_map(m_hotkeys[1], name_hash, key) // released
            && update_hotkey_map(m_hotkeys[0], name_hash, key); // down
    }

    void hotkey_service::wndproc(eKeyState state, key_t key)
    {
        if (const auto chat_data = *g_pointers->m_chat_data; chat_data && (chat_data->m_chat_open || chat_data->m_timer_two))
            return;

        if (state == eKeyState::RELEASE || state == eKeyState::DOWN)
        {
            auto &hotkey_map = m_hotkeys[state == eKeyState::RELEASE];
            if (const auto &it = hotkey_map.find(key); it != hotkey_map.end())
            {
                if (auto &hotkey = it->second; hotkey.can_exec())
                {
                    g_fiber_pool->queue_job([&hotkey]
                    {
                        hotkey.exec();
                    });
                }
            }
        }
    }
}