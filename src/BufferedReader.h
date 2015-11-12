#ifndef BUFFERED_READER_H
#define BUFFERED_READER_H

#ifdef __linux__
#elif _WIN32
	#include <Windows.h>
#endif

class BufferedReader {
private:
	HANDLE hFile; // file handle

	BYTE *buffer; // pointer to allocated buffer
	DWORD bufferSize;  // size of allocated buffer
	DWORD bufferEnd; //

	DWORD fileAddress; // file pointer offset
	DWORD bufferAddress; // buffer pointer offset
	DWORD bufferFileAddress; // the address from where the buffer was filled

public:
	BufferedReader(const DWORD size);
	~BufferedReader();

	BOOL Open(WCHAR lpFileName[]);
	BOOL Close();

	DWORD Read(void* const lpbuffer, const DWORD nNumberOfBytesToRead);
	DWORD Read(const DWORD address, void* const lpbuffer, const DWORD nNumberOfBytesToRead);
};

#endif // BUFFERED_READER_H
