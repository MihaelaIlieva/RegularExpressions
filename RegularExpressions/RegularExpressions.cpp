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
#include <string>

using namespace std;

//the specialSymbols array keeps the symbols that are regex operations
char specialSymbols[] = { '^','.','*','+','?','\\','\0' };

// checking if a given regex command is valid based on the given criteria
bool IsRegexValid(string regex) {
	//keeping the count of the special symbols in order not to exceed the limit
	int specialSymbolsCount = 0;
	for (size_t i = 0; i < regex.size(); i++) {
		// if there is a symbol before the '^', the regex command is invalid
		if (regex[i] == '^' && i != 0) {
			return false;
		}
		// if '+','*' or '?' is the first symbol the regex command is invalid as they work with
		//the previous element and in this case there is no such element
		if ((regex[i] == '+'|| regex[i] == '*'|| regex[i] == '?') && i == 0) {
			return false;
		}
		// if the symbol that escapes the special symbols is in last place, the regex is invalid
		// there is no symbol to be escaped
		else if (regex[i] == '\\' ) {
			if (i == regex.size() - 1 ) {
				return false;
			}
			// isSpecialSymbol shows if the symbol after the '\' is special
			bool isSpecialSymbol = false;
			// checking if the symbol after the '\'is special
			for (size_t j = 0; j < 6; j++) {
				
				if (regex[i+1]== specialSymbols[j]) {
					isSpecialSymbol = true;
				}
			}
			// if the next symbol is not special, the regex is invalid
			if (!isSpecialSymbol) {
				return false;
			}
			i++;
		}
		// checking how many of those special symbols are there in the regex command
		else if (regex[i] == '*' || regex[i] == '+' || regex[i] == '?') {
			specialSymbolsCount++;
		}
		// if there are two '.' next to each other, the command is invalid, the case about the other
		// such special symbols is already checked in the previous else if
		else if (i<regex.size()-1 && regex[i] == '.' && regex[i + 1] == regex[i]) {
			return false;
		}
		
	}
	// if the special symbols '*','+' and '?' are found two or more times, the command is invalid
	if (specialSymbolsCount>=2) {
		return false;
	}
	else {
		return true;
	}
}
int main() {

   // inserting the command for the file	
	string regexCommand;
	cout << "Insert a regex command: \n";
	cin >> regexCommand;
	// validating the command and waiting for a valid one to be inserted in order to start working
	while (!IsRegexValid(regexCommand)) {
		cout << " Please insert a valid regex command! : \n";
		cin >> regexCommand;
	}
	// inserting the name of the text file
	string textFileName;
	cout << "Insert a file name in which the program should check for matches: \n";
	cin >> textFileName;

	// opening the file with the name given by the user
	fstream myFile;
	myFile.open(textFileName, fstream::in);
	//checking if there is a problem opening the file and asking for a new file that the program can work with
	while (!myFile.is_open()) {
		cout << "Please insert a valid file name in the correct dirrectory! : \n";
		cin >> textFileName;
		myFile.open(textFileName, fstream::in);
	}
	// textFromFile will keep the text from the file in order to work easier with the information
	string textFromFile;

	//adding each line of the file to the textFromFile string untill we have reached the end of the file
	while (!myFile.eof()) {
		string currentLine;
		getline(myFile, currentLine);
		textFromFile += currentLine+"\n";
	}
	//cout << IsRegexValid(regexCommand);
	/*cout << textFromFile;*/

	myFile.close();

	return 0;
}