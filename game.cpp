#include <iostream>
#include<random>
using namespace std;

class Box
{
    public:
        int mineCounts; //Mines in Adjacent Boxes
        bool mine; //If there is a Mine or Not
        bool known; //If the Box is Builtin or Not
        bool marked; //If the Box is Marked with Input

        void ResetBox()
        {
            mineCounts = 0;
            mine = false;
            known = false;
            marked = false;
        }
};

void configuration();
void InitializeBoard();
void Board();
void Input();
void PlayAgain();

void CountAdjacentMines(int,int);
void DetonateMines();
void showMines();

void BoardSize();
void MineProbability();

int rows = 8, cols = 8;
int mineProbability = 20; 
int mines;
int knownBoxes;
Box board[30][30];
Box solution[30][30];

bool detonate; //To Quit the Board
char playAgain = 'Y';
random_device Seed; //RandomValue Generator
uniform_int_distribution<int> randomNumber(1,1000000000);

int main()
{
	cout << endl;
	cout << "Welcome to the Mine Finder Board-Game. A Logical-Puzzle Game. \n\n";
	
    do {
        configuration();
        InitializeBoard();
        
        cout << "So, Lets Begin! \n"
            << "Out of " << rows*cols << " Boxes, \n"
            << "\t Number of Sensor-Known Boxes: " << knownBoxes << endl
            << "\t Number of Total Mines: " << mines << endl
            << endl;
        Board();
        
        //Main Game
        while(true)
        {
            cout << "Select the Box (Row Col) on the Board: ";
            Input();
            
            if(detonate)
                break;
        }
        cout << endl;
        PlayAgain();
    }
    while(playAgain == 'Y');
    
    cout << endl << endl;
    cout << "Thanks for Playing. Hope You Enjoyed the Game! \n";
    cout << "It would be very Kind of you, if you give us an Honest Feedback. \n\n";

    cout << "You can Find me in GitHub https://github.com/Haseebullah9012. \n";

    getchar();
    return 0;
}

void configuration()
{
    char configure = 'N'; //Default Option

	do {
		if(configure == 'Y') {
			BoardSize();
			MineProbability();
            cout << endl << endl;
		}
		
        cout << "The Board-Size is (" << rows << "x" << cols << "). \n"
            << "The Probability of Mine is " << mineProbability << "%. \n"
            << endl;
		
		cout << "  Do you want to Configure Anything (Y/N): ";
		cin >> configure;
		configure = toupper(configure);
		cin.ignore(255, '\n');
		cout << endl << endl;
	}
	while(configure == 'Y');
}

void InitializeBoard()
{
    //Reset Board
    for(int r=0; r<rows; r++)
        for(int c=0; c<cols; c++)
            board[r][c].ResetBox();
    knownBoxes = 0;
    mines = 0;
    detonate = false;
    
    
    //Placement of Mines in the Board
    int num = 100/mineProbability;
    for(int r=0; r<rows; r++)
        for(int c=0; c<cols; c++)
            if(randomNumber(Seed)%num==0) {
                board[r][c].mine = true;
                mines++;
            }
    
    //Known Boxes
    for(int r=0; r<rows; r++)
        for(int c=0; c<cols; c++)
            if(!board[r][c].mine) {
                CountAdjacentMines(r,c);
                if(board[r][c].mineCounts!=0) {
                    board[r][c].known = true;
                    knownBoxes++;
                }
            }
    
    //Store the Mines Location
    for(int r=0; r<rows; r++)
        for(int c=0; c<cols; c++)
            solution[r][c] = board[r][c];
    
    //Hide Mines
    for(int r=0; r<rows; r++)
        for(int c=0; c<cols; c++)
            board[r][c].mine = false;
    
    //Hide Some Boxes for Puzzle Complexity
    num = 100/mineProbability;
    for(int r=0; r<rows; r++)
        for(int c=0; c<cols; c++)
            if(board[r][c].known) {
                if(randomNumber(Seed)%num==0) {
                    board[r][c].known = false;
                    knownBoxes--;
                }
            }
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

    if(cin.fail()) {
		cin.clear(); cin.ignore(255, '\n');
		cout << "Oops! Its not Legal. \n"
            << "Select from " << rows << " Rows and " << cols << " Columns. \n\n";
		return;
	}
	
    if(a==-8 && b==-8) {
        detonate = true;
        DetonateMines();
        showMines();
        return;
    }

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

    Board();
}

void PlayAgain()
{
	cout << "Do You Want to Play Again (Y/N): ";
	cin >> playAgain;
	playAgain = toupper(playAgain);
	cin.ignore(255, '\n');

    if(playAgain != 'Y' && playAgain != 'N') {
		cout << "   Oops! Its not a legal Response. \n\n";
		cout << "Again, ";
		PlayAgain();
        
        cout << endl << endl;
	}
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

void DetonateMines()
{
    for(int r=0; r<rows; r++)
        for(int c=0; c<cols; c++)
            if(board[r][c].mine != solution[r][c].mine) {
                cout << "Ah! You Lose. You did not Found the Mines Correctly. \n";
                return;
            }
    
    cout << "Congratulations! You Win. You Marked the Mines Correctly. \n";
}

void showMines()
{
    Box temp[rows][cols];
    for(int r=0; r<rows; r++)
        for(int c=0; c<cols; c++)
            temp[r][c] = board[r][c];

    for(int r=0; r<rows; r++)
        for(int c=0; c<cols; c++)
            board[r][c] = solution[r][c];
    Board();

    for(int r=0; r<rows; r++)
        for(int c=0; c<cols; c++)
            board[r][c] = temp[r][c];
}


void BoardSize()
{
	char board_size;
	int size_sm = 6, size_md = 10, size_lg = 16; //Predefined Sizes
	
	cout << "Choose the Board-Size (s:small, m:medium, l:large, c:custom): ";
	cin >> board_size;
	board_size = toupper(board_size);
	cin.ignore(255, '\n');

	switch(board_size) 
	{
		case 'S':
			rows = size_sm, cols = size_sm; 
			break;
		case 'M':
			rows = size_md, cols = size_md; 
			break;
		case 'L':
			rows = size_lg, cols = size_lg;
			break;
		
		case 'C':
			cout << "   Enter the Custom Total Rows and Columns of the Board: ";
			cin >> rows >> cols;
			
            if(cin.fail()) {
				cin.clear(); cin.ignore(255,'\n');
				cout << "   Oops! Its not the legal Row-Column Numbers. \n\n";
				cout << "Again, ";
				BoardSize();
			}
			else if(rows<4 || cols<4) {
				cout << "   Oops! It's too Small to be a Board. \n\n";
				cout << "Again, ";
				BoardSize();
			}
			else if (rows>30 || cols>30) {
				cout << "   Oops! It's too Large to be a Board. \n\n";
				cout << "Again, ";
				BoardSize();
			}
            else
                cout << endl;
			break;
		
		default:
			cout << "The Board-Size is Set to Default. \n";
			rows = 8, cols = 8;
	}
}

void MineProbability()
{
    cout << "Enter the Probability for Mine Placement (in Percentage): ";
    cin >> mineProbability;
    
    if(cin.fail()) {
        cin.clear(); cin.ignore(255, '\n');
        cout << "   Oops! Its not a Legal Number. \n\n";
        cout << "Again, ";
        MineProbability();
    }
    else if(mineProbability>30) {
        cout << "   Oops! You are Placing too many Mines almost all Over the Place. \n\n";
        cout << "Again, ";
        MineProbability();
    }
    else if(mineProbability<5) {
        cout << "   Oops! You are almost not Placing any Mine. \n\n";
        cout << "Again, ";
        MineProbability();
    }
    else
        cout << endl;
}
