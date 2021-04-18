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
		inline void writeNotReleaseValue(size_t valueSize, char* value);

		inline void writeNumber(size_t valueSize, int64_t value);
		[[nodiscard]] int64_t inline readNumber(size_t valueSize) const;

		inline void checkForWrite(size_t valueSize);

		inline void writeSignedByte(int8_t value);
		[[nodiscard]] int8_t inline readSignedByte() const;

		inline void writeUnsignedByte(u_int8_t value);
		[[nodiscard]] u_int8_t inline readUnsignedByte() const;

		inline void writeSignedShort(int16_t value);
		[[nodiscard]] int16_t inline readSignedShort() const;

		inline void writeUnsignedShort(u_int16_t value);
		[[nodiscard]] u_int16_t inline readUnsignedShort() const;

		inline void writeSignedInt32(int32_t value);
		[[nodiscard]] u_int32_t inline readSignedInt32() const;

		inline void writeUnsignedInt32(u_int32_t value);
		[[nodiscard]] u_int32_t inline readUnsignedInt32() const;

		inline void writeSignedLong(int64_t value);
		[[nodiscard]] int64_t inline readSignedLong() const;

		inline void writeUnsignedLong(u_int64_t value);
		[[nodiscard]] u_int64_t inline readUnsignedLong() const;

	};

	typedef struct Stream Stream;

}