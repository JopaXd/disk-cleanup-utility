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

bool FSItem::operator<(const FSItem& fsi) {
	return size < fsi.getSize();
}

bool FSItem::operator>(const FSItem& fsi) {
	return size > fsi.getSize();
}

string FSItem::getName(){
	return this->name;
}

void FSItem::setName(string name) {
	this->name = name;
}

string FSItem::getPath(){
	return this->path;
}

void FSItem::setPath(string path) {
	this->path = path;
}

uintmax_t FSItem::getSize() const{
	return this->size;
}

void FSItem::setSize(uintmax_t size){
	this->size = size;
}

string FSItem::type(void){
	return "fsitem";
}