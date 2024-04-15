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
	clear();
	int choice;
	while (true){
		cout << "*** FS CleanUp Utility***" << endl;
		cout << "1. Scan a directory. (requires root depending on the task)" << endl;
		cout << "2. Empty recycle bin." << endl;
		cout << "3. Delete app cache." << endl;
		cout << "4. Delete app logs. (requires root)" << endl;
		cout << "5. Exit" << endl;
		cin >> choice;
		//When something that is not a number is entered, it returns 0.
		if (choice == 1) {
			string path;
			cout << "Enter the path: (/ for root, this might take a considerable amount of time. Root is also required to access protected parts of the filesystem.)" << endl;
			cin >> path;
			size_t found = path.find_last_of("/");
			uintmax_t currentDirSize = directory_size(path);
			string dirName = path.substr(found+1);
			Directory currentDir(dirName, path, currentDirSize);
			vector<unique_ptr<FSItem>> fs;
			for (const auto & entry: filesystem::directory_iterator(path)) {
				if (filesystem::is_directory(entry)){
					//Quickly add child elements to child dirs (this is only to show the count of items in directory).
					vector<unique_ptr<FSItem>> child_fs;
					for (const auto & child_entry: filesystem::directory_iterator(entry.path())) {
						if (filesystem::is_directory(child_entry)){
							uintmax_t child_s = directory_size(child_entry.path());
							child_fs.push_back(make_unique<Directory>(child_entry.path().filename().string(), child_entry.path(), child_s));
						}
						else{
							child_fs.push_back(make_unique<File>(child_entry.path().filename().string(), child_entry.path(), filesystem::file_size(child_entry)));
						}
					}
					uintmax_t s = directory_size(entry.path());
					Directory child_dir(entry.path().filename().string(), entry.path(), s);
					child_dir.setContents(move(child_fs));
					fs.push_back(std::make_unique<Directory>(std::move(child_dir)));
				}
				else{
					fs.push_back(make_unique<File>(entry.path().filename().string(), entry.path(), filesystem::file_size(entry)));
				}
			}
			currentDir.setContents(move(fs));
			//Dummy stuff.
			Directory nextDir("null", "null", 0);
			int index;
			int scanChoice;
			string nextDirPath = "";
			while(true){
				if (nextDirPath == path){
					nextDirPath = "";
				}
				index = 0;
				clear();
				//Doing it by path because a folder can be named null.
				if (nextDirPath != ""){
					size_t nextFound = path.find_last_of("/");
					uintmax_t nextDirSize = directory_size(nextDirPath);
					string nextDirName = path.substr(nextFound+1);
					nextDir.setPath(nextDirPath);
					nextDir.setName(nextDirName);
					nextDir.setSize(nextDirSize);
					vector<unique_ptr<FSItem>> nextFs;
					for (const auto & entry: filesystem::directory_iterator(nextDirPath)) {
						if (filesystem::is_directory(entry)){
							//Quickly add child elements to child dirs (this is only to show the count of items in directory).
							vector<unique_ptr<FSItem>> child_fs;
							for (const auto & child_entry: filesystem::directory_iterator(entry.path())) {
								if (filesystem::is_directory(child_entry)){
									uintmax_t child_s = directory_size(child_entry.path());
									child_fs.push_back(make_unique<Directory>(child_entry.path().filename().string(), child_entry.path(), child_s));
								}
								else{
									child_fs.push_back(make_unique<File>(child_entry.path().filename().string(), child_entry.path(), filesystem::file_size(child_entry)));
								}
							}
							uintmax_t s = directory_size(entry.path());
							Directory child_dir(entry.path().filename().string(), entry.path(), s);
							child_dir.setContents(move(child_fs));
							nextFs.push_back(std::make_unique<Directory>(std::move(child_dir)));
						}
						else{
							nextFs.push_back(make_unique<File>(entry.path().filename().string(), entry.path(), filesystem::file_size(entry)));
						}
					}
					nextDir.setContents(move(nextFs));
					for (auto& i : nextDir.getContents()) {
						cout << index << " ";
						i->print();
						index++;
					}
					cout << endl << endl << endl;
					cout << "Contents of:" << nextDir.getPath() << endl;
					cout << endl << endl << endl;
					cout << "1. Sort by size (ascending)" << endl;
					cout << "2. Sort by size (descending)" << endl;
					cout << "3. Select an item." << endl;
					cout << "4. Wipe folder." << endl;
					cout << "5. Back." << endl;
					cout << "6. Back to main menu." << endl;
				}
				else{
					for (auto& i : currentDir.getContents()) {
						cout << index << " ";
						i->print();
						index++;
					}
					cout << endl << endl << endl;
					cout << "Contents of:" << currentDir.getPath() << endl;
					cout << endl << endl << endl;
					cout << "1. Sort by size (ascending)" << endl;
					cout << "2. Sort by size (descending)" << endl;
					cout << "3. Select an item." << endl;
					cout << "4. Wipe folder." << endl;
					cout << "5. Back to main menu." << endl;
				}
				cin >> scanChoice;
				clear_input_buffer();
				vector<unique_ptr<FSItem>>& dirContents = (nextDirPath == "") ? currentDir.getContents() : nextDir.getContents();
				if (scanChoice == 1) {
					sort(dirContents.begin(), dirContents.end(), [](unique_ptr<FSItem>& a, unique_ptr<FSItem>& b){ return a->getSize() < b->getSize(); });
					clear();
				}
				else if (scanChoice == 2) {
					sort(dirContents.begin(), dirContents.end(), [](unique_ptr<FSItem>& a, unique_ptr<FSItem>& b){ return a->getSize() > b->getSize(); });
					clear();
				}
				else if(scanChoice == 3) {
					int itemIndex = 0;
					while(true){
						cout << "Select index of item you want to work with." << endl;
						cin >> itemIndex;
						clear_input_buffer();
						if (itemIndex > dirContents.size()){
							cout << "Please select a valid index." << endl;
							continue;
						}
						else{
							break;
						}
					}
					unique_ptr<FSItem>& chosenItem = dirContents[itemIndex];
					cout << endl << endl;
					chosenItem->print();
					cout << endl << endl;
					int operation;
					if (chosenItem->type() == "file"){
						cout << "What would you like to do?" << endl;
						cout << "1. Delete file" << endl;
						cout << "2. Back" << endl;;
						while(true){
							cin >> operation;
							clear_input_buffer();
							if (operation == 1) {
								int success = chosenItem->del();
								if (success == 0) {
									cout << endl;
									cout << "----------------" << endl;
									cout << "File deleted successfully!" << endl;
									cout << "----------------" << endl;
								}
								else{
									cout << endl;
									cout << "----------------" << endl;
									cout << "Error deleting file!" << endl;
									cout << "----------------" << endl;
								}
								break;
							}
							else if (operation == 2) {
								break;
							}
							else{
								cout << "Invalid option!" << endl;
								continue;
							}
						}
					}
					else if (chosenItem->type() == "dir"){
						cout << "What would you like to do?" << endl;
						cout << "1. Delete folder" << endl;
						cout << "2. Scan folder" << endl;
						cout << "3. Back" << endl;
						while(true){
							cin >> operation;
							if (operation == 1) {
								int success = chosenItem->del();
								if (success == 0) {
									cout << endl;
									cout << "----------------" << endl;
									cout << "Folder deleted successfully!" << endl;
									cout << "----------------" << endl;
								}
								else{
									cout << endl;
									cout << "----------------" << endl;
									cout << "Error deleting folder!" << endl;
									cout << "----------------" << endl;
								}
								break;
							}
							else if (operation == 2) {
								nextDirPath = chosenItem->getPath();
								break;
							}
							else if (operation == 3) {
								break;
							}
							else{
								cout << "Invalid option!" << endl;
								continue;
							}
						}
					}
				}
				else if (scanChoice == 4) {
					if (nextDirPath != "") {
						//Deleting a path that the user went to.
						int success = nextDir.del();
						if (success == 0) {
							cout << endl;
							cout << "----------------" << endl;
							cout << "Folder deleted successfully!" << endl;
							cout << "----------------" << endl;
							filesystem::path fp(nextDirPath);
							nextDirPath = fp.parent_path().string();
						}
						else{
							cout << endl;
							cout << "----------------" << endl;
							cout << "Error deleting folder!" << endl;
							cout << "----------------" << endl;
						}
						
					}
					else{
						//Deleting the initially scanned path.
						int success = currentDir.del();
						if (success == 0) {
							cout << endl;
							cout << "----------------" << endl;
							cout << "Folder deleted successfully!" << endl;
							cout << "----------------" << endl;
						}
						else{
							cout << endl;
							cout << "----------------" << endl;
							cout << "Error deleting folder!" << endl;
							cout << "----------------" << endl;
						}
						//Back to main menu
						break;
					}
				}
				else if (scanChoice == 5) {
					if (nextDirPath != "") {
						filesystem::path fp(nextDirPath);
						nextDirPath = fp.parent_path().string();
					}
					else{
						//Back to main menu
						clear();
						break;
					}
				}
				else if (scanChoice == 6) {
					if (nextDirPath == "") {
						continue;
					}
					else{
						//Back to main menu
						clear();
						break;
					}
				}
				else {
					//Clear the input buffer so that the programd oes not go into an infinite loop.
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
			clear();
			clear_input_buffer();
			cout << "Please select a valid option." << endl;
		}
	}
	return 0;
}

void clear(){
	system("clear");
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
}