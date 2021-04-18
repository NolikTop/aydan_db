//
// Created by NolikTop on 17.04.2021.
//
#include <iostream>

#pragma once

namespace binary {

	const int CAP_X2 = 1024 * 1024 * 10; // 10mb

	struct Stream {
		size_t size;
		size_t offset;
		size_t cap;
		char *buffer;

		Stream(size_t size, char *buffer);

		explicit Stream(size_t cap = 24);

		~Stream();

		[[nodiscard]] inline char *read(size_t len) const;

		inline void write(size_t dataSize, char* data);
		inline void writeNoRelease(size_t dataSize, char* data);

		inline void writeNumber(size_t size, int64_t value);
		[[nodiscard]] inline int64_t readNumber(size_t size) const;

		inline void writeSignedByte(int8_t value);
		[[nodiscard]] inline int8_t readSignedByte() const;

		inline void writeUnsignedByte(u_int8_t value);
		[[nodiscard]] inline u_int8_t readUnsignedByte() const;

		inline void writeSignedShort(int16_t value);
		[[nodiscard]] int16_t inline readSignedShort() const;

		inline void writeUnsignedShort(u_int16_t value);
		[[nodiscard]] u_int16_t inline readUnsignedShort() const;

		inline void writeSignedInt32(int32_t value);
		u_int32_t inline readSignedInt32();

		inline void writeUnsignedInt32(u_int32_t value);
		u_int32_t inline readUnsignedInt32();

		inline void writeSignedLong(int64_t value);
		int64_t inline readSignedLong();

		inline void writeUnsignedLong(u_int64_t value);
		u_int64_t inline readUnsignedLong();

	};

	typedef struct Stream Stream;

}