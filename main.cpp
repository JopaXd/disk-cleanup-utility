#include <iostream>
#include <cstdlib>
#include <limits>

using namespace std;

void clear(void);

int main() {
	int choice;
	clear();
	cout << "*** FS CleanUp Utility***" << endl;
	cout << "1. Scan a directory." << endl;
	cout << "2. Empty recycle bin." << endl;
	cout << "3. Delete temp data." << endl;
	cout << "0. Exit" << endl;
	while (true){
		cin >> choice;
		if (choice == 1) {
			cout << "scan" << endl;
		}
		else if (choice == 2) {
			cout << "Empty recycle bin" << endl;
		}
		else if (choice == 3) {
			cout << "Empty temp folder" << endl;
		}
		else if (choice == 0) {
			return 0;
			cout << "Goodbye!" << endl;
		}
		else {
			//Clear the input buffer so that the programd oes not go into an infinite loop.
			std::cin.clear();
		    std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Please select a valid option." << endl;
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