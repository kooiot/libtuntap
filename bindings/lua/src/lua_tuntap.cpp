#include <tuntap++.hh>

#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

#define LIB_PLC_TAG_VERSION TUNTAP_VERSION

namespace tuntap {
	sol::table open_module(sol::this_state L) {
		sol::state_view lua(L);
		sol::table module = lua.create_table();

		module.new_enum("ID",
			"MAX", TUNTAP_ID_MAX,
			"ANY", TUNTAP_ID_ANY
		);

		module.new_enum("MODE",
			"ETHERNET", TUNTAP_MODE_ETHERNET,
			"TUNNEL", TUNTAP_MODE_TUNNEL,
			"PERSIST", TUNTAP_MODE_PERSIST
		);

		module.new_enum("LOG",
			"NONE", TUNTAP_LOG_NONE,
			"DEBUG", TUNTAP_LOG_DEBUG,
			"INFO", TUNTAP_LOG_INFO,
			"NOTICE", TUNTAP_LOG_NOTICE,
			"WARN", TUNTAP_LOG_WARN,
			"ERROR", TUNTAP_LOG_ERR
		);

		module.new_usertype<tun>("tun",
			sol::call_constructor, sol::constructors<sol::types<>>(),
			"name", sol::overload(
				static_cast<std::string (tun::*)() const>(&tun::name),
				static_cast<void (tun::*)(std::string const &)>(&tun::name)
			),
			"mtu", sol::overload(
				static_cast<int (tun::*)() const>(&tun::mtu),
				static_cast<void (tun::*)(int)>(&tun::mtu)
			),
			"native_handle", &tun::native_handle,

			"up", &tun::up,
			"down", &tun::down,
			"ip", &tun::ip,

			"read", &tun::read,
			"write", &tun::write,

			"release", &tun::release,
			"nonblocking", &tun::nonblocking
		);


		module.new_usertype<tap>("tap",
			sol::call_constructor, sol::constructors<sol::types<>>(),
			"name", sol::overload(
				static_cast<std::string (tap::*)() const>(&tap::name),
				static_cast<void (tap::*)(std::string const &)>(&tap::name)
			),
			"hwaddr", sol::overload(
				static_cast<std::string (tap::*)() const>(&tap::hwaddr),
				static_cast<void (tap::*)(std::string const &)>(&tap::hwaddr)
			),
			"mtu", sol::overload(
				static_cast<int (tap::*)() const>(&tap::mtu),
				static_cast<void (tap::*)(int)>(&tap::mtu)
			),
			"native_handle", &tap::native_handle,

			"up", &tap::up,
			"down", &tap::down,
			"ip", &tap::ip,

			"read", &tap::read,
			"write", &tap::write,

			"release", &tap::release,
			"nonblocking", &tap::nonblocking
		);

		// Sol2 will wrap the NULL to nil automatically
		//module.set("null", PLC_TAG_NULL);

		/**
		 * Module version
		 */
		module["VERSION"] = LIB_PLC_TAG_VERSION;

		return module;
	}
}

extern "C" int luaopen_tuntap(lua_State *L) {
	return sol::stack::call_lua(L, 1, tuntap::open_module);
}
