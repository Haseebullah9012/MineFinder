#include <iostream>
#include<random>
using namespace std;

class Box
{
    public:
        int mineCounts; //Mines in Adjacent Boxes
        bool mine; //If there is a Mine or Not
        bool known; //If the Box is Builtin or Not
        bool marked; //If the Box is Marked

        Box()
        {
            mineCounts = 0;
            mine = false;
            known = false;
            marked = false;
        }
};

void Board();
void Input();
void InitializeBoard();
void CountAdjacentMines(int,int);

int rows = 16, cols = 16;
int mines; //No. of Mines
int knownBoxes;
Box board[30][30];
Box solution[30][30];

random_device Seed; //RandomValue Generator
uniform_int_distribution<int> randomNumber(1,rows);

int main()
{
	cout << endl;
	cout << "Welcome to the Mine Finder Board-Game. A Logical-Puzzle Game. \n\n";
	
    InitializeBoard();
    cout << "Out of " << rows*cols << " Boxes, \n";
    cout << "\t Number of Known Boxes: " << knownBoxes << endl;
    cout << "\t Number of Mines: " << mines << endl;
    Board();

	//Main Game
    while(true)
    {
        cout << "Select the Box (Row Col) on the Board: ";
        Input();
        Board();
    }
	
    getchar();
    return 0;
}

void Board()
{
	cout << endl;
    cout << "\t ";
    for(int i=0; i<cols*2+3; i++)
        cout << "-";
    cout << "\n";
	
    for(int r=0; r<rows; r++)
    {
		cout << "\t | ";
        for(int c=0; c<cols; c++)
        {
            if(board[r][c].known)
                cout << "\033[1;36m" << board[r][c].mineCounts << "\033[0m ";
            else if(board[r][c].mine)
                cout << "\033[1;31m" << "*" << "\033[0m ";
            else if(!board[r][c].marked)
                cout << "-" << " ";
            else
                cout << "\033[1;32m" << "#" << "\033[0m "; //Marked as Safe Box (No Mine)    
        }
		cout << "| \n";
	}

    cout << "\t ";
    for(int i=0; i<cols*2+3; i++)
        cout << "-";
    cout << "\n";
	cout << endl << endl;
}

void Input()
{
    int a,b;
    cin >> a >> b;

    if(a<1 || a>rows || b<1 || b>cols) {
        cout << "Oops! Its Outside the Board. \n"
            << "There are only " << rows << " Rows and " << cols << " Columns. \n\n";
        return;
    }
    
    int r = --a;
    int c = --b;
    if(board[r][c].known) {
        cout << "Oops! Its a Builtin Box, showing the Total Number of Mines in its Adjacent Boxes. \n\n";
        return;    
    }
    
    char mine;
    cout << "Mark if there's a Mine or Not (Y/N): ";
    cin >> mine;
    mine = toupper(mine);
	cin.ignore(255, '\n');

    if(mine=='Y') {
        board[r][c].marked = true;
        board[r][c].mine = true;
    }
    else if(mine=='N') {
        board[r][c].marked = true;
        board[r][c].mine = false;
    }
    else {
        board[r][c].marked = false; //Unmark the Box
        board[r][c].mine = false;
    }
}


void InitializeBoard()
{
    //Placement of Mines in the Board
    for(int r=0; r<rows; r++)
        for(int c=0; c<cols; c++)
            if(random()%7==0)
                board[r][c].mine = true; //Place the Mine with 1/7 Probability (14% Chance)
    

    //Known Boxes
    for(int r=0; r<rows; r++) {
        for(int c=0; c<cols; c++) {
            if(board[r][c].mine)
                mines++;
            else
                board[r][c].known = true;
        }
    }
    
    for(int r=0; r<rows; r++)
        for(int c=0; c<cols; c++)
            if(board[r][c].known)
                CountAdjacentMines(r,c);
    
    //Store the Mines Location
    for(int r=0; r<rows; r++)
        for(int c=0; c<cols; c++)
            solution[r][c] = board[r][c];
    
    
    //Hide Known Boxes
    for(int r=0; r<rows; r++)
        for(int c=0; c<cols; c++)
            if(board[r][c].known && board[r][c].mineCounts==0)
                board[r][c].known = false; //Set Boxes Unknown with 0 Adjacent Mines
    
    cout << "\t The Board with Actual Mines Location (For Test Purpose).";
    Board(); //Just for test Purpose
    
    for(int i=0, r,c; i<rows*5; i++)
    {
        r = randomNumber(Seed) -1;
        c = randomNumber(Seed) -1;
        if(board[r][c].known)
            board[r][c].known = false; //Set some Boxes Unknown Randomly
    }

    //Hide Mines
    for(int r=0; r<rows; r++)
        for(int c=0; c<cols; c++)
            board[r][c].mine = false;
    

    //Count the Number of Known Boxes
    for(int r=0; r<rows; r++)
        for(int c=0; c<cols; c++)
            if(board[r][c].known)
                knownBoxes++;
}

void CountAdjacentMines(int r, int c)
{
    //Main Diagonal
    if(board[r-1][c-1].mine)
        board[r][c].mineCounts++;
    if(board[r+1][c+1].mine)
        board[r][c].mineCounts++;
    
    //Left Diagonal
    if(board[r-1][c+1].mine)
        board[r][c].mineCounts++;
    if(board[r+1][c-1].mine)
        board[r][c].mineCounts++;

    //Horizontal Side
    if(board[r][c+1].mine)
        board[r][c].mineCounts++;
    if(board[r][c-1].mine)
        board[r][c].mineCounts++;

    //Vertical Side
    if(board[r+1][c].mine)
        board[r][c].mineCounts++;
    if(board[r-1][c].mine)
        board[r][c].mineCounts++;
}
