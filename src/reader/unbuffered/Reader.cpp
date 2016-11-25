#include "Reader.h"
#include <cstdio>

Reader::Reader() {}
Reader::~Reader() {}

DWORD Reader::read(const DWORD address, void* const buffer, const DWORD size) {
	DWORD numberOfBytesRead = 0;

	if (hFile == INVALID_HANDLE_VALUE) return numberOfBytesRead;
	if (buffer == nullptr) return numberOfBytesRead;
	if (size == 0) return numberOfBytesRead;
	
	SetFilePointer(hFile, address, NULL, SEEK_SET);
	ReadFile(hFile, buffer, size, &numberOfBytesRead, NULL);
	 
	return numberOfBytesRead;
}
