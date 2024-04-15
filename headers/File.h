#pragma once

#include <string>
#include "FSItem.h"

using namespace std;

class File : public FSItem {
public:
	int del(void);
	void print(void);
	friend std::ostream& operator<<(std::ostream& os, const File& item);
	string type(void);
	File(string name, string path, uintmax_t size);
};