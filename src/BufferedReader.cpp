#include <new>
#include "BufferedReader.h"

const DWORD SectorSize = 512;

BufferedReader::BufferedReader(const DWORD size) {
	hFile = INVALID_HANDLE_VALUE;

	buffer = new(std::nothrow) BYTE[size];
	if (buffer != nullptr) { bufferSize = size; }
	else { bufferSize = 0; }
	bufferEnd = 0;

	fileAddress = 0;
	bufferAddress = 0;
	bufferFileAddress = 0;
}
BufferedReader::~BufferedReader() {
	Close();
	if (buffer != nullptr)
		delete[] buffer;
}

DWORD BufferedReader::Read(void* const lpbuffer, const DWORD nNumberOfBytesToRead) {
	DWORD sizeToCopy;
	DWORD numberOfBytesRead = 0;

	if (hFile == INVALID_HANDLE_VALUE) return numberOfBytesRead;
	if (lpbuffer == nullptr) return numberOfBytesRead;
	if (nNumberOfBytesToRead == 0) return numberOfBytesRead;

	if (bufferEnd == 0 && bufferAddress != 0) { // buffer has

	}

	while (numberOfBytesRead<nNumberOfBytesToRead) {
		if (bufferEnd == bufferAddress) {
			bufferFileAddress = fileAddress;
			ReadFile(hFile, buffer, bufferSize, &bufferEnd, nullptr);
			bufferAddress = 0;
			fileAddress = bufferFileAddress + bufferEnd;
		}

		sizeToCopy = min(bufferEnd - bufferAddress, nNumberOfBytesToRead - numberOfBytesRead);
		memcpy(&((BYTE*)lpbuffer)[numberOfBytesRead], &buffer[bufferAddress], sizeToCopy);
		bufferAddress += sizeToCopy;
		numberOfBytesRead += sizeToCopy;
	}

	return numberOfBytesRead;
}
DWORD BufferedReader::Read(const DWORD address, void* const lpbuffer, const DWORD nNumberOfBytesToRead) {
	DWORD sizeToCopy;
	DWORD numberOfBytesRead = 0;

	if (hFile == INVALID_HANDLE_VALUE) return numberOfBytesRead;
	if (lpbuffer == nullptr) return numberOfBytesRead;
	if (nNumberOfBytesToRead == 0) return numberOfBytesRead;

	// initialize bufferAddress
	if (address >= bufferFileAddress && address - bufferFileAddress <= bufferEnd) {
		bufferAddress = address - bufferFileAddress;
	}
	else {
		if (address >= fileAddress && address - fileAddress <= bufferSize) {
			bufferFileAddress = fileAddress;
		}
		else {
			bufferFileAddress = address / SectorSize * SectorSize;
			SetFilePointer(hFile, bufferFileAddress, nullptr, FILE_BEGIN);
		}
		ReadFile(hFile, buffer, bufferSize, &bufferEnd, nullptr);
		bufferAddress = address - bufferFileAddress;
		fileAddress = bufferFileAddress + bufferEnd;
	}

	while (numberOfBytesRead<nNumberOfBytesToRead) {
		if (bufferEnd == bufferAddress) {
			bufferFileAddress = fileAddress;
			ReadFile(hFile, buffer, bufferSize, &bufferEnd, nullptr);
			bufferAddress = 0;
			fileAddress = bufferFileAddress + bufferEnd;
		}

		sizeToCopy = min(bufferEnd - bufferAddress, nNumberOfBytesToRead - numberOfBytesRead);
		memcpy(&((BYTE*)lpbuffer)[numberOfBytesRead], &buffer[bufferAddress], sizeToCopy);
		bufferAddress += sizeToCopy;
		numberOfBytesRead += sizeToCopy;
	}

	return numberOfBytesRead;
}

BOOL BufferedReader::Open(WCHAR lpFileName[]) {
	if (lpFileName != nullptr) {
		hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
			return true; // file successfully openened
	}

	return false;
}
BOOL BufferedReader::Close() {
	if (hFile != INVALID_HANDLE_VALUE)
		return CloseHandle(hFile);
	return false;
}
