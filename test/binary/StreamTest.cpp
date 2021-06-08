//
// Created by NolikTop on 19.04.2021.
//

#include <gtest/gtest.h>
#include <binary/Stream.h>

// честно взято из StackOverflow https://stackoverflow.com/questions/1460703/comparison-of-arrays-in-google-test
#define EXPECT_ARRAY_EQ(TARTYPE, reference, actual, element_count) \
    {\
    TARTYPE* reference_ = static_cast<TARTYPE *> (reference); \
    TARTYPE* actual_ = static_cast<TARTYPE *> (actual); \
    for(int cmp_i = 0; cmp_i < element_count; cmp_i++ ){\
      EXPECT_EQ(reference_[cmp_i], actual_[cmp_i]);\
    }\
    }

binary::Stream* getStream(){
	return new binary::Stream;
}

TEST(Stream, byte){
	auto s = getStream();
	byte unsignedNumber = 0xFF;
	auto signedNumber = (int8_t)unsignedNumber;
	auto data = (byte*)"\xFF";

	s->writeSignedByte(signedNumber);
	EXPECT_ARRAY_EQ(byte, s->buffer, data, sizeof(unsignedNumber));
	EXPECT_EQ(s->readSignedByte(), signedNumber);

	s->clean();

	s->writeUnsignedByte(unsignedNumber);
	EXPECT_ARRAY_EQ(byte, s->buffer, data, sizeof(unsignedNumber));
	EXPECT_EQ(s->readUnsignedByte(), unsignedNumber);

	delete s;
}

TEST(Stream, short){
	auto s = getStream();
	uint16_t unsignedNumber = 0xFFAA;
	int16_t signedNumber = -0x0002;
	auto data = (byte*)"\xAA\xFF";
	auto dataSigned = (byte*)"\xFE\xFF";

	s->writeSignedShort(signedNumber);

	EXPECT_ARRAY_EQ(byte, s->buffer, dataSigned, sizeof(signedNumber));

	EXPECT_EQ(s->readSignedShort(), signedNumber);

	s->clean();

	s->writeUnsignedShort(unsignedNumber);
	EXPECT_ARRAY_EQ(byte, s->buffer, data, sizeof(unsignedNumber));
	EXPECT_EQ(s->readUnsignedShort(), unsignedNumber);

	delete s;
}

TEST(Stream, int){
	auto s = getStream();
	uint32_t unsignedNumber = 0xFFAABBCC;
	int32_t signedNumber = -0x00000002;
	auto data = (byte*)"\xCC\xBB\xAA\xFF";
	auto dataSigned = (byte*)"\xFE\xFF\xFF\xFF";

	s->writeSignedInt32(signedNumber);
	EXPECT_ARRAY_EQ(byte, s->buffer, dataSigned, sizeof(signedNumber));
	EXPECT_EQ(s->readSignedInt32(), signedNumber);

	s->clean();

	s->writeUnsignedInt32(unsignedNumber);
	EXPECT_ARRAY_EQ(byte, s->buffer, data, sizeof(unsignedNumber));
	EXPECT_EQ(s->readUnsignedInt32(), unsignedNumber);

	delete s;
}

TEST(Stream, long){
	auto s = getStream();
	uint64_t unsignedNumber = 0xFFAABBCCDDEE1122;
	int64_t signedNumber = -0x0000000000000002;
	auto data = (byte*)"\x22\x11\xEE\xDD\xCC\xBB\xAA\xFF";
	auto dataSigned = (byte*)"\xFE\xFF\xFF\xFF\xFF\xFF\xFF\xFF";

	s->writeSignedLong(signedNumber);
	EXPECT_ARRAY_EQ(byte, s->buffer, dataSigned, sizeof(signedNumber));
	EXPECT_EQ(s->readSignedLong(), signedNumber);

	s->clean();

	s->writeUnsignedLong(unsignedNumber);
	EXPECT_ARRAY_EQ(byte, s->buffer, data, sizeof(unsignedNumber));
	EXPECT_EQ(s->readUnsignedLong(), unsignedNumber);

	delete s;
}