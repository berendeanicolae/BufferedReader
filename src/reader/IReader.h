#ifndef IREADER_H
#define IREADER_H

#include <Windows.h>

class IReader {
protected:
	HANDLE hFile; // file handle


protected:
	virtual void reset();


public:
	IReader();
	virtual ~IReader();


	void open(WCHAR lpFileName[]);
	bool isOpen();
	void close();

	
	virtual DWORD read(const DWORD address, void* const buffer, const DWORD size) = 0;
};

#endif // IREADER_H
