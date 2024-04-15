#pragma once

#include <string>

using namespace std;

// File system item, could be a file, or a directory.
//Made for other classes to derive
class FSItem{
protected:
	string path;
	uintmax_t size;
	string name;
public:
	FSItem(string path, string name, uintmax_t size=0);
	virtual int del(void) = 0;
	virtual void print(void);
	friend std::ostream& operator<<(std::ostream& os, const FSItem& item);
};