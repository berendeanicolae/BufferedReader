#ifndef READER_H
#define READER_H

#include "../IReader.h"

class Reader : public IReader {
public:
	Reader();
	~Reader();

	DWORD read(const DWORD address, void* const lpbuffer, const DWORD nNumberOfBytesToRead);
};

#endif // READER_H
