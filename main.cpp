#include <iostream>
#include <random>

using namespace std;

const int ROWS = 10;
const int COLS = 8;
char matrix[ROWS][COLS];
const char* colorsArr = "   GORPB";
const int colorsArrLength = 8;
int score = 0;

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
    cout << "Current score: " << score << endl;
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
        x=ROWS-1-x; //account for the fact the game counts bottom to top

        if (!cin) {
            cout << "Invalid input" << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if(x < 0 || x > ROWS-1 || y < 0 || y > COLS-1 || moveAmount <= 0 || moveAmount > COLS-1 || !(direction == 'r' || direction == 'l')){
            cout<< "Invalid input" << endl;
            continue;
        }

        //check if coordinates point to start of brick (capital letter)
        if(!isUpper(matrix[x][y])){
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
        if(hasCollided) {
            cout<< "Illegal move" << endl;
            continue;
        }

        inputInvalid = false;
    }
}

bool hasSpaceToFall(int x, int y, int length){
    bool blockBelow = false;
    for (int i = 0; i < length; i++) {
        if(matrix[x+1][y+i] != ' ')  {
            blockBelow = true;
            break;
        }
    }
    return !blockBelow;
}

void fallBrick(int x, int y, int length){
    for (int i = 0; i < length; i++){
        matrix[x+1][y+i] = matrix[x][y+i];
        matrix[x][y+i] = ' ';
    }
}

void updateAllBricks(){
    bool changeHasOccurred = true;
    while(changeHasOccurred){
        changeHasOccurred = false;
        for(int curRow=0; curRow < ROWS-1; curRow++){
            for(int curCol=0; curCol < COLS; curCol++){
                int brickLength = 0;
                if(isUpper(matrix[curRow][curCol])){
                    brickLength = getBrickLength(curRow, curCol);
                    if(hasSpaceToFall(curRow, curCol, brickLength)){
                        fallBrick(curRow, curCol, brickLength);
                        changeHasOccurred = true;
                    }
                    brickLength--;
                }
                curCol+=brickLength;
            }
        }
    }
}

bool clearFilledRows(){
    bool hasPerformedOperation = false;
    for(int i=0; i < ROWS; i++){
        bool hasEmptySpace = false;
        for(int j=0; j < COLS; j++){
            if(matrix[i][j]==' ') {
                hasEmptySpace = true;
                break;
            }
        }
        if(!hasEmptySpace) {
            for(int j=0; j < COLS; j++) matrix[i][j] = ' ';
            score += 10;
            hasPerformedOperation = true;
        }
    }
    return hasPerformedOperation;
}

int main() {
    cout << "Welcome to Metni Tuhla!" << endl;
    clearMatrix();

    goto reRow; //Generate initial row

    while(true){
        cout << "Enter command: " << endl;

        //Handle player move and update bricks
        processPlayerInput();
        updateAllBricks();

        //Check for Filled rows and update if any
        while(clearFilledRows()) updateAllBricks();

        //Handle new row and update
        shiftMatrixUp();
        if(!rowIsEmpty(0)) break; //Game Over condition

        reRow:
        fillRow(ROWS-1);
        if(rowIsFilled(ROWS-1)) {
            score += 10;
            goto reRow;
        }
        if(rowIsEmpty(ROWS-1)) goto reRow;

        updateAllBricks();
        while(clearFilledRows()) updateAllBricks();

        //Print on screen
        printMatrix();
    }
    return 0;
}
