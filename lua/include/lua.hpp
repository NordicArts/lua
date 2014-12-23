// lua.hpp
// Lua header files for C++
// <<extern "C">> not supplied automatically because Lua also compiles as C++

extern "C" {
    #include <lua/include/lua.h>
    #include <lua/include/lualib.h>
    #include <lua/include/lauxlib.h>
}
