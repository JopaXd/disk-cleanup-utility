#pragma once

#include <string>
#include <vector>
#include <variant>
#include <memory>
#include "File.h"

using namespace std;

class Directory : public FSItem {
private:
	vector<unique_ptr<FSItem>> contents;
	Directory* parent;
public:
	int del(void);
	void print(void);
	friend std::ostream& operator<<(std::ostream& os, const Directory& item);
	void setContents(vector<unique_ptr<FSItem>> newContents);
	string type(void);
	vector<unique_ptr<FSItem>>& getContents();
	Directory(string name, string path, uintmax_t size, vector<unique_ptr<FSItem>> contents = vector<unique_ptr<FSItem>>());
	void setParent(Directory* newParent);
	Directory* getParent(void);
};