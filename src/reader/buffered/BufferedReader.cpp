#include <new>
#include "BufferedReader.h"

const DWORD SectorSize = 512;

BufferedReader::BufferedReader(const DWORD size): bufferSize(size), buffer(nullptr) {
	buffer = new BYTE[bufferSize];
}

BufferedReader::~BufferedReader() {
	if (buffer != nullptr)
		delete[] buffer;
}

void BufferedReader::reset() {
	bufferEnd = 0;
	fileAddress = 0;
	bufferAddress = 0;
	bufferFileAddress = 0;
}

DWORD BufferedReader::read(const DWORD address, void* const lpbuffer, const DWORD nNumberOfBytesToRead) {
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

	if (bufferFileAddress + bufferEnd < address) return numberOfBytesRead;

	while (numberOfBytesRead<nNumberOfBytesToRead) {
		if (bufferEnd == bufferAddress) {
			if (bufferEnd != bufferSize) return numberOfBytesRead;
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
