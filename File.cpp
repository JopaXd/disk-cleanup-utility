#include <iostream>
#include <filesystem>
#include "headers/File.h"

using namespace std;

File::File(string name, string path, uintmax_t size) : FSItem(name, path, size){
	
}

void File::print(void) {
    cout << "Name: " << name << ", Path: " << path << ", Size: " << size << endl;
}

ostream& operator<<(std::ostream& os, const File& item) {
    os << "Name: " << item.name << ", Path: " << item.path << ", Size: " << item.size;
    return os;
}

int File::del(void){
	try{
		filesystem::remove(this->path);
		return 0;
	}
	catch (filesystem::filesystem_error &exc) {
		return 1;
	}
}

string File::type(void){
	return "file";
}