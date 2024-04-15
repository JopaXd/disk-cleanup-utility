#include <iostream>
#include <vector>
#include <variant>
#include <memory>
#include "headers/Directory.h"

using namespace std;

Directory::Directory(string name, string path, uintmax_t size, vector<unique_ptr<FSItem>> contents) : FSItem(name, path, size), contents(std::move(contents)){
}

void Directory::print(void) {
    cout << "Name: " << name << ", Path: " << path << ", Size: " << size << endl;
}

ostream& operator<<(std::ostream& os, const Directory& item) {
    os << "Name: " << item.name << ", Path: " << item.path << ", Size: " << item.size;
    return os;
}

int Directory::del(void){
	cout << "Deleted directory." << endl;
	return 0;
}

void Directory::setContents(vector<unique_ptr<FSItem>> newContents) {
    contents = move(newContents);
}

vector<unique_ptr<FSItem>>& Directory::getContents() {
	return this->contents;
}

string Directory::type(void){
	return "dir";
}