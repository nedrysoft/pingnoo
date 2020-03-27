#ifndef CATCH_EXTENSIONS_HPP
#define CATCH_EXTENSIONS_HPP

#include "catch/catch.hpp"

#define CHECK_MESSAGE(cond, msg) do { INFO(msg); CHECK(cond); } while((void)0, 0)
#define REQUIRE_MESSAGE(cond, msg) do { INFO(msg); REQUIRE(cond); } while((void)0, 0)


#endif // CATCH_EXTENSIONS_HPP
