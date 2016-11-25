#include "IReader.h"

IReader::IReader() : hFile(INVALID_HANDLE_VALUE) { reset(); }
IReader::~IReader() { close(); }

void IReader::reset() {}

void IReader::open(WCHAR lpFileName[]) {
	close();

	if (lpFileName != nullptr) hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	reset();
}

bool IReader::isOpen() {
	return hFile != INVALID_HANDLE_VALUE;
}

void IReader::close() {
	if (isOpen()) {
		CloseHandle(hFile);
	}
	hFile = INVALID_HANDLE_VALUE;
}
