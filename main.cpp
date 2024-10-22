#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

// Symbol types
enum SymbolType {
    // Keywords
    S_IF, S_ELSE, S_CIN, S_COUT, S_RETURN,
    // Operators
    S_PLUS, S_MINUS, S_MUL, S_DIV, S_GREAT, S_LESS, S_EQ, S_GEQ, S_LEQ, S_NEQ,
    S_IS, S_QUOTA, S_LBRA, S_RBRA, S_SEMI,
    // Input and output
    S_IN, S_OUT,
    // Others
    S_ID, S_NUM, S_ERROR
};

// Structure for a symbol
struct Symbol {
    SymbolType type;
    string name;
};

// Structure for processing input
struct TInput {
    char character;
    int lineNumber;
    int position;
    bool end;
};

TInput input;
Symbol symbol;

char getCharacter(const string& text, int& index) {
    if (index < text.length()) {
        input.character = text[index];
        input.character = toupper(input.character);
        input.position++;
        index++;
        return input.character;
    } else {
        input.end = true;
        return '\0';
    }
}

// Lexical part of the compiler
const int k_error = 0,
        // Keywords
        k_if = 8,
        k_cin = 12,
        k_cout = 15,
        k_else = 16,
        k_return = 19,
        // Operators
        k_plus = 20,
        k_minus = 21,
        k_mul = 22,
        k_div = 23,
        k_less = 24,
        k_great = 25,
        k_eq = 28,
        k_geq = 29,
        k_leq = 30,
        k_neq = 32,
        k_is = 33,
        k_quota = 34,
        k_lbra = 35,
        k_rbra = 36,
        k_semi = 37,
        // Input and output
        k_out = 26,
        k_in = 27;

int lexKeywords(const string& word) {
    // Check for numbers
    if (word[0] >= '0' && word[0] <= '9') {
        for (char c : word) {
            if (c < '0' || c > '9') {
                return S_ERROR;
            }
        }
        return S_NUM;
    }

    int state = 1;
    int pos = 0;
    while ((pos < word.length()) && (state != k_error)) {
        char character = toupper(word[pos]);
        switch (state) {
            case 1:
                switch (character) {
                    case 'C': state = 2; break;
                    case 'I': state = 3; break;
                    case 'E': state = 4; break;
                    case 'R': state = 5; break;
                    // Operators
                    case '+': state = 20; break;
                    case '-': state = 21; break;
                    case '*': state = 22; break;
                    case '/': state = 23; break;
                    case '<': state = 24; break;
                    case '>': state = 25; break;
                    case '=': state = 28; break;
                    case '!': state = 31; break;
                    case '"': state = 34; break;
                    case '(': state = 35; break;
                    case ')': state = 36; break;
                    case ';': state = 37; break;
                    default: state = k_error; break;
                }
                break;
            case 2:
                if (character == 'O') state = 6;
                else if (character == 'I') state = 7; else state = k_error;
                break;
            case 3:
                if (character == 'F') state = k_if; else state = k_error;
                break;
            case 4:
                if (character == 'L') state = 9; else state = k_error;
                break;
            case 5:
                if (character == 'E') state = 10; else state = k_error;
                break;
            case 6:
                if (character == 'U') state = 11; else state = k_error;
                break;
            case 7:
                if (character == 'N') state = k_cin; else state = k_error;
                break;
            case 9:
                if (character == 'S') state = 13; else state = k_error;
                break;
            case 10:
                if (character == 'T') state = 14; else state = k_error;
                break;
            case 11:
                if (character == 'T') state = k_cout; else state = k_error;
                break;
            case 13:
                if (character == 'E') state = k_else; else state = k_error;
                break;
            case 14:
                if (character == 'U') state = 17; else state = k_error;
                break;
            case 17:
                if (character == 'R') state = 18; else state = k_error;
                break;
            case 18:
                if (character == 'N') state = k_return; else state = k_error;
                break;
            case 24:
                if (character == '<') state = k_out;
                else if (character == '=') state = k_leq; else state = k_error;
                break;
            case 25:
                if (character == '>') state = k_in;
                else if (character == '=') state = k_geq; else state = k_error;
                break;
            case 28:
                if (character == '=') state = k_is; else state = k_error;
                break;
            case 31:
                if (character == '=') state = k_neq; else state = k_error;
                break;
            default:
                state = k_error;
                break;
        }
        pos++;
    }

    switch (state) {
        // Keywords
        case k_if: return S_IF;
        case k_else: return S_ELSE;
        case k_cin: return S_CIN;
        case k_cout: return S_COUT;
        case k_return: return S_RETURN;
        // Operators
        case k_plus: return S_PLUS;         // +
        case k_minus: return S_MINUS;       // -
        case k_mul: return S_MUL;           // *
        case k_div: return S_DIV;           // /
        case k_less: return S_LESS;         // <
        case k_great: return S_GREAT;       // >
        case k_eq: return S_EQ;             // =
        case k_geq: return S_GEQ;           // >=
        case k_leq: return S_LEQ;           // <=
        case k_neq: return S_NEQ;           // !=
        case k_is: return S_IS;             // ==
        case k_quota: return S_QUOTA;       // "
        case k_lbra: return S_LBRA;         // (
        case k_rbra: return S_RBRA;         // )
        case k_semi: return S_SEMI;         // ;
        // Input and output
        case k_in: return S_IN;             // >>
        case k_out: return S_OUT;           // <<n
        // Unknown, therefore identifiers
        default: return S_ID;
    }
}

int main() {
    string text;

    cout << "Enter text: ";
    getline(cin, text);  // Read the entire line including spaces

    input.lineNumber = 1;  // Initialize line number
    input.position = 0;     // Initialize position
    input.end = false;      // Initialize end

    // Split the text into words
    stringstream ss(text);
    string word;

    // Analyze word by word
    while (ss >> word) {
        int type = lexKeywords(word);
        
        switch (type) {
            case S_IF: cout << word << " -> S_IF" << endl; break;
            case S_ELSE: cout << word << " -> S_ELSE" << endl; break;
            case S_CIN: cout << word << " -> S_CIN" << endl; break;
            case S_COUT: cout << word << " -> S_COUT" << endl; break;
            case S_RETURN: cout << word << " -> S_RETURN" << endl; break;
            case S_PLUS: cout << word << " -> S_PLUS" << endl; break;
            case S_MINUS: cout << word << " -> S_MINUS" << endl; break;
            case S_MUL: cout << word << " -> S_MUL" << endl; break;
            case S_DIV: cout << word << " -> S_DIV" << endl; break;
            case S_ID: cout << word << " -> S_ID" << endl; break;
            case S_NUM: cout << word << " -> S_NUM" << endl; break;
            case S_LESS: cout << word << " -> S_LESS" << endl; break;
            case S_GREAT: cout << word << " -> S_GREAT" << endl; break;
            case S_EQ: cout << word << " -> S_EQ" << endl; break;
            case S_LEQ: cout << word << " -> S_LEQ" << endl; break;
            case S_GEQ: cout << word << " -> S_GEQ" << endl; break;
            case S_IN: cout << word << " -> S_IN" << endl; break;
            case S_OUT: cout << word << " -> S_OUT" << endl; break;
            case S_NEQ: cout << word << " -> S_NEQ" << endl; break;
            case S_IS: cout << word << " -> S_IS" << endl; break;
            case S_QUOTA: cout << word << " -> S_QUOTA" << endl; break;
            case S_LBRA: cout << word << " -> S_LBRA" << endl; break;
            case S_RBRA: cout << word << " -> S_RBRA" << endl; break;
            case S_SEMI: cout << word << " -> S_SEMI" << endl; break;
            default: cout << word << " -> S_ERROR" << endl; break;
        }
    }

    // Wait for a key press before ending the program
    cout << "Press Enter to exit the program..." << endl;
    cin.get(); // Wait for a key press

    return 0;
}
