#ifndef _DIRECT3D_V9_WRAPPER_H_
#define _DIRECT3D_V9_WRAPPER_H_

#include <cstddef>
#include <cstdint>
#include <string>
#include <d3d9.h>

class Direct3D9Wrapper
{
public:
    static std::uintptr_t GetVTableFunctionAddress( std::uint32_t VTableIndex ) {
        return (*reinterpret_cast<std::uintptr_t**>(FindDevice(0x128000)))[VTableIndex];
    }
private:
    static std::uintptr_t FindDevice( std::uint32_t Len ) {
        static std::uintptr_t base = []( std::size_t Len ) {
            std::string path_to( MAX_PATH, '\0' );
            if ( auto size = GetSystemDirectoryA( path_to.data(), MAX_PATH ) ) {
                path_to.resize( size );
                path_to += "\\d3d9.dll";
                std::uintptr_t dwObjBase = reinterpret_cast<std::uintptr_t>( LoadLibraryA( path_to.c_str() ) );
                // Pattern: C7 06 ?? ?? ?? ?? 89 86 ?? ?? ?? ?? 89 86
                while ( dwObjBase++ < dwObjBase + Len ) {
                    if (*reinterpret_cast<std::uint16_t*>( dwObjBase + 0x00 ) == 0x06C7 &&
                        *reinterpret_cast<std::uint16_t*>( dwObjBase + 0x06 ) == 0x8689 &&
                        *reinterpret_cast<std::uint16_t*>( dwObjBase + 0x0C ) == 0x8689) {
                        dwObjBase += 2;
                        break;
                    }
                }
                return dwObjBase;
            }
            return std::uintptr_t( 0 );
        }( Len );
        return base;
    }
}; // class Direct3D9Wrapper

#endif // !_DIRECT3D_V9_WRAPPER_H_