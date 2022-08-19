#include <Windows.h>
#include <sol.hpp>
#include "Direct3D9Hook.h"

void DefineDirect3D9HookFunctions(sol::this_state ts, sol::table& module ) {
	Direct3D9Hook::Initialize( ts );
	module.set_function( "Uninitialize", &Direct3D9Hook::Uninitialize ); // unwork cuz dll pinned -> check dllmain
}

sol::table open( sol::this_state ts ) {
	sol::state_view lua( ts );
	sol::table module = lua.create_table();
	module["VERSION"] = "1.0";
	
	DefineDirect3D9HookFunctions( ts, module );

	return module;
}

extern "C" __declspec(dllexport) int luaopen_Direct3D9HookDll(lua_State * LuaState) {
	return (sol::c_call<decltype(&(open)), &(open)>)(LuaState);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		// pin DLL to prevent unloading
		HMODULE module;
		GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_PIN, reinterpret_cast<LPCWSTR>(&DllMain), &module);
	}
	return TRUE;
}