#pragma once

namespace parser {

	typedef unsigned char USER_VALUE_T;
	const USER_VALUE_T UT_STR = 0x00; // std::string
	const USER_VALUE_T UT_NUMBER = 0x01; // int64_t
	const USER_VALUE_T UT_UNSIGNED_BIG_NUMBER = 0x02; // не поместилось в int64_t, поэтому uint64_t
	const USER_VALUE_T UT_BOOL = 0x03; // bool
	const USER_VALUE_T UT_FLOAT = 0x04; // float

}