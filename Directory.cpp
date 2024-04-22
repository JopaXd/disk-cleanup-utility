#include <iostream>
#include <vector>
#include <variant>
#include <memory>
#include <filesystem>
#include "headers/Directory.h"
#include "headers/Log.h"
#include "headers/InfoLog.h"
#include "headers/ErrorLog.h"

using namespace std;

Directory::Directory(string name, string path, uintmax_t size, vector<unique_ptr<FSItem>> contents) : FSItem(name, path, size), contents(std::move(contents)){
}

void Directory::print(void) {
	const char* suffixes[] = {"B", "KB", "MB", "GB", "TB"};
	int suffixIndex = 0;
		double finalSize = static_cast<double>(size);

		while (finalSize >= 1024 && suffixIndex < 5) {
			finalSize /= 1024;
			suffixIndex++;
	}
	cout << "Name: " << name << ", Path: " << path << ", Size: " << finalSize << " " << suffixes[suffixIndex] << " Contents: " << contents.size() << endl;
}

ostream& operator<<(std::ostream& os, const Directory& item) {
	os << "Name: " << item.name << ", Path: " << item.path << ", Size: " << item.size;
	return os;
}

int Directory::del(void){
	try{
		filesystem::remove_all(path);
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

void Directory::setContents(vector<unique_ptr<FSItem>> newContents) {
	contents = move(newContents);
}

vector<unique_ptr<FSItem>>& Directory::getContents() {
	return contents;
}

string Directory::type(void){
	return "dir";
}

Directory* Directory::getParent(void) {
	return parent;
}

void Directory::setParent(Directory* newParent){
	parent = newParent;
}