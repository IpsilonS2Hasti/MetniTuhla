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

void shiftMatrixUp(){
    for(int i=0; i<ROWS-1; i++){
        for(int j=0; j<COLS; j++)
            matrix[i][j] = matrix[i+1][j];
    }
    for(int i=0; i<COLS; i++)
        matrix[ROWS-1][i] = ' ';
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
    cout << "  ";
    for(int i=0; i < COLS; i++)
        cout << i << ' ';
    cout << endl;
    for(int i = 0; i < ROWS; i++){
        cout << ROWS-1-i << '|';
        for(int j = 0; j < COLS; j++){
            if(isupper(matrix[i][j])) {
                cout << getColor(matrix[i][j]) << "{]" << RESET; // Start of the brick
            } else if(matrix[i][j] != ' ') {
                cout << getColor(matrix[i][j]) << "[]" << RESET; // Continuation of the brick
            } else {
                cout << "  ";
            }
        }
        cout << '|'  << ROWS-1-i << endl;
    }
    cout << "  ";
    for(int i=0; i < COLS; i++)
        cout << i << ' ';
    cout << endl;
}

int getBrickLength(int x, int y){
    int i=y+1;
    while(i < COLS && !isUpper(matrix[x][i]) && matrix[x][i] != ' ') i++;
    return i-y;
}

void moveBrick(int x, int y, int offset){
    int length = getBrickLength(x, y);
    char tempBrick[5] = {' ', ' ', ' ', ' '};
    for(int i=0; i<=length-1; i++){
        tempBrick[i] = matrix[x][y+i];
        matrix[x][y+i] = ' ';
    }
    for(int i=0; i<=length-1; i++)
        matrix[x][y+offset+i] = tempBrick[i];
}

void processPlayerInput(){
    bool inputInvalid = true;

    while(inputInvalid){
        int x, y, moveAmount;
        char direction;
        cin >> x >> y >> direction >> moveAmount;
        x=ROWS-1-x;

        if (!cin) {
            cout << "Invalid input" << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if(x < 0 || x > ROWS-1 || y < 0 || y > COLS-1 || moveAmount <= 0 || moveAmount >= COLS-1 || !(direction == 'r' || direction == 'l')){
            cout<< "Invalid input" << endl;
            continue;
        }

        //check for collisions
        bool hasCollided = false;
        if(direction == 'l'){
            if(y-moveAmount < 0) hasCollided = true;
            else{
                for(int i=1; i<=moveAmount; i++){
                    if(matrix[x][y-i] != ' '){
                        hasCollided = true;
                        break;
                    }
                }
            }
            if(!hasCollided) moveBrick(x, y, -moveAmount);

        }else{
            if(y+moveAmount > COLS-1) hasCollided = true;
            else{
                for(int i=1; i<=moveAmount; i++){
                    if(matrix[x][y+getBrickLength(x, y)-1+i] != ' '){
                        hasCollided = true;
                        break;
                    }

                }
            }
            if(!hasCollided) moveBrick(x, y, moveAmount);
        }
        if(hasCollided) cout<< "Illegal move" << endl;

        inputInvalid = false;
    }
}

int main() {
    cout << "Welcome to Metni Tuhla!" << endl;
    clearMatrix();
    printMatrix();
    int points = 0;
    while(true){
        cout << "Enter command: " << endl;

        //Calculate player move and its consequences
        processPlayerInput();

        shiftMatrixUp();

        //Game Over condition
        if(!rowIsEmpty(0)) break;

        reRow:
        fillRow(ROWS-1);
        if(rowIsFilled(ROWS-1)) {
            points += 10;
            goto reRow;
        }
        if(rowIsEmpty(ROWS-1)) goto reRow;

        printMatrix();
    }
    return 0;
}
