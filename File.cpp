#include <iostream>
#include <filesystem>
#include "headers/File.h"
#include "headers/Log.h"
#include "headers/InfoLog.h"
#include "headers/ErrorLog.h"

using namespace std;

File::File(string name, string path, uintmax_t size) : FSItem(name, path, size){
	
}

void File::print(void) {
	const char* suffixes[] = {"B", "KB", "MB", "GB", "TB"};
	int suffixIndex = 0;
		double finalSize = static_cast<double>(size);

		while (finalSize >= 1024 && suffixIndex < 5) {
			finalSize /= 1024;
			suffixIndex++;
	}
    cout << "Name: " << name << ", Path: " << path << ", Size: " << finalSize << " " << suffixes[suffixIndex] << endl;
}

ostream& operator<<(ostream& os, const File& item) {
    os << "Name: " << item.name << ", Path: " << item.path << ", Size: " << item.size;
    return os;
}

int File::del(void){
	try{
		filesystem::remove(this->path);
		InfoLog il("Deleted " + path);
		il.writeLog();
		return 0;
	}
	catch (filesystem::filesystem_error &exc) {
		ErrorLog el("Error when trying to delete " + path);
		el.writeLog();
		return 1;
	}
}

string File::type(void){
	return "file";
}