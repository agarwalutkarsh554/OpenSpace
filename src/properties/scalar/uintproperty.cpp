/*****************************************************************************************
 *                                                                                       *
 * OpenSpace                                                                             *
 *                                                                                       *
 * Copyright (c) 2014-2021                                                               *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this  *
 * software and associated documentation files (the "Software"), to deal in the Software *
 * without restriction, including without limitation the rights to use, copy, modify,    *
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to    *
 * permit persons to whom the Software is furnished to do so, subject to the following   *
 * conditions:                                                                           *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all copies *
 * or substantial portions of the Software.                                              *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,   *
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A         *
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT    *
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF  *
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  *
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                         *
 ****************************************************************************************/

#include <openspace/properties/scalar/uintproperty.h>

#include <ghoul/lua/ghoul_lua.h>

#include <limits>
#include <sstream>

namespace {

unsigned int fromLuaConversion(lua_State* state, bool& success) {
    success = (lua_isnumber(state, -1) == 1);
    if (success) {
        unsigned int val = static_cast<unsigned int>(lua_tonumber(state, -1));
        return val;
    }
    else {
        return 0;
    }
}

bool toLuaConversion(lua_State* state, unsigned int value) {
    lua_pushnumber(state, static_cast<lua_Number>(value));
    return true;
}

unsigned int fromStringConversion(const std::string& val, bool& success) {
    std::stringstream s(val);
    unsigned int v;
    s >> v;
    success = !s.fail();
    if (success) {
        return v;
    }
    else {
        throw ghoul::RuntimeError("Conversion error for string: " + val);
    }
}

bool toStringConversion(std::string& outValue, unsigned int inValue) {
    outValue = std::to_string(inValue);
    return true;
}

} // namespace

namespace openspace::properties {

REGISTER_NUMERICALPROPERTY_SOURCE(
    UCharProperty,
    unsigned int,
    0,
    std::numeric_limits<unsigned int>::lowest(),
    std::numeric_limits<unsigned int>::max(),
    1,
    fromLuaConversion,
    toLuaConversion,
    fromStringConversion,
    toStringConversion,
    LUA_TNUMBER
)

} // namespace openspace::properties
