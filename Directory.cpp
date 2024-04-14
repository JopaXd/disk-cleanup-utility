#include <iostream>
#include <vector>
#include "headers/Directory.h"

using namespace std;

Directory::Directory(string name, string size, string path, vector<File> files) : FSItem(name, size, path){
	this->files = files;
}

void Directory::print(void) {
    cout << "Name: " << name << ", Path: " << path << ", Size: " << size << endl;
}

ostream& operator<<(std::ostream& os, const Directory& item) {
    os << "Name: " << item.name << ", Path: " << item.path << ", Size: " << item.size;
    return os;
}

int Directory::del(void){
	cout << "Deleted" << endl;
	return 0;
}