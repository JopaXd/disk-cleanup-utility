#include <iostream>
#include "headers/FSItem.h"

using namespace std;

FSItem::FSItem(string name, string path, uintmax_t size){
	this->name = name;
	this->size = size;
	this->path = path;
}

void FSItem::print(void) {
    cout << "Name: " << name << ", Path: " << path << ", Size: " << size << endl;
}

ostream& operator<<(std::ostream& os, const FSItem& item) {
    os << "Name: " << item.name << ", Path: " << item.path << ", Size: " << item.size;
    return os;
}