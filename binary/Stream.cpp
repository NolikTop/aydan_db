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
	writeNoRelease(dataSize, data);
	delete[] data;
}

void Stream::writeNoRelease(size_t dataSize, char *data) {
	const size_t needSize = offset + dataSize;
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

	memcpy(buffer + offset, data, dataSize);

	offset += dataSize;
}


void Stream::writeNumber(size_t size, int64_t value) {

}

int64_t Stream::readNumber(size_t size) const {
	return 0;
}


void Stream::writeSignedByte(int8_t value) {
	write(sizeof(char), new char[1]{
		(char)value
	});
}

int8_t Stream::readSignedByte() const {
	return read(1)[0];
}

void Stream::writeUnsignedByte(u_int8_t value) {
	writeSignedByte((int8_t) value);
}

u_int8_t Stream::readUnsignedByte() const {
	return readSignedByte();
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

u_int32_t Stream::readSignedInt32() {
	return 0;
}

void Stream::writeUnsignedInt32(u_int32_t value) {

}

u_int32_t Stream::readUnsignedInt32() {
	return 0;
}

void Stream::writeSignedLong(int64_t value) {

}

int64_t Stream::readSignedLong() {
	return 0;
}

void Stream::writeUnsignedLong(u_int64_t value) {

}

u_int64_t Stream::readUnsignedLong() {
	return 0;
}