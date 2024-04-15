#include <iostream>
#include <cstdlib>
#include <limits>
#include <filesystem>
#include <string>
#include <memory>
#include <algorithm>
#include "headers/Directory.h"
#include "headers/File.h"
#include "headers/FSItem.h"

#define BIN_PATH " ~/.local/share/Trash/"
#define APP_CACHE_PATH "~/.cache/"
#define APP_LOGS "/var/log"
#define DEBUG 1
// #define APT_CACHE = "/var/cache/apt/archives";

using namespace std;

void clear(void);
void delete_dir_content(const filesystem::path& dir);
uintmax_t directory_size(string path);
void clear_input_buffer();

int main() {
	int choice;
	clear();
	cout << "*** FS CleanUp Utility***" << endl;
	cout << "1. Scan a directory. (requires root depending on the task)" << endl;
	cout << "2. Empty recycle bin." << endl;
	cout << "3. Delete app cache." << endl;
	cout << "4. Delete app logs. (requires root)" << endl;
	cout << "5. Exit" << endl;
	while (true){
		cin >> choice;
		//When something that is not a number is entered, it returns 0.
		if (choice == 1) {
			string path;
			// File f("test", "test", "test");
			// File f1("test", "test", "test");
			// Directory d1("test", "test", "test");
			// vector<FSItem*> fs;
			// fs.push_back(&f);
			// fs.push_back(&f1);
			// fs.push_back(&d1);
			// for (auto i : fs) {
			// 	i->del();
			// }
			cout << "Enter the path: (/ for root, this might take a considerable amount of time. Root is also required to access protected parts of the filesystem.)" << endl;
			cin >> path;
			size_t found = path.find_last_of("/");
			uintmax_t currentDirSize = directory_size(path);
			string dirName = path.substr(found+1);
			Directory currentDir(dirName, path, currentDirSize);
			currentDir.print();
			vector<unique_ptr<FSItem>> fs;
			for (const auto & entry: filesystem::directory_iterator(path)) {
				if (filesystem::is_directory(entry)){
					//TODO: Use threading to improve the speed.
					cout << entry.path().filename().string() << " " << entry.path() << endl;
					uintmax_t s = directory_size(entry.path());
					fs.push_back(make_unique<Directory>(entry.path().filename().string(), entry.path(), s));
				}
				else{
					cout << entry.path().filename().string() <<  " " << entry.path() << endl;
					fs.push_back(make_unique<File>(entry.path().filename().string(), entry.path(), filesystem::file_size(entry)));
				}
			}
			currentDir.setContents(move(fs));
			int index = 0;
			int scanChoice;
			while(true){
				for (auto& i : currentDir.getContents()) {
					cout << index << " ";
					i->print();
					index++;
				}
				cout << endl << endl << endl;
				cout << "1. Sort by size (ascending)" << endl;
				cout << "2. Sort by size (descending)" << endl;
				cout << "3. Select an item." << endl;
				cout << "4. Wipe folder." << endl;
				cout << "5. Back." << endl;
				cin >> scanChoice;
				vector<unique_ptr<FSItem>>& dirContents = currentDir.getContents();
				if (scanChoice == 1) {
					sort(dirContents.begin(), dirContents.end(), [](unique_ptr<FSItem>& a, unique_ptr<FSItem>& b){ return a->getSize() < b->getSize(); });
					clear();
				}
				else if (scanChoice == 2) {
					sort(dirContents.begin(), dirContents.end(), [](unique_ptr<FSItem>& a, unique_ptr<FSItem>& b){ return a->getSize() > b->getSize(); });
					clear();
				}
				else {
					//Clear the input buffer so that the programd oes not go into an infinite loop.
					clear_input_buffer();
					continue;
				}
			}
		}
		else if (choice == 2) {
			// delete_dir_content(BIN_PATH);
			cout << "Empty recycle bin" << endl;
		}
		else if (choice == 3) {
			cout << "Empty temp folder" << endl;
		}
		else if (choice == 4) {
			cout << "Empty app logs." << endl;
		}
		else if (choice == 5) {
			cout << "Goodbye!" << endl;
			break;
		}
		else {
			//Clear the input buffer so that the programd oes not go into an infinite loop.
			clear_input_buffer();
		}
	}
	return 0;
}

void clear(){
	#ifdef _WIN32
	system("cls");
	#else
	system("clear");
	#endif
}

void delete_dir_content(const filesystem::path& dir){
	for (const auto& entry: filesystem::directory_iterator(dir)) {
		filesystem::remove_all(entry.path());
	}
}

uintmax_t directory_size(string path) {
    uintmax_t size = 0;
    for (const auto& entry : filesystem::recursive_directory_iterator(path)) {
        if (!filesystem::is_directory(entry)) {
        	try{
    	        size += filesystem::file_size(entry);
        	}
        	//Failed to read for various reasons... (Permissions, fs errors, etc...)
        	catch (filesystem::filesystem_error &exc){
        		if (DEBUG){
	        		cout << "ERROR!" << " " << exc.what() << endl;
        		}
        		continue;
        	}
        }
    }
    return size;
}

void clear_input_buffer() {
	//Clear the input buffer so that the programd oes not go into an infinite loop.
	std::cin.clear();
    std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "Please select a valid option." << endl;
}