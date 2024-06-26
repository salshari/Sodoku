// include necessary libraries
#include <iostream>
#include <fstream>
#include "d_matrix.h"
#include "d_except.h"
#include <vector>

using namespace std;

// The type of the value in a cell
typedef int ValueType; 
// Indicates that a cell is blank
const int Blank = -1; 
 // The number of cells in a small square
const int SquareSize = 3;
const int BoardSize = SquareSize * SquareSize;
const int MinValue = 1;
const int MaxValue = 9;
int numSolutions = 0;

class board 
{
    // declare the public members of the board class
    public:
        board(int);
        void clear();
        void initialize(ifstream &fin);
        void print();
        bool isBlank(int, int);
        ValueType getCell(int, int);
        void setCell(int, int, ValueType);
        // the clearCell function is a function that clears the cell
        void clearCell(int, int); 
        // the isSolved function returns a boolean which is the result of checking if the board was solved
        bool isSolved(); 
        // the printConflicts function prints out the conflicts encountered 
        void printConflicts() const;

    // declare the private members of the board class
    private:
        matrix<ValueType> value;
        matrix<bool> rows;
        matrix<bool> columns;
        matrix<bool> squares;
};

// constructor for the board class
board::board(int sqSize) : value(BoardSize + 1, BoardSize + 1), rows(BoardSize + 1, MaxValue + 1),
                            columns(BoardSize + 1, MaxValue + 1), squares(BoardSize + 1, MaxValue + 1) {
    clear();
}

// calculates the square number of the sodoku board by dividing the row and column indices
int squareNumber(int i, int j) 
{
    return SquareSize * ((i - 1) / SquareSize) + ((j - 1) / SquareSize) + 1;
}

void board::clear() 
{   // for every cell in the board, do the folloiwing
    for (int i = 1; i <= BoardSize; i++) 
    {
        for (int j = 1; j <= BoardSize; j++) 
        {
            // set the value of the square at every row and column to blank
            value[i][j] = Blank;
        }
    }
}


void board::initialize(ifstream &fin)
{
    // create a char called ch
    char ch;
    // call the clear function to clear the board before intitializing
    clear(); 

    // for each cell in the board, do the following
    for (int i = 1; i <= BoardSize; i++)
    {
        for (int j = 1; j <= BoardSize; j++)
        {
            // read a character from the input file
            fin >> ch;

            // if the read char is not Blank, do the following
            if (ch != '.')
            {
                try
                {
                    // convert the charcater to an integer and set the cell value
                    setCell(i, j, ch - '0');
                }
                // if there is an error reading the char or the char is out of range, do the follwing:
                catch (rangeError &ex)
                {
                    // print error message
                    cout << "Error: " << ex.what() << endl;
                    // create a placeholder string
                    string placeholder;
                    getline(fin, placeholder);
                    // exit the function and go to the next board
                    return; 
                }
            }
        }
    }
}

void board::print() 
{
    // for each cell in the board, do the following
    for (int i = 1; i <= BoardSize; i++) 
    {
        if ((i - 1) % SquareSize == 0) 
        {
            // print a dash for each cell
            cout << " -";
            for (int j = 1; j <= BoardSize; j++)
                cout << "---";
            cout << "-";
            cout << endl;
        }
        for (int j = 1; j <= BoardSize; j++) 
        {
            // create vertical division line between blocks
            if ((j - 1) % SquareSize == 0)
                cout << "|";
            // if not blank, print the cell value
            if (!isBlank(i, j))
                cout << " " << getCell(i, j) << " ";
            // if blank, print a dash
            else
                cout << " ";
        }
        // clsoe out the block using a vertical line
        cout << "|";
        cout << endl;
    }
    cout << " -";
    // create the boarder for the bottom of the board
    for (int j = 1; j <= BoardSize; j++)
        cout << "---";
    cout << "-";
    cout << endl;
}

ValueType board::getCell(int i, int j) 
{
    // if the index is within a valid range of the board, do the folliwing
    if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
        // return the value of that cell at the (i,j)
        return value[i][j];
    // if the index is not within a valid range of the board, do the folliwing
    else
        // throw a rangeError
        throw rangeError("bad value in getCell");
}

void board::setCell(int i, int j, ValueType newValue)
{
    // if the index is within a valid range of the board, do the folliwing
    if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
    {
        // set the value of the cell (i,j) to newValue
        value[i][j] = newValue;
        rows[i][newValue] = true;
        columns[j][newValue] = true;
        squares[squareNumber(i, j)][newValue] = true;
    }
    // if the index is not within a valid range of the board, do the folliwing
    else
    {
        // throw a rangeError
        throw rangeError("bad value in setCell");
    }
}

void board::clearCell(int i, int j) 
{
    // if the index is within a valid range of the board, do the folliwing
    if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
        // throw a rangeError
        throw rangeError("bad value in clearCell");
    // if the cell is not already blank
    if (!isBlank(i, j)) 
    {
        // get the value of the cell at (i,j)
        ValueType val = getCell(i, j);
        // set the cell (i,j) to blank
        value[i][j] = Blank;
        // the corresponding row is marked as not having the previous value 
        rows[i][val] = false;
        // the corresponding column is marked as not having the previous value 
        columns[j][val] = false;
        // the corresponding square is marked as not having the previous value 
        squares[squareNumber(i, j)][val] = false;
    }
}

bool board::isBlank(int i, int j) 
{
    // if the index is within a valid range of the board, do the folliwing
    if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
        // throw a rangeError
        throw rangeError("bad value in isBlank");
    // check if the value of the cell at (i,j) is blank and return 
    return (value[i][j] == Blank);
}

bool board::isSolved() 
{
    // for each cell of the board, do the following
    for (int i = 1; i <= BoardSize; ++i) 
    {
        // if a cell is blank, the board is not solved
        for (int j = 1; j <= BoardSize; ++j) {
            if (isBlank(i, j)) 
            {
                // the board is not solved, return false
                return false;
            }
        }
    }
    // if every cell is filled (not blank), the board is solved, return true
    return true;
}


void board::printConflicts() const {
    cout << "Row Conflicts:" << endl;
    for (int i = 1; i <= BoardSize; ++i) {
        for (int val = 1; val <= MaxValue; ++val) {
            int count = 0;
            for (int j = 1; j <= BoardSize; ++j) {
                if (value[i][j] == val) {
                    ++count;
                }
            }
            cout << count << " ";
        }
        cout << endl;
    }

    cout << "Column Conflicts:" << endl;
    for (int j = 1; j <= BoardSize; ++j) {
        for (int val = 1; val <= MaxValue; ++val) {
            int count = 0;
            for (int i = 1; i <= BoardSize; ++i) {
                if (value[i][j] == val) {
                    ++count;
                }
            }
            cout << count << " ";
        }
        cout << endl;
    }

    cout << "Square Conflicts:" << endl;
    for (int i = 1; i <= BoardSize; i += SquareSize) {
        for (int j = 1; j <= BoardSize; j += SquareSize) {
            for (int val = 1; val <= MaxValue; ++val) {
                int count = 0;
                for (int r = i; r < i + SquareSize; ++r) {
                    for (int c = j; c < j + SquareSize; ++c) {
                        if (value[r][c] == val) {
                            ++count;
                        }
                    }
                }
                cout << count << " ";
            }
            cout << endl;
        }
    }
}


int main() {
    ifstream fin;
    string fileName = "sudoku.txt";
    fin.open(fileName.c_str());
    if (!fin) {
        cerr << "Cannot open " << fileName << endl;
        exit(1);
    }
    try {
        board b1(SquareSize);
        while (fin && fin.peek() != 'Z') {
            b1.initialize(fin);
            b1.print();
            cout << "Is Solved: " << (b1.isSolved() ? "Yes" : "No") << endl;
            b1.printConflicts();
        }
    } catch (indexRangeError &ex) {
        cout << ex.what() << endl;
        exit(1);
    }
    return 0;
}