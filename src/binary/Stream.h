//
// Created by NolikTop on 17.04.2021.
//
#include <iostream>

#pragma once

typedef unsigned char byte;

namespace binary {

	const int CAP_X2 = 1024 * 1024 * 10; // 10mb

	struct Stream {
		size_t size;
		size_t offset;
		size_t cap;
		byte *buffer;

		Stream(size_t size, byte *buffer);

		explicit Stream(size_t cap = 24);

		~Stream();

		void clean(size_t capacity = 24);

		[[nodiscard]] std::string getBufferAsString() const;

		[[nodiscard]] byte *read(size_t valueSize);

		void writeByteString(const std::string& value);
		[[nodiscard]] std::string readByteString();

		void writeShortString(const std::string& value);
		[[nodiscard]] std::string readShortString();

		void write(size_t dataSize, byte* data);
		void writeNotReleaseValue(size_t valueSize, byte* value);

		void writeNumber(size_t valueSize, int64_t value);
		[[nodiscard]] int64_t readNumber(size_t valueSize);

		void writeUnsignedVarInt(u_int64_t number);
		u_int64_t readUnsignedVarInt();


		void checkForWrite(size_t valueSize);

		void writeSignedByte(int8_t value);
		[[nodiscard]] int8_t readSignedByte();

		void writeUnsignedByte(u_int8_t value);
		[[nodiscard]] u_int8_t readUnsignedByte();

		void writeSignedShort(int16_t value);
		[[nodiscard]] int16_t readSignedShort();

		void writeUnsignedShort(u_int16_t value);
		[[nodiscard]] u_int16_t readUnsignedShort();

		void writeSignedInt32(int32_t value);
		[[nodiscard]] int32_t readSignedInt32();

		void writeUnsignedInt32(u_int32_t value);
		[[nodiscard]] u_int32_t readUnsignedInt32();

		void writeSignedLong(int64_t value);
		[[nodiscard]] int64_t readSignedLong();

		void writeUnsignedLong(u_int64_t value);
		[[nodiscard]] u_int64_t readUnsignedLong();

	};

	typedef struct Stream Stream;

}