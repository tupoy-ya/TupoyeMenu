#if defined (ENABLE_LUA)

#include "lua_native_binding.hpp"
#include "natives.hpp"

namespace lua::native
{
	static bool LUA_NATIVE_SAVEMIGRATION_SAVEMIGRATION_IS_MP_ENABLED()
	{
		auto retval = (bool)SAVEMIGRATION::SAVEMIGRATION_IS_MP_ENABLED();
		return retval;
	}

	static bool LUA_NATIVE_SAVEMIGRATION_SAVEMIGRATION_MP_REQUEST_ACCOUNTS()
	{
		auto retval = (bool)SAVEMIGRATION::SAVEMIGRATION_MP_REQUEST_ACCOUNTS();
		return retval;
	}

	static int LUA_NATIVE_SAVEMIGRATION_SAVEMIGRATION_MP_GET_ACCOUNTS_STATUS()
	{
		auto retval = SAVEMIGRATION::SAVEMIGRATION_MP_GET_ACCOUNTS_STATUS();
		return retval;
	}

	static int LUA_NATIVE_SAVEMIGRATION_SAVEMIGRATION_MP_NUM_ACCOUNTS()
	{
		auto retval = SAVEMIGRATION::SAVEMIGRATION_MP_NUM_ACCOUNTS();
		return retval;
	}

	static std::tuple<bool, Any> LUA_NATIVE_SAVEMIGRATION_SAVEMIGRATION_MP_GET_ACCOUNT(int p0, Any p1)
	{
		std::tuple<bool, Any> return_values;
		std::get<0>(return_values) = (bool)SAVEMIGRATION::SAVEMIGRATION_MP_GET_ACCOUNT(p0, &p1);
		std::get<1>(return_values) = p1;

		return return_values;
	}

	static bool LUA_NATIVE_SAVEMIGRATION_SAVEMIGRATION_MP_REQUEST_STATUS()
	{
		auto retval = (bool)SAVEMIGRATION::SAVEMIGRATION_MP_REQUEST_STATUS();
		return retval;
	}

	static int LUA_NATIVE_SAVEMIGRATION_SAVEMIGRATION_MP_GET_STATUS()
	{
		auto retval = SAVEMIGRATION::SAVEMIGRATION_MP_GET_STATUS();
		return retval;
	}

	void init_native_binding_SAVEMIGRATION(sol::state& L)
	{
		auto SAVEMIGRATION = L["SAVEMIGRATION"].get_or_create<sol::table>();
		SAVEMIGRATION.set_function("SAVEMIGRATION_IS_MP_ENABLED", LUA_NATIVE_SAVEMIGRATION_SAVEMIGRATION_IS_MP_ENABLED);
		SAVEMIGRATION.set_function("SAVEMIGRATION_MP_REQUEST_ACCOUNTS", LUA_NATIVE_SAVEMIGRATION_SAVEMIGRATION_MP_REQUEST_ACCOUNTS);
		SAVEMIGRATION.set_function("SAVEMIGRATION_MP_GET_ACCOUNTS_STATUS", LUA_NATIVE_SAVEMIGRATION_SAVEMIGRATION_MP_GET_ACCOUNTS_STATUS);
		SAVEMIGRATION.set_function("SAVEMIGRATION_MP_NUM_ACCOUNTS", LUA_NATIVE_SAVEMIGRATION_SAVEMIGRATION_MP_NUM_ACCOUNTS);
		SAVEMIGRATION.set_function("SAVEMIGRATION_MP_GET_ACCOUNT", LUA_NATIVE_SAVEMIGRATION_SAVEMIGRATION_MP_GET_ACCOUNT);
		SAVEMIGRATION.set_function("SAVEMIGRATION_MP_REQUEST_STATUS", LUA_NATIVE_SAVEMIGRATION_SAVEMIGRATION_MP_REQUEST_STATUS);
		SAVEMIGRATION.set_function("SAVEMIGRATION_MP_GET_STATUS", LUA_NATIVE_SAVEMIGRATION_SAVEMIGRATION_MP_GET_STATUS);
	}
}

#endif // ENABLE_LUA
