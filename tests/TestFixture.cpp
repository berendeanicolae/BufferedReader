#include "TestFixture.h"
#include <random>
#include <algorithm>

void TestFixture::Create(WCHAR *filePath, DWORD size) {
	HANDLE hFile;
	BYTE *buffer = new BYTE[BUFFER_SIZE];

	hFile = CreateFile(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	for (; size; ) {
		DWORD toWrite;

		toWrite = (std::min)(BUFFER_SIZE, size);
		FillBuffer(buffer, toWrite);
		WriteFile(hFile, buffer, toWrite, NULL, NULL);
		size -= toWrite;
	}
	CloseHandle(hFile);
	delete[] buffer;
}

void TestFixture::FillBuffer(BYTE *buffer, DWORD size) {
	static std::default_random_engine e;
	static std::uniform_int_distribution<DWORD> d(0, 0xFFFFFFFF);
	DWORD tr, random;

	for (tr = 0; size >= sizeof(random); tr += sizeof(random), size -= sizeof(random)) {
		random = d(e);
		memcpy(&buffer[tr], &random, sizeof(random));
	}
	for (buffer = &buffer[tr], tr = 0; tr < size; ++tr) {
		buffer[tr] = d(e) & 0xFF;
	}
}

TestFixture::TestFixture() {
	for (int i = 0; i < 3; ++i) {
		GetTempFileName(L".", NULL, 0, files[i]);
		Create(files[i], sizes[i]);
	}
}
TestFixture::~TestFixture() {
	for (int i = 0; i < 3; ++i) {
		DeleteFile(files[i]);
	}
}
