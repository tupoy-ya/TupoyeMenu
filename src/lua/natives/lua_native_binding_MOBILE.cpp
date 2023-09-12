#include "lua_native_binding.hpp"
#include "natives.hpp"

namespace lua::native
{
	static void LUA_NATIVE_MOBILE_CREATE_MOBILE_PHONE(int phoneType)
	{
		MOBILE::CREATE_MOBILE_PHONE(phoneType);
	}

	static void LUA_NATIVE_MOBILE_DESTROY_MOBILE_PHONE()
	{
		MOBILE::DESTROY_MOBILE_PHONE();
	}

	static void LUA_NATIVE_MOBILE_SET_MOBILE_PHONE_SCALE(float scale)
	{
		MOBILE::SET_MOBILE_PHONE_SCALE(scale);
	}

	static void LUA_NATIVE_MOBILE_SET_MOBILE_PHONE_ROTATION(float rotX, float rotY, float rotZ, Any p3)
	{
		MOBILE::SET_MOBILE_PHONE_ROTATION(rotX, rotY, rotZ, p3);
	}

	static Vector3 LUA_NATIVE_MOBILE_GET_MOBILE_PHONE_ROTATION(Vector3 rotation, Vehicle p1)
	{
		MOBILE::GET_MOBILE_PHONE_ROTATION(&rotation, p1);
		return rotation;
	}

	static void LUA_NATIVE_MOBILE_SET_MOBILE_PHONE_POSITION(float posX, float posY, float posZ)
	{
		MOBILE::SET_MOBILE_PHONE_POSITION(posX, posY, posZ);
	}

	static Vector3 LUA_NATIVE_MOBILE_GET_MOBILE_PHONE_POSITION(Vector3 position)
	{
		MOBILE::GET_MOBILE_PHONE_POSITION(&position);
		return position;
	}

	static void LUA_NATIVE_MOBILE_SCRIPT_IS_MOVING_MOBILE_PHONE_OFFSCREEN(bool toggle)
	{
		MOBILE::SCRIPT_IS_MOVING_MOBILE_PHONE_OFFSCREEN(toggle);
	}

	static bool LUA_NATIVE_MOBILE_CAN_PHONE_BE_SEEN_ON_SCREEN()
	{
		auto retval = (bool)MOBILE::CAN_PHONE_BE_SEEN_ON_SCREEN();
		return retval;
	}

	static void LUA_NATIVE_MOBILE_SET_MOBILE_PHONE_DOF_STATE(bool toggle)
	{
		MOBILE::SET_MOBILE_PHONE_DOF_STATE(toggle);
	}

	static void LUA_NATIVE_MOBILE_CELL_SET_INPUT(int direction)
	{
		MOBILE::CELL_SET_INPUT(direction);
	}

	static void LUA_NATIVE_MOBILE_CELL_HORIZONTAL_MODE_TOGGLE(bool toggle)
	{
		MOBILE::CELL_HORIZONTAL_MODE_TOGGLE(toggle);
	}

	static void LUA_NATIVE_MOBILE_CELL_CAM_ACTIVATE(bool p0, bool p1)
	{
		MOBILE::CELL_CAM_ACTIVATE(p0, p1);
	}

	static void LUA_NATIVE_MOBILE_CELL_CAM_ACTIVATE_SELFIE_MODE(bool toggle)
	{
		MOBILE::CELL_CAM_ACTIVATE_SELFIE_MODE(toggle);
	}

	static void LUA_NATIVE_MOBILE_CELL_CAM_ACTIVATE_SHALLOW_DOF_MODE(bool toggle)
	{
		MOBILE::CELL_CAM_ACTIVATE_SHALLOW_DOF_MODE(toggle);
	}

	static void LUA_NATIVE_MOBILE_CELL_CAM_SET_SELFIE_MODE_SIDE_OFFSET_SCALING(float p0)
	{
		MOBILE::CELL_CAM_SET_SELFIE_MODE_SIDE_OFFSET_SCALING(p0);
	}

	static void LUA_NATIVE_MOBILE_CELL_CAM_SET_SELFIE_MODE_HORZ_PAN_OFFSET(float horizontalPan)
	{
		MOBILE::CELL_CAM_SET_SELFIE_MODE_HORZ_PAN_OFFSET(horizontalPan);
	}

	static void LUA_NATIVE_MOBILE_CELL_CAM_SET_SELFIE_MODE_VERT_PAN_OFFSET(float vertPan)
	{
		MOBILE::CELL_CAM_SET_SELFIE_MODE_VERT_PAN_OFFSET(vertPan);
	}

	static void LUA_NATIVE_MOBILE_CELL_CAM_SET_SELFIE_MODE_ROLL_OFFSET(float roll)
	{
		MOBILE::CELL_CAM_SET_SELFIE_MODE_ROLL_OFFSET(roll);
	}

	static void LUA_NATIVE_MOBILE_CELL_CAM_SET_SELFIE_MODE_DISTANCE_SCALING(float distanceScaling)
	{
		MOBILE::CELL_CAM_SET_SELFIE_MODE_DISTANCE_SCALING(distanceScaling);
	}

	static void LUA_NATIVE_MOBILE_CELL_CAM_SET_SELFIE_MODE_HEAD_YAW_OFFSET(float yaw)
	{
		MOBILE::CELL_CAM_SET_SELFIE_MODE_HEAD_YAW_OFFSET(yaw);
	}

	static void LUA_NATIVE_MOBILE_CELL_CAM_SET_SELFIE_MODE_HEAD_ROLL_OFFSET(float roll)
	{
		MOBILE::CELL_CAM_SET_SELFIE_MODE_HEAD_ROLL_OFFSET(roll);
	}

	static void LUA_NATIVE_MOBILE_CELL_CAM_SET_SELFIE_MODE_HEAD_PITCH_OFFSET(float pitch)
	{
		MOBILE::CELL_CAM_SET_SELFIE_MODE_HEAD_PITCH_OFFSET(pitch);
	}

	static bool LUA_NATIVE_MOBILE_CELL_CAM_IS_CHAR_VISIBLE_NO_FACE_CHECK(Entity entity)
	{
		auto retval = (bool)MOBILE::CELL_CAM_IS_CHAR_VISIBLE_NO_FACE_CHECK(entity);
		return retval;
	}

	static int LUA_NATIVE_MOBILE_GET_MOBILE_PHONE_RENDER_ID(int renderId)
	{
		MOBILE::GET_MOBILE_PHONE_RENDER_ID(&renderId);
		return renderId;
	}

	void init_native_binding_MOBILE(sol::state& L)
	{
		auto MOBILE = L["MOBILE"].get_or_create<sol::table>();
		MOBILE.set_function("CREATE_MOBILE_PHONE", LUA_NATIVE_MOBILE_CREATE_MOBILE_PHONE);
		MOBILE.set_function("DESTROY_MOBILE_PHONE", LUA_NATIVE_MOBILE_DESTROY_MOBILE_PHONE);
		MOBILE.set_function("SET_MOBILE_PHONE_SCALE", LUA_NATIVE_MOBILE_SET_MOBILE_PHONE_SCALE);
		MOBILE.set_function("SET_MOBILE_PHONE_ROTATION", LUA_NATIVE_MOBILE_SET_MOBILE_PHONE_ROTATION);
		MOBILE.set_function("GET_MOBILE_PHONE_ROTATION", LUA_NATIVE_MOBILE_GET_MOBILE_PHONE_ROTATION);
		MOBILE.set_function("SET_MOBILE_PHONE_POSITION", LUA_NATIVE_MOBILE_SET_MOBILE_PHONE_POSITION);
		MOBILE.set_function("GET_MOBILE_PHONE_POSITION", LUA_NATIVE_MOBILE_GET_MOBILE_PHONE_POSITION);
		MOBILE.set_function("SCRIPT_IS_MOVING_MOBILE_PHONE_OFFSCREEN", LUA_NATIVE_MOBILE_SCRIPT_IS_MOVING_MOBILE_PHONE_OFFSCREEN);
		MOBILE.set_function("CAN_PHONE_BE_SEEN_ON_SCREEN", LUA_NATIVE_MOBILE_CAN_PHONE_BE_SEEN_ON_SCREEN);
		MOBILE.set_function("SET_MOBILE_PHONE_DOF_STATE", LUA_NATIVE_MOBILE_SET_MOBILE_PHONE_DOF_STATE);
		MOBILE.set_function("CELL_SET_INPUT", LUA_NATIVE_MOBILE_CELL_SET_INPUT);
		MOBILE.set_function("CELL_HORIZONTAL_MODE_TOGGLE", LUA_NATIVE_MOBILE_CELL_HORIZONTAL_MODE_TOGGLE);
		MOBILE.set_function("CELL_CAM_ACTIVATE", LUA_NATIVE_MOBILE_CELL_CAM_ACTIVATE);
		MOBILE.set_function("CELL_CAM_ACTIVATE_SELFIE_MODE", LUA_NATIVE_MOBILE_CELL_CAM_ACTIVATE_SELFIE_MODE);
		MOBILE.set_function("CELL_CAM_ACTIVATE_SHALLOW_DOF_MODE", LUA_NATIVE_MOBILE_CELL_CAM_ACTIVATE_SHALLOW_DOF_MODE);
		MOBILE.set_function("CELL_CAM_SET_SELFIE_MODE_SIDE_OFFSET_SCALING", LUA_NATIVE_MOBILE_CELL_CAM_SET_SELFIE_MODE_SIDE_OFFSET_SCALING);
		MOBILE.set_function("CELL_CAM_SET_SELFIE_MODE_HORZ_PAN_OFFSET", LUA_NATIVE_MOBILE_CELL_CAM_SET_SELFIE_MODE_HORZ_PAN_OFFSET);
		MOBILE.set_function("CELL_CAM_SET_SELFIE_MODE_VERT_PAN_OFFSET", LUA_NATIVE_MOBILE_CELL_CAM_SET_SELFIE_MODE_VERT_PAN_OFFSET);
		MOBILE.set_function("CELL_CAM_SET_SELFIE_MODE_ROLL_OFFSET", LUA_NATIVE_MOBILE_CELL_CAM_SET_SELFIE_MODE_ROLL_OFFSET);
		MOBILE.set_function("CELL_CAM_SET_SELFIE_MODE_DISTANCE_SCALING", LUA_NATIVE_MOBILE_CELL_CAM_SET_SELFIE_MODE_DISTANCE_SCALING);
		MOBILE.set_function("CELL_CAM_SET_SELFIE_MODE_HEAD_YAW_OFFSET", LUA_NATIVE_MOBILE_CELL_CAM_SET_SELFIE_MODE_HEAD_YAW_OFFSET);
		MOBILE.set_function("CELL_CAM_SET_SELFIE_MODE_HEAD_ROLL_OFFSET", LUA_NATIVE_MOBILE_CELL_CAM_SET_SELFIE_MODE_HEAD_ROLL_OFFSET);
		MOBILE.set_function("CELL_CAM_SET_SELFIE_MODE_HEAD_PITCH_OFFSET", LUA_NATIVE_MOBILE_CELL_CAM_SET_SELFIE_MODE_HEAD_PITCH_OFFSET);
		MOBILE.set_function("CELL_CAM_IS_CHAR_VISIBLE_NO_FACE_CHECK", LUA_NATIVE_MOBILE_CELL_CAM_IS_CHAR_VISIBLE_NO_FACE_CHECK);
		MOBILE.set_function("GET_MOBILE_PHONE_RENDER_ID", LUA_NATIVE_MOBILE_GET_MOBILE_PHONE_RENDER_ID);
	}
}
