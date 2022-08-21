#include "Direct3D9Hook.h"

Direct3D9Hook* Direct3D9Hook::self{ nullptr };

Direct3D9Hook::Direct3D9Hook( sol::this_state ts ) : _ts( ts ) {
	presentHook.before += [this]( auto&&... args ) { return Direct3D9Hook::PresentHooked( args... ); };
	presentHook.install();
	resetHook.before += [this]( auto&&... args ) { return Direct3D9Hook::ResetHooked( args... ); };
	resetHook.after += [this]( auto&&... args ) { return Direct3D9Hook::LostHooked( args... ); };
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

std::optional<HRESULT> Direct3D9Hook::PresentHooked( const decltype( presentHook )& hook, IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA* ) {
	sol::state_view lua{ _ts };
	if ( lua["OnPresent"].valid() )
		lua["OnPresent"].call();
	return std::nullopt;
}

std::optional<HRESULT> Direct3D9Hook::ResetHooked( const decltype( resetHook )& hook, IDirect3DDevice9*, D3DPRESENT_PARAMETERS* ) {
	sol::state_view lua{ self->_ts };
	if ( lua["OnReset"].valid() )
		lua["OnReset"].call();
	return std::nullopt;
}

void Direct3D9Hook::LostHooked( const decltype( resetHook )& hook, HRESULT&, IDirect3DDevice9*, D3DPRESENT_PARAMETERS* ) {
	sol::state_view lua{ self->_ts };
	if ( lua["OnLost"].valid() )
		lua["OnLost"].call();
}