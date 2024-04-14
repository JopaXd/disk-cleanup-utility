#pragma once

#include <string>
#include <vector>
#include <variant>
#include "File.h"

using namespace std;

class Directory : public FSItem {
private:
	vector<FSItem*> files;
public:
	int del(void);
	void print(void);
	friend std::ostream& operator<<(std::ostream& os, const Directory& item);
	Directory(string name, string size, string path, vector<FSItem*> files = vector<FSItem*>());
};