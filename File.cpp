#include <iostream>
#include "headers/File.h"

using namespace std;

File::File(string name, string size, string path) : FSItem(name, size, path){
	
}

void File::print(void) {
    cout << "Name: " << name << ", Path: " << path << ", Size: " << size << endl;
}

ostream& operator<<(std::ostream& os, const File& item) {
    os << "Name: " << item.name << ", Path: " << item.path << ", Size: " << item.size;
    return os;
}

int File::del(void){
	cout << "Deleted file" << endl;
	return 0;
}