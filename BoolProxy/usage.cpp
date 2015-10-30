#include <string>
#include <iostream>
#include "bool_proxy.h"
#include <exception>

/*
 * Does your codebase include logs or traces ?
 * Do you often see code like:
 *	if(!returnsBoolConvertible()) {
 *		LOGGING::LOG_TO_FILE("returnsBoolConvertible failed with ", getError());
 *		TRACER::TRACE("returnsBoolConvertible failed with ", getError());
 *	}
 * I know I do...
 * Let's make this better, shall we ?
 * How about -
 * CHECK_LOG(returnsBoolConvertible());
 * Hm... But I need a more advanced logic on this
 * Well why not -
 * CHECK_CUSTOM(returnsBoolConvertible(), [](auto firstThing, auto secondThing) { // Logic });
 * 
 * Much nicer, isn't ?
 **/
using namespace bool_proxy::detail;

int main() {
	CHECK_LOG(false, "I will be printed because the first expression is false");
	CHECK_LOG(true, "I will not be printed because the first expression is true");
	CHECK_LOG_VERBOSE(false, "I am verbose and I'll be printed either way");
	CHECK_LOG_VERBOSE(true, "I am verbose and I'll be printed either way");
	CHECK_CUSTOM(false, [](auto&&... datas) {
		std::cout << "This is a custom handler with " << sizeof...(datas) << " args" << std::endl;
	}, 1, 2, 3);
	CHECK_GET_LAST_ERROR(false, "I will print GetLastError() as well as this string");
	CHECK_TRHOW(false, std::logic_error, "0xDEADBEEF");
}