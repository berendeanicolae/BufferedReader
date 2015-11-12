#include <gtest\gtest.h>
#include <BufferedReader.h>

TEST(BufferedReader, OpenClose) {
	BufferedReader reader(1024);

	EXPECT_FALSE(reader.Close());
	EXPECT_FALSE(reader.Open(nullptr));
	EXPECT_FALSE(reader.Open(L"C:/temp/cwd/zbot"));
	EXPECT_TRUE(reader.Open(L"file"));
	EXPECT_TRUE(reader.Close());
	EXPECT_FALSE(reader.Close());
	EXPECT_FALSE(reader.Open(nullptr));

}


TEST(BufferedReader, InvalidRead) {
	BufferedReader reader(256);
	BYTE buffer[8192] = {};

	EXPECT_EQ(0, reader.Read(nullptr, 0));
	EXPECT_EQ(0, reader.Read(buffer, 0));
	EXPECT_EQ(0, reader.Read(nullptr, 128));
	EXPECT_EQ(0, reader.Read(buffer, 128));
	EXPECT_EQ(0, reader.Read(nullptr, 512));
	EXPECT_EQ(0, reader.Read(buffer, 512));
}

TEST(BufferedReader, Read) {
	BufferedReader reader(256);
	HANDLE hFile;
	DWORD readSize1, readSize2;
	BYTE buffer1[8192] = {}, buffer2[8192] = {};

	hFile = CreateFile(L"file", GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	EXPECT_TRUE(reader.Open(L"file"));
	EXPECT_EQ(0, reader.Read(buffer1, 0));


	readSize1 = reader.Read(buffer1, 128);
	ReadFile(hFile, buffer2, 128, &readSize2, nullptr);
	EXPECT_EQ(readSize2, readSize1);
	EXPECT_EQ(0, memcmp(buffer1, buffer2, readSize1));
	readSize1 = reader.Read(buffer1, 512);
	ReadFile(hFile, buffer2, 512, &readSize2, nullptr);
	EXPECT_EQ(readSize2, readSize1);
	EXPECT_EQ(0, memcmp(buffer1, buffer2, readSize1));
	readSize1 = reader.Read(buffer1, 2048);
	ReadFile(hFile, buffer2, 2048, &readSize2, nullptr);
	EXPECT_EQ(readSize2, readSize1);
	EXPECT_EQ(0, memcmp(buffer1, buffer2, readSize1));

	SetFilePointer(hFile, 0, nullptr, SEEK_SET);
	readSize1 = reader.Read(0, buffer1, 128);
	ReadFile(hFile, buffer2, 128, &readSize2, nullptr);
	EXPECT_EQ(readSize2, readSize1);
	EXPECT_EQ(0, memcmp(buffer1, buffer2, readSize1));

	SetFilePointer(hFile, 0, nullptr, SEEK_SET);
	readSize1 = reader.Read(0, buffer1, 256);
	ReadFile(hFile, buffer2, 256, &readSize2, nullptr);
	EXPECT_EQ(readSize2, readSize1);
	EXPECT_EQ(0, memcmp(buffer1, buffer2, readSize1));

	SetFilePointer(hFile, 0, nullptr, SEEK_SET);
	readSize1 = reader.Read(0, buffer1, 2048);
	ReadFile(hFile, buffer2, 2048, &readSize2, nullptr);
	EXPECT_EQ(readSize2, readSize1);
	EXPECT_EQ(0, memcmp(buffer1, buffer2, readSize1));

	SetFilePointer(hFile, 0x100, nullptr, SEEK_SET);
	readSize1 = reader.Read(0x100, buffer1, 2048);
	ReadFile(hFile, buffer2, 2048, &readSize2, nullptr);
	EXPECT_EQ(readSize2, readSize1);
	EXPECT_EQ(0, memcmp(buffer1, buffer2, readSize1));
}

int main(int argc, char *argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}