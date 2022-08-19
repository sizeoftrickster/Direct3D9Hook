#ifndef _DIRECT3D_V9_HOOK_H_
#define _DIRECT3D_V9_HOOK_H_

#include "Direct3D9Wrapper.hpp"
#include <sol.hpp>
#include <kthook/kthook.hpp>

using PresentPrototype = HRESULT( __stdcall* )( IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA* );
using ResetPrototype = HRESULT( __stdcall* )( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );

class Direct3D9Hook
{
	static Direct3D9Hook* self;
	sol::this_state _ts = nullptr;

	Direct3D9Hook( sol::this_state ts );
	~Direct3D9Hook();
public:
	static void Initialize( sol::this_state ts );
	static void Uninitialize();

	static void CallFunctionPresent();
	static void CallFunctionReset();
	static void CallFunctionLost();
}; // class Direct3D9Hook

#endif // !_DIRECT3D_V9_HOOK_H_