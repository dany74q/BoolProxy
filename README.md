# BoolProxy
A utility for operating on boolean (or bool-convertible) values. Useful for logging and alike.
BoolProxy tries to tackle all of those "if(!success) { log to file, trace, GetLastError, throw, etc... }"
It does so with an extensible interface that everybody can write quickly to.
Use variations of this to clean up your code and make each function as clear as possible.

See usage.cpp for examples.