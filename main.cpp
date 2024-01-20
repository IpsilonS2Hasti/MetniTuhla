#include <iostream>
#include <random>

using namespace std;

const int ROWS = 10;
const int COLS = 8;
char matrix[ROWS][COLS];
const char* colorsArr = "   GORPB";
const int colorsArrLength = 8;

// ANSI color codes
const char* RED = "\033[31m";
const char* GREEN = "\033[32m";
const char* ORANGE = "\033[38;5;214m";
const char* BLUE = "\033[34m";
const char* PURPLE = "\033[35m";
const char* RESET = "\033[0m";

int randomNumInRange(int min, int max){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

bool isUpper(char a){
    return a >= 'A' && a <= 'Z';
}

char toLower(char a){
    if(isUpper(a)) a+=32;
    return a;
}

void clearMatrix(){
    for(int i = 0; i < ROWS; i++)
        for(int j = 0; j < COLS; j++) matrix[i][j] = ' ';
}

void fillRow(int row){
    int nextCoord = 0;
    while(nextCoord < COLS){
        int lastCoord = nextCoord;
        nextCoord += randomNumInRange(1, 4);
        if (nextCoord > COLS) nextCoord = COLS;
        char curColor = colorsArr[randomNumInRange(1, colorsArrLength-1)]; // Capital letter - start of brick
        matrix[row][lastCoord] = curColor;
        for(int i = lastCoord + 1; i < nextCoord; i++)
            matrix[row][i] = toLower(curColor); // Lower letter - continuation of the brick
    }
}

bool rowIsFilled(int row){
    for(int i=0; i<COLS; i++)
        if(matrix[row][i] == ' ') return false;
    return true;
}

bool rowIsEmpty(int row){
    for(int i=0; i<COLS; i++)
        if(matrix[row][i] != ' ') return false;
    return true;
}

const char* getColor(char block) {
    switch(toLower(block)) {
        case 'g': return GREEN;
        case 'o': return ORANGE;
        case 'r': return RED;
        case 'p': return PURPLE;
        case 'b': return BLUE;
        default: return "";
    }
}

void printMatrix(){
    for(int i = 0; i < ROWS; i++){
        cout << '|';
        for(int j = 0; j < COLS; j++){
            if(isupper(matrix[i][j])) {
                cout << getColor(matrix[i][j]) << "{]" << RESET; // Start of the brick
            } else if(matrix[i][j] != ' ') {
                cout << getColor(matrix[i][j]) << "[]" << RESET; // Continuation of the brick
            } else {
                cout << "  ";
            }
        }
        cout << '|' << endl;
    }
}

int main() {
    cout << "Welcome to Metni Tuhla!" << endl;
    clearMatrix();
    printMatrix();
    while(true){
        cout << "Enter command: " << endl;
        char a;
        cin >> a;
        if(a == 'q') break;
        rowCheck:
        fillRow(ROWS-1);
        if(rowIsFilled(ROWS-1)) goto rowCheck;
        printMatrix();
    }
    return 0;
}
