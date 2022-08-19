#include "Direct3D9Hook.h"

Direct3D9Hook* Direct3D9Hook::self{ nullptr };

kthook::kthook_signal<PresentPrototype> presentHook{ Direct3D9Wrapper::GetVTableFunctionAddress( 17 ) };
kthook::kthook_signal<ResetPrototype> resetHook{ Direct3D9Wrapper::GetVTableFunctionAddress( 16 ) };

std::optional<HRESULT> PresentHooked( const decltype( presentHook )& hook, IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA* ) {
	Direct3D9Hook::CallFunctionPresent();
	return std::nullopt;
}

std::optional<HRESULT> ResetHooked( const decltype( resetHook )& hook, IDirect3DDevice9* , D3DPRESENT_PARAMETERS* ) {
	Direct3D9Hook::CallFunctionReset();
	return std::nullopt;
}

void LostHooked( const decltype( resetHook )& hook, HRESULT&, IDirect3DDevice9*, D3DPRESENT_PARAMETERS* ) {
	Direct3D9Hook::CallFunctionLost();
}

Direct3D9Hook::Direct3D9Hook( sol::this_state ts ) : _ts( ts ) {
	presentHook.before += &PresentHooked;
	presentHook.install();
	resetHook.before += &ResetHooked;
	resetHook.after += &LostHooked;
	resetHook.install();
}

Direct3D9Hook::~Direct3D9Hook() {
	resetHook.remove();
	presentHook.remove();
}

void Direct3D9Hook::Initialize( sol::this_state ts ) {
	if ( self )
		return;
	self = new Direct3D9Hook( ts );
}

void Direct3D9Hook::Uninitialize() {
	if ( self ) {
		delete self;
		self = nullptr;
	}
}

void Direct3D9Hook::CallFunctionPresent() {
	sol::state_view lua{ self->_ts };
	if ( lua["OnPresent"].valid() )
		lua["OnPresent"].call();
}

void Direct3D9Hook::CallFunctionReset() {
	sol::state_view lua{ self->_ts };
	if ( lua["OnReset"].valid() )
		lua["OnReset"].call();
}

void Direct3D9Hook::CallFunctionLost() {
	sol::state_view lua{ self->_ts };
	if ( lua["OnLost"].valid() )
		lua["OnLost"].call();
}