//
// Created by NolikTop on 17.04.2021.
//

#include <string>
#include "Stream.h"
#include "Exception.h"

using namespace binary;

Stream::Stream(size_t size, byte *buffer) : size(size), offset(0), cap(size), buffer(buffer) {}

Stream::Stream(size_t cap) : size(0), offset(0), cap(cap), buffer(new byte[cap]){}

Stream::~Stream() {
	delete[] buffer;
}

bool Stream::eof() const {
    return offset >= size;
}

void Stream::clean(size_t capacity) {
	delete[] buffer;
	cap = capacity;
	offset = 0;
	size = 0;
	buffer = new byte[cap];
}

std::string Stream::getBufferAsString() const {
	return std::string((char*)buffer, size);
}

byte *Stream::read(size_t valueSize) {
	if(offset + valueSize > size){
		throw binary::Exception("Cant read more information than exists. Offset=" + std::to_string(offset) + " ValueSIze=" + std::to_string(valueSize) + " Size=" + std::to_string(size));
	}
	offset += valueSize;
	return buffer + offset - valueSize;
}

void Stream::writeByteString(const std::string& value) {
	if(value.length() > 0xFF){
		throw Exception("Value is too big (more than 255)");
	}

	writeUnsignedByte(value.length());

	for(auto c : value){
		writeSignedByte(c);
	}
}

std::string Stream::readByteString() {
	auto len = readUnsignedByte();

	return std::string((char*)read(len), len);
}

void Stream::writeShortString(const std::string& value) {
	if(value.length() > 0xFFFF){
		throw Exception("Value is too big (more than 65535)");
	}

	writeUnsignedShort(value.length());

	for(auto c : value){
		writeSignedByte(c);
	}
}

std::string Stream::readShortString() {
	auto len = readUnsignedShort();

	return std::string((char*)read(len), len);
}

void Stream::write(size_t dataSize, byte* data) {
	writeNotReleaseValue(dataSize, data);
	delete[] data;
}

void Stream::writeNotReleaseValue(size_t valueSize, byte *value) {
	checkForWrite(valueSize);

	memcpy(buffer + offset, value, valueSize);
}

void Stream::writeNumber(size_t valueSize, int64_t value) {
	if(valueSize > 8){
		throw binary::Exception("Wrong valueSize");
	}

	checkForWrite(valueSize);

	byte* buf = buffer + offset;

	// > 0, а не >= 0, потому что иначе происходит integer overflow
	for(size_t j = valueSize - 1, i = j * 8; i > 0; i -= 8, j--){
		buf[j] = (byte)((value >> i) & 0xFF);
	}

	buf[0] = (byte)(value & 0xFF);
}

int64_t Stream::readNumber(size_t valueSize){
	if(valueSize > 8){
		throw binary::Exception("Wrong valueSize");
	}

	auto bytes = read(valueSize);

	int64_t num = 0;

	// Странный момент. Ввел дополнительный j, чтобы лишний раз i не делить на 8
	for(size_t i = 0, j = 0; j < valueSize; i += 8, j++) {
		num |= ((int64_t)bytes[j]) << i;
	}

	return num;
}

void Stream::checkForWrite(size_t valueSize) {
	offset = size;
	size += valueSize;
	size_t newCap = 0;
	//std::cout << "check ";
	//std::cout << "size=" << size << " cap=" << cap << std::endl;
	if(size > cap){
		//std::cout << "realloc";
		//std::cout << std::endl;
		this->dump();

		if(size >= CAP_X2){
			// todo может вообще стоит отказывать в таких размерах
			newCap = (size_t) ((double)size * 1.25);
		}else{

			newCap = size * 2;
		}

		auto newBuffer = new byte[newCap];
		for(auto i = 0; i < cap; ++i){
			newBuffer[i] = buffer[i];
		}
		delete[] buffer;
		buffer = newBuffer;
		cap = newCap;
		this->dump();

	}
}

void Stream::writeSignedByte(int8_t value) {
	writeNumber(sizeof(int8_t), value);
}

int8_t Stream::readSignedByte(){
	return (int8_t)readNumber(sizeof(int8_t));
}

void Stream::writeUnsignedByte(uint8_t value) {
	writeNumber(sizeof(int8_t), value);
}

uint8_t Stream::readUnsignedByte(){
	return (int8_t)readNumber(sizeof(int8_t));
}

void Stream::writeSignedShort(int16_t value) {
	writeNumber(sizeof(int16_t), value);
}

int16_t Stream::readSignedShort(){
	return (int16_t) readNumber(sizeof(int16_t));
}

void Stream::writeUnsignedShort(uint16_t value) {
	writeNumber(sizeof(uint16_t), value);
}

uint16_t Stream::readUnsignedShort(){
	return (uint16_t) readNumber(sizeof(int16_t));
}

void Stream::writeSignedInt32(int32_t value) {
	writeNumber(sizeof(int32_t), value);
}

int32_t Stream::readSignedInt32(){
	return (int32_t) readNumber(sizeof(int32_t));
}

void Stream::writeUnsignedInt32(uint32_t value) {
	writeNumber(sizeof(uint32_t), value);
}

uint32_t Stream::readUnsignedInt32(){
	return (uint32_t) readNumber(sizeof(uint32_t));
}

void Stream::writeSignedLong(int64_t value) {
	writeNumber(sizeof(int64_t), value);
}

int64_t Stream::readSignedLong(){
	return readNumber(sizeof(int64_t));
}

void Stream::writeUnsignedLong(uint64_t value) {
	writeNumber(sizeof(uint64_t), (int64_t) value);
}

uint64_t Stream::readUnsignedLong(){
	return readNumber(sizeof(uint64_t));
}

void Stream::dump() {
	//std::cout << "HEX dump of binary::Stream. Size=" << this->size << ". Current offset=" << this->offset;
}
