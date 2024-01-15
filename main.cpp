#include <iostream>
#include <random>

using namespace std;

char matrix[10][8];
const int ROWS = 10;
const int COLS = 8;
const char* colorsArr = "   gorpb";
const int colorsArrLength = 8;

int randomNumInRange(int min, int max){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

void clearMatrix(){
    for(int i=0; i < ROWS; i++)
        for(int j=0; j < COLS; j++) matrix[i][j] = ' ';
}

void fillRow(int row){
    int nextCoord = 0;
    while(nextCoord < COLS){
        int lastCoord = nextCoord;
        nextCoord += randomNumInRange(1,4);
        if (nextCoord >= COLS) nextCoord = COLS;
        char curColor = colorsArr[randomNumInRange(0,colorsArrLength-1)];
        for(int i=lastCoord; i < nextCoord; i++)
            matrix[row][i] = curColor;
    }
}

void printMatrix(){
    for(int i = 0; i < ROWS; i++){
        cout << '|';
        for(int j = 0; j < COLS; j++){
            cout << matrix[i][j];
        }
        cout << '|' << endl;
    }
}

bool checkIfRowFull(int row){
    for(int i=0; i<COLS; i++)
        if(matrix[row][i] == ' ') return false;
    return true;
}

bool checkIfRowEmpty(int row){
    for(int i=0; i<COLS-1; i++)
        if(!(matrix[row][i] == ' ' && matrix[row][i] == matrix[row][i+1])) return false;
    return true;
}

int main() {
    clearMatrix();
    printMatrix();
    while(true){
        char a;
        cin>>a;
        fillRow(ROWS-1);
        printMatrix();
    }
    return 0;
}
