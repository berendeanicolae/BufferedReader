#ifndef BUFFERED_READER_H
#define BUFFERED_READER_H

#include "../IReader.h"

class BufferedReader : public IReader {
private:
	BYTE *buffer; // pointer to allocated buffer
	DWORD bufferSize;  // size of allocated buffer
	DWORD bufferEnd; //

	DWORD fileAddress; // file pointer offset
	DWORD bufferAddress; // buffer pointer offset
	DWORD bufferFileAddress; // the address from where the buffer was filled

private:
	void reset();

public:
	BufferedReader(const DWORD size);
	~BufferedReader();

	DWORD read(const DWORD address, void* const lpbuffer, const DWORD nNumberOfBytesToRead);
};

#endif // BUFFERED_READER_H
