#pragma once
#include <string>
#include <iostream>
#include <type_traits>

/*
 * Writing a custom handler is easy too !
 * 
 * 1. Have a functor ready to use with the appropriate amount of arguments you'd expect
 * 2. Add a using declaration for BoolProxy<YourHandler>
 * 3. Add a using declaration for a verbose handler (if needed) with BoolProxy<YourHandler, policies::HandleVerbose>
 * 
 * Done.
 **/
namespace bool_proxy {
namespace detail {
inline namespace v1 {

namespace policies {

struct HandleOnFalse {
	bool should_handle(bool b) const noexcept { return !b; }
};
struct HandleVerbose {
	bool should_handle(bool b) const noexcept { return true; }
};

}

template <typename Handler, typename HandlePolicy = policies::HandleOnFalse>
struct BoolProxy : private HandlePolicy {
	template <typename... HandlerArgs>
	BoolProxy(bool b, HandlerArgs&&... args) {
		Handler handler{};
		if (should_handle(b)) { handler(std::forward<HandlerArgs>(args)...); }
	}
};

struct LoggingHandler {
	template <typename... Args>
	void operator()(std::string const& fileName, std::string const& funcName, unsigned lineNum, std::string const& msg = "", Args&&... args) {
		// Your logging functionality
		std::cout << fileName << "::" << funcName << "::" << lineNum << " " << msg << " ";
		log_args(std::forward<Args>(args)...);
	}

	template <typename... Args>
	void log_args(Args&&... args) {
		std::cout << std::endl;
	}
	template <typename First, typename... Rest>
	void log_args(First&& first, Rest&&... rest) {
		std::cout << first << " ";
		log_args(std::forward<Rest>(rest)...);
	}
};
using BoolProxyLogger = BoolProxy<LoggingHandler>;
using BoolProxyLoggerVerbose = BoolProxy<LoggingHandler, policies::HandleVerbose>;
#define CHECK_LOG(_bool_, ...) BoolProxyLogger(_bool_, __FILE__, __FUNCSIG__, __LINE__, __VA_ARGS__) 
#define CHECK_LOG_VERBOSE(_bool_, ...) BoolProxyLoggerVerbose(_bool_, __FILE__, __FUNCSIG__, __LINE__, __VA_ARGS__) 


struct CustomHandler {
	template <typename Func>
	void operator()(Func&& func) {
		func();
	}
};
using BoolCustomHandler = BoolProxy<CustomHandler>;
using BoolCustomHandlerVerbose = BoolProxy<CustomHandler, policies::HandleVerbose>;
#define CHECK_CUSTOM(_bool_, _func_, ...) BoolCustomHandler(_bool_, []() { _func_(__VA_ARGS__);  })
#define CHECK_CUSTOM_VERBOSE(_bool_, _func_, ...) BoolCustomHandlerVerbose(_bool_, []() { _func_(__VA_ARGS__);  })

}
}
}