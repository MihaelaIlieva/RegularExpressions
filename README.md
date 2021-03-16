# RegularExpressions
A university task
This project is a simplified version of Regex. The user can choose from the following special symbols to include in their expression:
1. ‘^’ - expected only in the beginning of the expression; shows all matches that are on a new line
2. ‘.’ - one random symbol, there are no restrictions for it
3. ‘*’ - looking for 0 or many symbols that are the same as the one before it; it cannot be in the beginning
4. ‘+’ - looking for 1 or many symbols that are the same as the one before it; it cannot be in the beginning
5. ‘?’ - looking for 0 or 1 of the symbol that is before it; it cannot be in the beginning
6. ‘\’ - placed before a special symbol in order to escape it( treat it like any other ordinary symbol)

If the expression is invalid, the user will be asked to enter a valid one. The same goes for the name of the file where matches will be searched.
All matches will be displayed, if there are any.
