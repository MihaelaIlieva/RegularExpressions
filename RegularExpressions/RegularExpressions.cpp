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
	if (currentIndexInText >= text.size()) {
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
	while (text[currentIndexInText] == searchedString[0] && currentIndexInText >= 0)
	{
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
	while (text[currentTextIndex] == toMatchWith[indexOfStringToMatchWith] && indexOfStringToMatchWith <= toMatchWith.size()
		&& currentTextIndex <= text.size()) {
		indexOfStringToMatchWith++;
		currentTextIndex++;
		if (indexOfStringToMatchWith > toMatchWith.size() - 1) {
			
			return true;
		}
	}
	currentTextIndex = originalIndex;
	return false;
}
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
vector< pair<string, bool (*)(string searchedChar, int& currentIndexInText, string text)>> functions;

void ConvertRegexexpressionToFunctions(string regex) {
	string toBeCaught = "";
	int specialsymbolscount = 0;

	for (size_t i = 0; i < regex.size(); i++) {
		if (regex[i] == '*') {

			string lastSymbol = string(1,toBeCaught.back());
			toBeCaught.pop_back();
			if (!toBeCaught.empty()) {
				functions.push_back({ toBeCaught,CaughtString });
			}
			//clearing it
			toBeCaught = "";
			functions.push_back({ lastSymbol,CaughtZeroOrManySymbols });

		}
		else if (regex[i] == '+') {

			string lastSymbol = string(1, toBeCaught.back());
			toBeCaught.pop_back();
			if (!toBeCaught.empty()) {
				functions.push_back({ toBeCaught,CaughtString });
			}
			//clearing it
			toBeCaught = "";
			functions.push_back({ lastSymbol,CaughtOneOrManySymbols });

		}
		else if (regex[i] == '?') {

			string lastSymbol = string(1, toBeCaught.back());
			toBeCaught.pop_back();
			if (!toBeCaught.empty()) {
				functions.push_back({ toBeCaught,CaughtString });
			}
			//clearing it
			toBeCaught = "";
			functions.push_back({ lastSymbol, CaughtZeroOrOneSymbols});

		}
		else if (regex[i] == '\\') {
			char escapedSymbol = regex[i + 1];
			toBeCaught.push_back(escapedSymbol);
			i++;
		}
		else if (regex[i] == '^') {
			functions.push_back({ "",Anchored });
		}
		else if (regex[i] == '.') {
			
			if (!toBeCaught.empty()) {
				functions.push_back({ toBeCaught,CaughtString });
			}
			//clearing it
			toBeCaught = "";
			functions.push_back({ "",AnySymbol});
		}
		// if is an ordinary symbol
		else {
			toBeCaught.push_back(regex[i]);
		}
	}
	if (!toBeCaught.empty()) {
		functions.push_back({ toBeCaught,CaughtString });
	}
}

//changed if regex is contained in text with the newly implemented logic and now the function counts the occurences of the regex in text
int OccurrencesOfRegexExpressionInString(string text)
{
	int index = 0;
	int numberOfCaught = 0;
	for (int i = 0; i <= text.size() - 1; i++) {
		bool caught = true;
		for (size_t j = 0; j < functions.size(); j++) {
			if (!functions[j].second(functions[j].first, i, text)) {
				caught = false;
				break;
			}
		}
		if (caught) {
			numberOfCaught++;
		}
	}
	return numberOfCaught;
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

	ConvertRegexexpressionToFunctions(regexCommand);
	cout << OccurrencesOfRegexExpressionInString(textFromFile);
	
	return 0;
}