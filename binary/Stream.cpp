//
// Created by NolikTop on 17.04.2021.
//

#include "Stream.h"
#include "Exception.h"
#include <string>
#include <initializer_list>

using namespace binary;

Stream::Stream(size_t size, char *buffer) : size(size), offset(0), cap(size), buffer(buffer) {}

Stream::Stream(size_t cap) : size(0), offset(0), cap(cap), buffer(new char[cap]){}

Stream::~Stream() {
	delete[] buffer;
}

char *Stream::read(size_t len) const {
	if(offset + len > size){
		throw Exception("Cant read more information than exists. Offset=" + std::to_string(offset) + " Len=" + std::to_string(len) + " Size=" + std::to_string(size));
	}
	return buffer + len;
}

void Stream::write(size_t dataSize, char* data) {
	writeNotReleaseValue(dataSize, data);
	delete[] data;
}

void Stream::writeNotReleaseValue(size_t valueSize, char *value) {
	checkForWrite(valueSize);

	memcpy(buffer + offset, value, valueSize);

	offset += valueSize;
}

void Stream::writeNumber(size_t valueSize, int64_t value) {
	if(valueSize > 8){
		throw binary::Exception("Wrong valueSize");
	}

	checkForWrite(valueSize);

	char* buf = buffer + offset;

	// > 0, а не >= 0, потому что иначе происходит integer overflow
	for(size_t j = valueSize - 1, i = j * 8; i > 0; i -= 8, j--){
		buf[j] = (char)((value >> i) & 0xFF);
	}

	buf[0] = (char)(value & 0xFF);
}

int64_t Stream::readNumber(size_t valueSize) const {
	if(valueSize > 8){
		throw binary::Exception("Wrong valueSize");
	}

	auto bytes = read(valueSize);

	int64_t num = 0;

	// Странный момент. Ввел дополнительный j, чтобы лишний раз i не делить на 8
	for(size_t i = 0, j = 0; i < valueSize; i += 8, j++) {
		num |= ((int64_t)bytes[j]) << i;
	}

	return num;
}

void Stream::checkForWrite(size_t valueSize) {
	const size_t needSize = offset + valueSize;
	size_t newCap;
	if(needSize > cap){
		if(needSize >= CAP_X2){
			// todo может вообще стоит отказывать в таких размерах
			newCap = (size_t) ((double)needSize * 1.25);
		}else{
			newCap = needSize * 2;
		}

		char* newBuffer = new char[newCap];
		memcpy(newBuffer, buffer, cap);
		delete[] buffer;
		buffer = newBuffer;

		cap = newCap;
	}
}

void Stream::writeSignedByte(int8_t value) {
	writeNumber(sizeof(int8_t), value);
}

int8_t Stream::readSignedByte() const {
	return (int8_t)readNumber(sizeof(int8_t));
}

void Stream::writeUnsignedByte(u_int8_t value) {
	writeNumber(sizeof(int8_t), value);
}

u_int8_t Stream::readUnsignedByte() const {
	return (int8_t)readNumber(sizeof(int8_t));
}

void Stream::writeSignedShort(int16_t value) {
	writeNumber(sizeof(int16_t), value);
}

int16_t Stream::readSignedShort() const {
	return (int16_t) readNumber(sizeof(int16_t));
}

void Stream::writeUnsignedShort(u_int16_t value) {
	writeNumber(sizeof(u_int16_t), value);
}

u_int16_t Stream::readUnsignedShort() const {
	return (u_int16_t) readNumber(sizeof(int16_t));
}

void Stream::writeSignedInt32(int32_t value) {
	writeNumber(sizeof(int32_t), value);
}

u_int32_t Stream::readSignedInt32() const {
	return (int32_t) readNumber(sizeof(int32_t));
}

void Stream::writeUnsignedInt32(u_int32_t value) {
	writeNumber(sizeof(u_int32_t), value);
}

u_int32_t Stream::readUnsignedInt32() const {
	return (u_int32_t) readNumber(sizeof(u_int32_t));
}

void Stream::writeSignedLong(int64_t value) {
	writeNumber(sizeof(int64_t), value);
}

int64_t Stream::readSignedLong() const {
	return readNumber(sizeof(int64_t));
}

void Stream::writeUnsignedLong(u_int64_t value) {
	writeNumber(sizeof(u_int64_t), (int64_t) value);
}

u_int64_t Stream::readUnsignedLong() const {
	return readNumber(sizeof(u_int64_t));
}