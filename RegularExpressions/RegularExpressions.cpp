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

// seeks out as many symbols as possible that are the same as the symbol before the '*' operator
// sets the index to the last found match +1 and returns true
bool CaughtZeroOrManySymbols(char searchedChar, int& currentIndexInText, string text) {
	while (text[currentIndexInText] == searchedChar && currentIndexInText >= 0) {
		currentIndexInText--;
	}
	currentIndexInText++;
	return true;
}

// seeks out as many symbols as possible (at least one) that are the same as the symbol before the '+' operator
// sets the index to the last found match +1 and returns if there is at least one symbol equal to the one before '+'
bool CaughtOneOrManySymbols(char searchedChar, int& currentIndexInText, string text) {
	bool atLeastOne = false;
	while (text[currentIndexInText] == searchedChar && currentIndexInText >= 0)
	{
		atLeastOne = true;
		currentIndexInText--;
	}
	currentIndexInText++;
	return atLeastOne;
}

// seeks out if there are zero or one symbols that are the same as the symbol before the '?' operator
// sets the index to the last found match +1 and returns if there are zero or one symbols equal to the one before '?'
bool CaughtZeroOrOneSymbols(char searchedChar, int& currentIndexInText, string text) {
	if (text[currentIndexInText] == searchedChar && currentIndexInText >= 0) {
		currentIndexInText--;
	}
	currentIndexInText++;
	return true;
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

// if the regex is only of ordinary symbols, we check if it is contained in the text from the file
bool IsRegexExpressionContainedInString(string regex, string text)
{
	int IndexOfRegexExpression = regex.size() - 1;
	for (int i = text.size() - 1; i >= 0; i--) {

		if (regex[IndexOfRegexExpression] == '*') {
			if (CaughtZeroOrManySymbols(regex[IndexOfRegexExpression - 1], i, text)) {
				IndexOfRegexExpression-=2;
			}
			else {
				IndexOfRegexExpression = regex.size() - 1;
			}
		}
		else if (regex[IndexOfRegexExpression] == '+') {
			if (CaughtOneOrManySymbols(regex[IndexOfRegexExpression - 1], i, text)) {
				IndexOfRegexExpression -= 2;
			}
			else {
				IndexOfRegexExpression = regex.size() - 1;
			}
		}
		else if (regex[IndexOfRegexExpression] == '?') {
			if (CaughtZeroOrOneSymbols(regex[IndexOfRegexExpression - 1], i, text)) {
				IndexOfRegexExpression -= 2;
			}
			else {
				IndexOfRegexExpression = regex.size() - 1;
			}
		}
		else {
			//cout << s[i];
		//If it is just a normal symbol
			//we try to match it with the string symbol
			if (text[i] == regex[IndexOfRegexExpression]) {
				// on success we move on to the next symbol for matching
				IndexOfRegexExpression--;
			}
			else {
				//on fail we reset try again to match the last symbol of the expression 
				IndexOfRegexExpression = regex.size() - 1;
			}
			if (IndexOfRegexExpression < 0) {
				return true;
			}
		}

		
	}
	return false;
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

	cout << IsRegexExpressionContainedInString(regexCommand, textFromFile);

	return 0;
}