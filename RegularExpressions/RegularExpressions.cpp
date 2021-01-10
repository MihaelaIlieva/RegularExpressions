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
#include <vector>

using namespace std;

//the specialSymbols array keeps the symbols that are regex operations
char specialSymbols[] = { '^','.','*','+','?','\\','\0' };

bool AnySymbol(string searchedString, int& currentIndexInText, string text) {
	if (currentIndexInText >= (int)text.size()) {
		return false;
	}
	else {
		currentIndexInText++;
		return true;
	}
}
// ^
bool Anchored(string searchedString, int& currentIndexInText, string text) {
	return currentIndexInText == 0;
}

// seeks out as many symbols as possible that are the same as the symbol before the '*' operator
// sets the index to the last found match +1 and returns true
bool CaughtZeroOrManySymbols(string searchedString, int& currentIndexInText, string text) {
	while (text[currentIndexInText] == searchedString[0] && currentIndexInText >= 0) {
		currentIndexInText++;
	}
	
	return true;
}

// seeks out as many symbols as possible (at least one) that are the same as the symbol before the '+' operator
// sets the index to the last found match +1 and returns if there is at least one symbol equal to the one before '+'
bool CaughtOneOrManySymbols(string searchedString, int& currentIndexInText, string text) {
	bool atLeastOne = false;
	while (text[currentIndexInText] == searchedString[0] && currentIndexInText >= 0) {
		atLeastOne = true;
		currentIndexInText++;
	}	
	return atLeastOne;
}

// seeks out if there are zero or one symbols that are the same as the symbol before the '?' operator
// sets the index to the last found match +1 and returns if there are zero or one symbols equal to the one before '?'
bool CaughtZeroOrOneSymbols(string searchedString, int& currentIndexInText, string text) {
	if (text[currentIndexInText] == searchedString[0] && currentIndexInText >= 0) {
		currentIndexInText++;
	}

	return true;
}
bool CaughtString(string toMatchWith, int& currentTextIndex, string text) {

	int indexOfStringToMatchWith = 0;
	int originalIndex = currentTextIndex;
	if (toMatchWith.empty()) {
		return true;
	}
	while (text[currentTextIndex] == toMatchWith[indexOfStringToMatchWith] && indexOfStringToMatchWith <= (int)toMatchWith.size()
		&& currentTextIndex <= (int)text.size()) {
		indexOfStringToMatchWith++;
		currentTextIndex++;
		if (indexOfStringToMatchWith > (int)toMatchWith.size() - 1) {
			
			return true;
		}
	}
	currentTextIndex = originalIndex;
	return false;
}
// checking if a given regex command is valid based on the given criteria

vector< pair<string, bool (*)(string searchedChar, int& currentIndexInText, string text)>> functions;

string PushToCaughtStringAndGetLastSymbol(string& toBeCaught) {
	
	string lastSymbol = string(1, toBeCaught.back());
	toBeCaught.pop_back();
	if (!toBeCaught.empty()) {
		functions.push_back({ toBeCaught,CaughtString });
	}
	//clearing it
	toBeCaught = "";
	return lastSymbol;
}

bool ConvertRegexexpressionToFunctions(string regex) {
	string toBeCaught = "";
	int specialsymbolscount = 0;

	
		for (size_t i = 0; i < regex.size(); i++) {
			if (regex[i] == '*') {
				if (toBeCaught.empty()) {
					return false;
				}
				string lastSymbol = PushToCaughtStringAndGetLastSymbol(toBeCaught);
				functions.push_back({ lastSymbol,CaughtZeroOrManySymbols });

			}
			else if (regex[i] == '+') {
				if (toBeCaught.empty()) {
					return false;
				}
				string lastSymbol = PushToCaughtStringAndGetLastSymbol(toBeCaught);
				functions.push_back({ lastSymbol,CaughtOneOrManySymbols });

			}
			else if (regex[i] == '?') {
				if (toBeCaught.empty()) {
					return false;
				}
				string lastSymbol = PushToCaughtStringAndGetLastSymbol(toBeCaught);
				functions.push_back({ lastSymbol, CaughtZeroOrOneSymbols });

			}
			else if (regex[i] == '\\') {

				if (i == regex.size() - 1) {
					return false;
				}
				// isSpecialSymbol shows if the symbol after the '\' is special
				bool isSpecialSymbol = false;
				// checking if the symbol after the '\'is special
				for (size_t j = 0; j < 6; j++) {

					if (regex[i + 1] == specialSymbols[j]) {
						isSpecialSymbol = true;
					}
				}
				// if the next symbol is not special, the regex is invalid
				if (!isSpecialSymbol) {
					return false;
				}
				else {
					char escapedSymbol = regex[i + 1];
					toBeCaught.push_back(escapedSymbol);
					i++;
				}
							
			}
			else if (regex[i] == '^') {
				if (i != 0) {
					return false;
				}
				functions.push_back({ "",Anchored });
			}
			else if (regex[i] == '.') {
				if (!toBeCaught.empty()) {
					functions.push_back({ toBeCaught,CaughtString });
				}
				//clearing it
				toBeCaught = "";
				functions.push_back({ "",AnySymbol });
			}
			// if is an ordinary symbol
			else {
				toBeCaught.push_back(regex[i]);
			}
		}
		
		if (!toBeCaught.empty()) {
			functions.push_back({ toBeCaught,CaughtString });
		}
	return true;
}

//changed if regex is contained in text with the newly implemented logic and now the function counts the occurences of the regex in text
void OccurrencesOfRegexExpressionInString(string text)
{
	int index = 0;
	int numberOfCaught = 0;
	for (int i = 0; i <= (int)text.size() - 1; i++) {
		bool caught = true;
		int startIndexOfCaughtMatch = i;
		for (size_t j = 0; j < functions.size(); j++) {
			if (!functions[j].second(functions[j].first, i, text)) {
				caught = false;
				break;
			}
		}
		if (caught) {
			int lastIndexOfCaughtMatch = i;
			numberOfCaught++;
			for (int t = startIndexOfCaughtMatch; t < lastIndexOfCaughtMatch; t++) {
				cout << text[t];
			}
			cout << endl;
		}
	}
}

int main() {

   // inserting the command for the file	
	string regexCommand;
	cout << "Insert a regex command: \n";
	cin >> regexCommand;
	// validating the command and waiting for a valid one to be inserted in order to start working
	while (!ConvertRegexexpressionToFunctions(regexCommand)) {
		cout << " Please insert a valid regex command! : \n";
		functions.clear();
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
	string* linesToCheck=NULL;
	int countOfLines = 0;
	while (!myFile.eof()) {
		string currentLine;
		getline(myFile, currentLine);
		countOfLines++;
	}
	linesToCheck = new string[countOfLines];
	myFile.clear();
	myFile.seekg(0);
	//adding each line of the file to the textFromFile string untill we have reached the end of the file
	for (int i = 0; i < countOfLines; i++) {
		string currentLine;
		getline(myFile, currentLine);
		linesToCheck[i] = currentLine;
	}
	//cout << IsRegexValid(regexCommand);
	/*cout << textFromFile;*/

	myFile.close();

	for (int i = 0; i < countOfLines; i++) {
		string a = linesToCheck[i];
		OccurrencesOfRegexExpressionInString(linesToCheck[i]);
	}
	
	return 0;
}