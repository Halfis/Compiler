#include <iostream>
#include <string>
#include <cctype>

using namespace std;

enum SymbolType {
    S_IF, S_ELSE, S_OR, S_AND, S_PRINT,
    S_PLUS, S_MINUS, S_MUL, S_DIV, S_GREAT,
    S_LESS, S_EQ, S_GEQ, S_LEQ, S_NEQ,
    S_IS, S_LPAR, S_RPAR, S_LCUR, S_RCUR, S_SEMI,
    S_ID, S_INT, S_EOF, S_ERROR
};

class Symbol {
public:
    SymbolType type;
    string name;
    double number;
    char character;
    int lineNumber;
    int position;

    Symbol() : lineNumber(1), position(0), character('\0') {}
};

Symbol currentSymbol;
string inputText;
int textIndex = 0;

enum {
    ERROR_STATE = -1, STATE_IF, STATE_ELSE, STATE_AND, STATE_OR, STATE_PRINT
};

char ReadChar() {
    if (textIndex < inputText.size()) {
        currentSymbol.character = inputText[textIndex++];
        currentSymbol.position++;
        return currentSymbol.character;
    } else {
        currentSymbol.character = '\0';
        return currentSymbol.character;  // End of input
    }
}

char NewLine() {
    currentSymbol.lineNumber++;
    currentSymbol.position = 0;
    return ReadChar();
}

// Lexical Analysis
Symbol LexicalAnalysis() {
    while (true) {
        // Skip whitespace
        if (isspace(currentSymbol.character)) {
            if (currentSymbol.character == '\n') NewLine();
            else ReadChar();
        } else break;
    }

    if (currentSymbol.character == '\0') {
        currentSymbol.type = S_EOF;
        return currentSymbol;
    }

    // Recognizing numbers
    if (isdigit(currentSymbol.character)) {
        currentSymbol.number = 0;
        while (isdigit(currentSymbol.character)) {
            currentSymbol.number = currentSymbol.number * 10 + (currentSymbol.character - '0');
            ReadChar();
        }
        
        // Check if the number has a decimal point, treat it as an error if true
        if (currentSymbol.character == '.') {
            currentSymbol.type = S_ERROR;
            while (isdigit(ReadChar())) {}
            return currentSymbol;
        }
        // If the next character is a letter, it's an invalid token like "56asd"
        else if (isalpha(currentSymbol.character)) {
            currentSymbol.type = S_ERROR;
            while (isalnum(ReadChar())) {}
            return currentSymbol;
        } else {
            currentSymbol.type = S_INT;
            return currentSymbol;
        }
    }
    
    // Recognizing operators and special symbols
    switch (currentSymbol.character) {
        case '+': currentSymbol.type = S_PLUS; ReadChar(); return currentSymbol;
        case '-': currentSymbol.type = S_MINUS; ReadChar(); return currentSymbol;
        case '*': currentSymbol.type = S_MUL; ReadChar(); return currentSymbol;
        case '/': currentSymbol.type = S_DIV; ReadChar(); return currentSymbol;
        case '>':
            if (ReadChar() == '=') { currentSymbol.type = S_GEQ; ReadChar(); }
            else { currentSymbol.type = S_GREAT; }
            return currentSymbol;
        case '<':
            if (ReadChar() == '=') { currentSymbol.type = S_LEQ; ReadChar(); }
            else { currentSymbol.type = S_LESS; }
            return currentSymbol;
        case '=':
            if (ReadChar() == '=') { currentSymbol.type = S_EQ; ReadChar(); }
            else { currentSymbol.type = S_IS; }
            return currentSymbol;
        case '!':
            if (ReadChar() == '=') { currentSymbol.type = S_NEQ; ReadChar(); }
            else { currentSymbol.type = S_ERROR; currentSymbol.name = "Error: Invalid symbol !"; }
            return currentSymbol;
        case '(': currentSymbol.type = S_LPAR; ReadChar(); return currentSymbol;
        case ')': currentSymbol.type = S_RPAR; ReadChar(); return currentSymbol;
        case '{': currentSymbol.type = S_LCUR; ReadChar(); return currentSymbol;
        case '}': currentSymbol.type = S_RCUR; ReadChar(); return currentSymbol;
        case ';': currentSymbol.type = S_SEMI; ReadChar(); return currentSymbol;
    }

    // Recognizing identifiers and keywords
    int state = 1;
    currentSymbol.name.clear();

    while (state != ERROR_STATE && currentSymbol.character != '\0' && isalpha(currentSymbol.character)) {
        char charUpper = toupper(currentSymbol.character);
        switch (state) {
            case 1:
                switch (charUpper) {
                    case 'I': state = 2; break;
                    case 'A': state = 3; break;
                    case 'O': state = 4; break;
                    case 'E': state = 5; break;
                    case 'P': state = 6; break;
                    default: state = ERROR_STATE; break;
                }
                break;
            case 2:
                if (charUpper == 'F') state = STATE_IF; else state = ERROR_STATE;
                break;
            case 3:
                if (charUpper == 'N') state = 8; else state = ERROR_STATE;
                break;
            case 4:
                if (charUpper == 'R') state = STATE_OR; else state = ERROR_STATE;
                break;
            case 5:
                if (charUpper == 'L') state = 10; else state = ERROR_STATE;
                break;
            case 6:
                if (charUpper == 'R') state = 11; else state = ERROR_STATE;
                break;
            case 8:
                if (charUpper == 'D') state = STATE_AND; else state = ERROR_STATE;
                break;
            case 10:
                if (charUpper == 'S') state = 13; else state = ERROR_STATE;
                break;
            case 11:
                if (charUpper == 'I') state = 14; else state = ERROR_STATE;
                break;
            case 13:
                if (charUpper == 'E') state = STATE_ELSE; else state = ERROR_STATE;
                break;
            case 14:
                if (charUpper == 'N') state = 16; else state = ERROR_STATE;
                break;
            case 16:
                if (charUpper == 'T') state = STATE_PRINT; else state = ERROR_STATE;
                break;
            default:
                state = ERROR_STATE;
                break;
        }
        
        // Store character and load next
        if (state != ERROR_STATE) {
            currentSymbol.name += currentSymbol.character;
            ReadChar();
        } else {
            break;
        }
    }

    // Determine symbol type based on final state
    switch (state) {
        case STATE_IF: currentSymbol.type = S_IF; break;
        case STATE_ELSE: currentSymbol.type = S_ELSE; break;
        case STATE_AND: currentSymbol.type = S_AND; break;
        case STATE_OR: currentSymbol.type = S_OR; break;
        case STATE_PRINT: currentSymbol.type = S_PRINT; break;
        default: currentSymbol.type = S_ID; break;
    }

    return currentSymbol;
}

// Main function
int main() {
    cout << "Enter text: ";
    getline(cin, inputText);
    currentSymbol.character = ReadChar();

    // Perform lexical analysis
    do {
        currentSymbol = LexicalAnalysis();
        switch (currentSymbol.type) {
            case S_IF: cout << "S_IF\n"; break;
            case S_ELSE: cout << "S_ELSE\n"; break;
            case S_OR: cout << "S_OR\n"; break;
            case S_AND: cout << "S_AND\n"; break;
            case S_PRINT: cout << "S_PRINT\n"; break;
            case S_ID: cout << currentSymbol.name << " -> S_ID\n"; break;
            case S_INT: cout << currentSymbol.number << " -> S_INT\n"; break;
            case S_PLUS: cout << "S_PLUS\n"; break;
            case S_MINUS: cout << "S_MINUS\n"; break;
            case S_MUL: cout << "S_MUL\n"; break;
            case S_DIV: cout << "S_DIV\n"; break;
            case S_GREAT: cout << "S_GREAT\n"; break;
            case S_LESS: cout << "S_LESS\n"; break;
            case S_GEQ: cout << "S_GEQ\n"; break;
            case S_LEQ: cout << "S_LEQ\n"; break;
            case S_IS: cout << "S_IS\n"; break;
            case S_EQ: cout << "S_EQ\n"; break;
            case S_NEQ: cout << "S_NEQ\n"; break;
            case S_LPAR: cout << "S_LPAR\n"; break;
            case S_RPAR: cout << "S_RPAR\n"; break;
            case S_LCUR: cout << "S_LCUR\n"; break;
            case S_RCUR: cout << "S_RCUR\n"; break;
            case S_SEMI: cout << "S_SEMI\n"; break;
            case S_EOF: cout << "S_EOF\n"; break;
            default: cout << "ERROR\n"; break;
        }
    } while (currentSymbol.type != S_EOF);

    return 0;
}
