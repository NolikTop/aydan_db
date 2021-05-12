
#pragma once

namespace db {

	typedef unsigned char COLUMN_T;

	const COLUMN_T CT_UNKNOWN = 0x00;
	const COLUMN_T CT_SIGNED_BYTE = 0x01;
	const COLUMN_T CT_UNSIGNED_BYTE = 0x02;
	const COLUMN_T CT_SIGNED_SHORT = 0x03;
	const COLUMN_T CT_UNSIGNED_SHORT = 0x04;
	const COLUMN_T CT_SIGNED_INT32 = 0x05;
	const COLUMN_T CT_UNSIGNED_INT32 = 0x06;
	const COLUMN_T CT_SIGNED_LONG = 0x07;
	const COLUMN_T CT_UNSIGNED_LONG = 0x08;
	const COLUMN_T CT_BOOL = 0x09;
	const COLUMN_T CT_FLOAT = 0x0a;
	const COLUMN_T CT_BYTE_SIZED_STRING = 0x10;
	const COLUMN_T CT_SHORT_SIZED_STRING = 0x11;
	const COLUMN_T CT_INT32_SIZED_STRING = 0x12;

	bool equals(const COLUMN_T &a, const COLUMN_T &b) {
		if (
			a >= CT_BYTE_SIZED_STRING && a <= CT_INT32_SIZED_STRING
			&&
			b >= CT_BYTE_SIZED_STRING && b <= CT_INT32_SIZED_STRING
		) {
			return true;
		}
		return a == b;
	}

}