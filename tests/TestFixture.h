#ifndef TEST_FIXTURE_H
#define TEST_FIXTURE_H

#include <gtest/gtest.h>
#include <Windows.h>

class TestFixture : public ::testing::Test {
private:
	DWORD BUFFER_SIZE = 1024;

public:
	WCHAR files[3][MAX_PATH];
	DWORD sizes[3] = { 0x100, 0x10000, 0x10000000 };

private:
	void FillBuffer(BYTE *buffer, DWORD size);
	void Create(WCHAR *filePath, DWORD size);

public:
	TestFixture();
	~TestFixture();
};

#endif // TEST_FIXTURE_H
