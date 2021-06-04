
#pragma once

namespace db {

	typedef unsigned char COLUMN_T;

	const COLUMN_T CT_UNKNOWN = 0x00;
	const COLUMN_T CT_NUMBER = 0x01; // signed int32
	// от остальных типов я отказался - банально не успею доделать иначе
	const COLUMN_T CT_STRING = 0x02;

}