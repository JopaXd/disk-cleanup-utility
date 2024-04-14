#pragma once

#include <string>
#include "FSItem.h"

using namespace std;

class File : public FSItem {
public:
	int del(void);
	void print(void);
	friend std::ostream& operator<<(std::ostream& os, const File& item);
	File(string name, string size, string path);
};