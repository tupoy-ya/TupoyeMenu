#include "fiber_pool.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "services/globals/globals_service.hpp"
#include "thread_pool.hpp"
#include "util/system.hpp"
#include "views/view.hpp"

namespace big
{
	void view::debug_globals() {
		if (ImGui::Checkbox("Enable Freezing", &g_globals_service->m_running) && g_globals_service->m_running)
			g_thread_pool->push([&]() { g_globals_service->loop(); });

		if (components::button("Load"))
			g_globals_service->load();
		ImGui::SameLine();
		if (components::button("Save"))
			g_globals_service->save();

		components::button("Network Bail", []
			{
				NETWORK::NETWORK_BAIL(16, 0, 0);
			});

		ImGui::SameLine();
		if (components::button("Add Global"))
		{
			ImGui::OpenPopup("New Global");
		}

		if (ImGui::BeginPopupModal("New Global"))
		{
			static int base_address = 0;
			static bool freeze = false;
			static char name[32] = "";
			static int(*offsets)[2] = nullptr;
			static int offset_count = 0;
			static int previous_offset_count = 0;

			g_fiber_pool->queue_job([] {
				PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
				});

			ImGui::Text("Name:");
			ImGui::InputText("##modal_global_name", name, sizeof(name));
			ImGui::Text("Base Address:");
			ImGui::InputInt("##modal_global_base_addr", &base_address);
			ImGui::Text("Freeze:");
			ImGui::Checkbox("##modal_global_freeze", &freeze);
			ImGui::Text("Number of Offsets:");
			ImGui::InputInt("##modal_offset_count", &offset_count);

			if (offset_count < 0) offset_count;
			else if (offset_count > 10) offset_count = 10;

			if (offset_count != previous_offset_count)
			{
				int(*new_offsets)[2] = new int[offset_count][2]{ 0 };
				memcpy(new_offsets, offsets, sizeof(int) * std::min(offset_count, previous_offset_count) * 2);

				delete[] offsets;
				offsets = new_offsets;

				previous_offset_count = offset_count;
			}

			ImGui::PushItemWidth(320.f);
			for (int i = 0; i < offset_count; i++)
			{
				char id[32];

				ImGui::Separator();

				ImGui::Text("Offset: %d", i + 1);

				sprintf(id, "##offset_%d", i);
				ImGui::InputInt(id, &offsets[i][0]);

				ImGui::Text("Size:");
				ImGui::SameLine();
				sprintf(id, "##size_%d", i);
				ImGui::InputInt(id, &offsets[i][1]);
			}
			ImGui::PopItemWidth();

			if (components::button("Cancel"))
			{
				strcpy(name, "");
				freeze = false;
				delete[] offsets;
				offsets = nullptr;
				offset_count = 0;
				previous_offset_count = 0;

				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (components::button("Save"))
			{
				auto new_global = global(name, base_address, freeze, offsets, offset_count);
				new_global.build_cache();

				g_globals_service->m_globals.push_back(new_global);

				strcpy(name, "");
				freeze = false;
				delete[] offsets;
				offsets = nullptr;
				offset_count = 0;
				previous_offset_count = 0;

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		for (auto& global : g_globals_service->m_globals)
		{
			char label[64];

			ImGui::Separator();

			sprintf(label, "Freeze##%d", global.get_id());
			ImGui::Checkbox(label, &global.m_freeze);

			ImGui::BeginGroup();

			ImGui::Text("Name:");
			ImGui::Text("Value:");

			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();

			ImGui::Text(global.m_name.c_str());

			sprintf(label, "###input_%d", global.get_id());
			ImGui::SetNextItemWidth(200.f);
			ImGui::InputInt(label, global.get());

			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();

			sprintf(label, "Delete##%d", global.get_id());
			if (components::button(label))
			{
				for (int i = 0; i < g_globals_service->m_globals.size(); i++)
					if (auto& it = g_globals_service->m_globals.at(i); it.get_id() == global.get_id())
						g_globals_service->m_globals.erase(g_globals_service->m_globals.begin() + i);

				break;
			}

			sprintf(label, "Write###%d", global.get_id());
			if (components::button(label))
				global.write();

			ImGui::EndGroup();
		}
	}
}