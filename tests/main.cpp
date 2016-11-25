#include <gtest/gtest.h>
#include "TestFixture.h"

#include <random>
#include <algorithm>
#include "reader/Ireader.h"
#include "reader/unbuffered/Reader.h"
#include "reader/buffered/BufferedReader.h"

namespace testing {
	namespace internal {
		enum GTestColor {
			COLOR_DEFAULT,
			COLOR_RED,
			COLOR_GREEN,
			COLOR_YELLOW
		};
		extern void ColoredPrintf(GTestColor color, const char* fmt, ...);
	}
}

#define PRINTF(...)  do { testing::internal::ColoredPrintf(testing::internal::COLOR_GREEN, "[          ] "); testing::internal::ColoredPrintf(testing::internal::COLOR_YELLOW, __VA_ARGS__); } while(0)

TEST_F(TestFixture, BufferedReader) {
	IReader *reader1 = new Reader();
	IReader *reader2 = new BufferedReader(1024);
	BYTE *buffer1 = new BYTE[0x10000000](), *buffer2 = new BYTE[0x10000000]();

	std::default_random_engine e;
	std::uniform_int_distribution<DWORD> d(0, 0xFFFFFFFF);


	EXPECT_EQ(reader1->read(0x172, buffer1, 12), reader2->read(0x172, buffer2, 12));

	for (int i = 0; i < 3; ++i) {
		reader1->open(files[i]);
		reader2->open(files[i]);

		DWORD address, offset, size;
		for (int j = 0; j < 100; ++j) {
			address = d(e) % sizes[i];
			for (int k = 0; k < 5; ++k) {
				offset = address + (std::min)((sizes[i] - address) ? (d(e) % (sizes[i] - address)) : 0, 1000UL);
				size = (sizes[i] - offset) ? (d(e) % (std::min)(sizes[i] - offset, 1000UL)) : 0;
				EXPECT_EQ(reader1->read(address, buffer1, size), reader2->read(address, buffer2, size));
				EXPECT_EQ(0, memcmp(buffer1, buffer2, size));
			}
		}

		reader1->close();
		reader2->close();
	}

	delete reader1;
	delete reader2;
	delete[] buffer1;
	delete[] buffer2;
}

TEST_F(TestFixture, BufferedReaderSpeed) {
	IReader *reader;
	BYTE *buffer = new BYTE[0x10000000]();
	std::vector<std::pair<DWORD, DWORD>> addresses;

	std::default_random_engine e;
	std::uniform_int_distribution<DWORD> d(0, 0xFFFFFFFF);


	for (int i = 0; i < 3; ++i) {
		DWORD address, offset, size;
		for (int j = 0; j < 10000; ++j) {
			address = d(e) % sizes[i];
			for (int k = 0; k < 5; ++k) {
				offset = address + (std::min)((sizes[i] - address) ? (d(e) % (sizes[i] - address)) : 0, 1000UL);
				size = (sizes[i] - offset) ? (d(e) % (std::min)(sizes[i] - offset, 1000UL)) : 0;
				addresses.push_back(std::make_pair(offset, size));
			}
		}
	}


	DWORD start, stop, time1, time2;

	reader = new Reader();
	start = GetTickCount();
	for (int i = 0; i < 3; ++i) {
		reader->open(files[i]);
		for (int j=0; j<50000; ++j) reader->read(addresses[j].first, buffer, addresses[j].second);
		reader->close();
	}
	stop = GetTickCount();
	time1 = stop - start;
	PRINTF("Reader\t%d\n", time1);
	delete reader;

	reader = new BufferedReader(1024);
	start = GetTickCount();
	for (int i = 0; i < 3; ++i) {
		reader->open(files[i]);
		for (int j = 0; j<500; ++j) reader->read(addresses[j].first, buffer, addresses[j].second);
		reader->close();
	}
	stop = GetTickCount();
	time2 = stop - start;
	PRINTF("BufferedReader\t%d\n", time2);
	delete reader;

	EXPECT_LE(time2, time1);

	delete[] buffer;
}

int main(int argc, char *argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
