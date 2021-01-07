/**
*
* Solution to course project # 7
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2020/2021
*
* @author Mihaela Ilieva
* @idnumber 62550
* @compiler VC
*
* <file with helper functions>
*
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main() {

	//the specialSymbols array keeps the symbols that are regex operations
	char specialSymbols[] = { '^','.','*','+','?','\\','\0' };

   // insert the command for the file
	string regexCommand;

	//validation
	cin >> regexCommand;

	// 
	string textFileName;

	cin >> textFileName;

	fstream myFile;

	myFile.open(textFileName, fstream::in);

	string textFromFile;

	while (!myFile.eof()) {
		string currentLine;
		getline(myFile, currentLine);
		textFromFile += currentLine+"\n";
	}

	cout << textFromFile;
	return 0;
}