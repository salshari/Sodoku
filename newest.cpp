#include <iostream>
#include <climits>      // formerly:  #include <limits.h>
#include "d_matrix.h"
#include "d_except.h"
#include <list>
#include <fstream>
#include "Cell.h"
#include <vector>

using namespace std;

typedef int ValueType; // The type of the value in a cell
const int Blank = -1;  // Indicates that a cell is blank
 
const int SquareSize = 3;  //  The number of cells in a small square
                           //  (usually 3).  The board has
                           //  SquareSize^2 rows and SquareSize^2
                           //  columns.

const int BoardSize = SquareSize * SquareSize;

const int MinValue = 1;
const int MaxValue = 9;

int numSolutions = 0;

class board
// Stores the entire Sudoku board
{
public:
    // Sample functions
    board(int sqSize);
    void clear();
    void initialize(ifstream &fin);
    void print();  //->  replaced by operator<<
    bool isBlank(int i, int j);
    cell getCell(int i, int j);

    // New functions for 4a
    void setCell(int a, int b, int val);
    //void setCell(int a, int b, const cell& newCell);
    void emptyCell(int a, int b);
    bool isSolved();
    void setConflicts(int row, int column, cell &cell);
    void updateConflicts();                                         // update conflicts after modification
    void printConflicts();                                          // print conflicts
    friend ostream& operator<< (ostream& ostr, board& b);     // print board

private:
      // The following matrices go from 1 to BoardSize in each
      // dimension, i.e., they are each (BoardSize+1) * (BoardSize+1)
      matrix<cell> value;
};

// FUNCTIONS WE ADDED
ostream& operator<< (ostream& ostr, board& b)
{
    // i = row.  j = column
    ostr << "\nCurrent Sudoku Board:\n";
    ostr << "╔═════════╤═════════╤═════════╗";
    for (int i=0; i<MaxValue; i++)
    {
        // separate squares with box-lines
        if ((((i+1) % 3) == 1) && (i+1 != 1))
        {
            ostr << "║\n";
            ostr << "╟─────────┼─────────┼─────────╢";
            ostr << "\n║";
        }
        else if (i+1 != 1)
            ostr << "║\n║";
        else if (i == 0)
            ostr << "\n║";

        // print main number sequence within each box level boxes
        for (int j=0; j<MaxValue; j++)
        {
            if (b.isBlank(i,j))
                ostr << " · ";
            else
                ostr << ' ' << b.value[i][j] << ' ';

            // separate squares with vert line between boxes
            if ((((j+1) % 3) == 0) && (j+1 != 9))  { ostr <<"│"; }
            //if (j+1 == 9)                          { ostr <<"│"; }
        }
    }
    ostr << "║\n";
    ostr << "╚═════════╧═════════╧═════════╝";
    ostr << '\n';

    return ostr;
}

/** ************* PRE-PROVIDED FUNCTIONS ************** **/
board::board(int sqSize)
    : value(BoardSize+1,BoardSize+1)
// Board constructor
{
    clear();
}

void board::clear()
// Mark all possible values as legal for each board entry
{
    for (int i = 0; i < BoardSize; i++)
    {
         for (int j = 0; j < BoardSize; j++)
            {
                value[i][j] = Blank;
            }
    }
}

void board::initialize(ifstream &fin)
// Read a Sudoku board from the input file.
{
    char ch;

    clear();

    for (int i = 0; i < BoardSize; i++)
        for (int j = 0; j < BoardSize; j++)
        {
            fin >> ch;

            // If the read char is not Blank
            // all non pre-solved locs start as Null types not cells
            if (ch != '.')
            {
                setCell(i,j,ch-'0');   // Convert char to int
                value[i][j].givenInFile = true;
            }
        }
}

int squareNumber(int i, int j)
// Return the square number of cell i,j (counting from left to right,
// top to bottom.  Note that i and j each go from 1 to BoardSize
{
    // Note that (int) i/SquareSize and (int) j/SquareSize are the x-y
    // coordinates of the square that i,j is in.

    return SquareSize * ((i-1)/SquareSize) + (j-1)/SquareSize + 1;
}

ostream &operator<<(ostream &ostr, vector<int> &v)
// Overloaded output operator for vector class.
{
    for (int i = 0; i < v.size(); i++)
        ostr << v[i] << " ";
    ostr << endl;
}

cell board::getCell(int i, int j)
// Returns the value stored in a cell.  Throws an exception
// if bad values are passed.
{
    //if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
    if ((i >= 0 && i < BoardSize) && (j >= 0 && j < BoardSize))
        return value[i][j];
    else
        throw rangeError("bad value in getCell");
}

bool board::isBlank(int i, int j)
// Returns true if cell i,j is blank, and false otherwise.
{
    if (i < 0 || i >= BoardSize || j < 0 || j >= BoardSize)
        throw rangeError("bad value in setCell");

    //char tempCellVal;
    //tempCallVal << getCell(i,j);
    // return (tempCallVal == Blank);
    //return (getCell(i,j).givenInFile);
    return !value[i][j].givenInFile;
}

//NOT BEING USED FOR THE SAKE OF operator<<
void board::print()
// Prints the current board.
{
    for (int i = 0; i < BoardSize; i++)
    {
        if ((i) % SquareSize == 0)
        {
        cout << " -";
        for (int j = 0; j < BoardSize; j++)
            cout << "---";
        cout << "-";
        cout << endl;
        }
        for (int j = 0; j < BoardSize; j++)
        {
            if ((j) % SquareSize == 0)
                cout << "|";
            if (!isBlank(i,j))
                cout << " " << getCell(i,j) << " ";
            else
                cout << "   ";
        }
        cout << "|";
        cout << endl;
    }
    cout << " -";
    for (int j = 0; j < BoardSize; j++)
        cout << "---";
    cout << "-";
    cout << endl;
}

//Sets an object of the cell class equal to a given cell in the matrix
void board::setCell(int a, int b, int val)
{
    // alex got dis one homies
    //yea boi
    // yurr yur
    //rah

    value[a][b].setCellValue(val);
    //cell tempcell(val);
    //value[a][b] = tempcell;
}


//Clear a cell in the matrix at a specific point
void board::emptyCell(int a, int b)
{
    value[a][b].clearCell();
    updateConflicts();
}

//Determines whether the sudoku has been completely filled or not
bool board::isSolved()
{
    bool solved = true;
    updateConflicts();
    for (int i=0; i<BoardSize; i++)
    {
        for (int j=0; j<BoardSize; j++)
        {
            //if any cell is blank the board is not solved
            if (isBlank(i, j) == true)
            {
                solved = false;
            }
        }
    }
    return solved;
}

//Sets the conflicts for a specific cell in the sudoku
void board::setConflicts(int row, int column, cell &cell)
{
    cell.getConflicts().clear(); //Remove previous conflicts
    //Checks for all conflicts within the row
    for (int i = 0; i < BoardSize; i++)
    {
        //don't mess with the given numbers
        if (cell.givenInFile == true)
        {
            break;
        }
        if (i != column)
        {
            cell.addConflict(value[row][i].getCellValue());
        }
    }

    //Checks for all conflicts within the column
    for (int i = 0; i < BoardSize; i++)
    {
        //don't mess with the given numbers
        if (cell.givenInFile == true)
        {
            break;
        }
        if (i != row)
        {
            cell.addConflict(value[i][column].getCellValue());
        }
    }

    int startCol = column - ((column-1) % SquareSize);
    int startRow = row - ((row - 1) % SquareSize);

    //Changes location based on which square it's doing
    for (int i = startCol; i < startCol + SquareSize; i++)
    {
        for (int j = startRow; j < startRow + SquareSize; j++)
        {
            if (cell.givenInFile == true)
            {
                break;
            }
            if (i != column || j != row)
            {
                cell.addConflict(value[j][i].getCellValue());
            }
        }
    }

}

//Updates the board's cells with their conflicts when needed
//Saves space in other parts of the program to avoid repeating
void board::updateConflicts()
{
    //Goes through every cell and sets the conflicts
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            setConflicts(i, j, value[i][j]);
        }
    }
}

//Prints the conflicts for specific cells on the board
void board::printConflicts()
{
    //For every row
    for (int i = 0; i < BoardSize; i++)
    {
        //For every column
        for (int j = 0; j < BoardSize; j++)
        {
            cout << "Cell [" << i << "][" << j << "] conflicts: ";
            //vector<int> conflicts = value[i][j].getConflicts();
            //cout << conflicts << endl;
            //Prints out every cell
            for (int k = 0; k < value[i][j].getConflicts().size(); k++)
            {
                cout << value[i][j].getConflicts()[k] << " ";
            }
            cout << endl;

        }
    }
}

int main()
{
ifstream fin;
// Read the sample grid from the file.
string fileName = "sudoku.txt";
fin.open(fileName.c_str());
if (!fin)
{
cerr << "Cannot open " << fileName << endl;
exit(1);
}
try
{
board b1(SquareSize);


while (fin && fin.peek() != 'Z')
{
b1.initialize(fin);
b1.print();
b1.printConflicts();
}
}
catch (indexRangeError &ex)
{
cout << ex.what() << endl;
exit(1);
}
}
