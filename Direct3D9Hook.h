#ifndef _DIRECT3D_V9_HOOK_H_
#define _DIRECT3D_V9_HOOK_H_

#include "Direct3D9Wrapper.hpp"
#include <lemon/detour.hpp>
#include <sol.hpp>

using PresentPrototype = void( __stdcall* )( IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA* );

class Direct3D9Hook
{
	static Direct3D9Hook* self;

	sol::this_state _ts = nullptr;

	lemon::detour<PresentPrototype> _presentHook{ Direct3D9Wrapper::GetVTableFunctionAddress( 17 ) };

	Direct3D9Hook( sol::this_state ts );
	~Direct3D9Hook();
public:
	static void Initialize( sol::this_state ts );
	static void Uninitialize();
private:
	void PresentHook( PresentPrototype fpHkOriginal, IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA* );
}; // class Direct3D9Hook

#endif // !_DIRECT3D_V9_HOOK_H_