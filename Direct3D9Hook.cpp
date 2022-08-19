#include "Direct3D9Hook.h"

Direct3D9Hook* Direct3D9Hook::self{ nullptr };

Direct3D9Hook::Direct3D9Hook( sol::this_state ts ) : _ts(ts) {
	_presentHook.install( std::make_tuple( this, &Direct3D9Hook::PresentHook ) );
}
 
Direct3D9Hook::~Direct3D9Hook() {
	_presentHook.remove();
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

void Direct3D9Hook::PresentHook( PresentPrototype fpHkOriginal, IDirect3DDevice9* a, CONST RECT* b, CONST RECT* c, HWND d, CONST RGNDATA* e ) {
	sol::state_view lua{ _ts };
	lua["CustomPresent"].call();
	return fpHkOriginal( a, b, c, d, e );
}