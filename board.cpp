#include <iostream>
#include <fstream>
#include "d_matrix.h"
#include "d_except.h"
#include <vector>

using namespace std;

typedef int ValueType; // The type of the value in a cell
const int Blank = -1; // Indicates that a cell is blank
const int SquareSize = 3; // The number of cells in a small square
const int BoardSize = SquareSize * SquareSize;
const int MinValue = 1;
const int MaxValue = 9;
int numSolutions = 0;

class board {
public:
    board(int);
    void clear();
    void initialize(ifstream &fin);
    void print();
    bool isBlank(int, int);
    ValueType getCell(int, int);
    void setCell(int, int, ValueType);
    void clearCell(int, int); // Function to clear a cell
    bool isSolved(); // Function to check if the board has been solved
    void printConflicts() const;
    bool solve(int, int); // Recursive solver function
    int getRecursiveCalls();

private:
    matrix<ValueType> value;
    matrix<bool> rows;
    matrix<bool> columns;
    matrix<bool> squares;
    int recursiveCalls; // Counter for recursive calls
};

board::board(int sqSize) : value(BoardSize + 1, BoardSize + 1), rows(BoardSize + 1, MaxValue + 1),
                            columns(BoardSize + 1, MaxValue + 1), squares(BoardSize + 1, MaxValue + 1) {
    clear();
    recursiveCalls = 0; // Initialize recursive calls counter
}

int squareNumber(int i, int j) {
    return SquareSize * ((i - 1) / SquareSize) + ((j - 1) / SquareSize) + 1;
}

int board::getRecursiveCalls(){
    return recursiveCalls;
}


void board::clear() {
    for (int i = 1; i <= BoardSize; i++) {
        for (int j = 1; j <= BoardSize; j++) {
            value[i][j] = Blank;
        }
    }
    // Reset rows, columns, and squares matrices
    rows = matrix<bool>(BoardSize + 1, MaxValue + 1);
    columns = matrix<bool>(BoardSize + 1, MaxValue + 1);
    squares = matrix<bool>(BoardSize + 1, MaxValue + 1);
}

void board::initialize(ifstream &fin)
{
    char ch;
    clear(); // Clear the board before initializing

    for (int i = 1; i <= BoardSize; i++)
    {
        for (int j = 1; j <= BoardSize; j++)
        {
            fin >> ch;
            // If the read char is not Blank
            if (ch != '.')
            {
                try
                {
                    setCell(i, j, ch - '0'); // Convert char to int and set cell value
                }
                catch (rangeError &ex)
                {
                    cout << "Error: " << ex.what() << endl;
                    // Skip to the end of the line in the input file
                    string dummy;
                    getline(fin, dummy);
                    return; // Exit the function to move to the next Sudoku board
                }
            }
        }
    }
}


void board::print() {
    for (int i = 1; i <= BoardSize; i++) {
        if ((i - 1) % SquareSize == 0) {
            cout << " -";
            for (int j = 1; j <= BoardSize; j++)
                cout << "---";
            cout << "-";
            cout << endl;
        }
        for (int j = 1; j <= BoardSize; j++) {
            if ((j - 1) % SquareSize == 0)
                cout << "|";
            if (!isBlank(i, j))
                cout << " " << getCell(i, j) << " ";
            else
                cout << " ";
        }
        cout << "|";
        cout << endl;
    }
    cout << " -";
    for (int j = 1; j <= BoardSize; j++)
        cout << "---";
    cout << "-";
    cout << endl;
}

ValueType board::getCell(int i, int j) {
    if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
        return value[i][j];
    else
        throw rangeError("bad value in getCell");
}

void board::setCell(int i, int j, ValueType newValue)
{
    if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
    {
        value[i][j] = newValue;
        // Mark the row, column, and square as occupied by this value
        rows[i][newValue] = true;
        columns[j][newValue] = true;
        squares[squareNumber(i, j)][newValue] = true;
    }
    else
    {
        throw rangeError("bad value in setCell");
    }
}

void board::clearCell(int i, int j) {
    if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
        throw rangeError("bad value in clearCell");
    if (!isBlank(i, j)) {
        ValueType val = getCell(i, j);
        value[i][j] = Blank;
        rows[i][val] = false;
        columns[j][val] = false;
        squares[squareNumber(i, j)][val] = false;
    }
}

bool board::isBlank(int i, int j) {
    if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
        throw rangeError("bad value in isBlank");
    return (value[i][j] == Blank);
}

bool board::isSolved() {
    for (int i = 1; i <= BoardSize; ++i) {
        for (int j = 1; j <= BoardSize; ++j) {
            if (isBlank(i, j)) {
                return false;
            }
        }
    }
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

bool board::solve(int i, int j) {
    ++recursiveCalls;

    if (i == BoardSize + 1) {
        i = 1;
        if (++j == BoardSize + 1)
            return true; // Entire board has been successfully filled without conflict
    }

    if (!isBlank(i, j))
        return solve(i + 1, j);

    for (ValueType val = MinValue; val <= MaxValue; ++val) {
        if (!rows[i][val] && !columns[j][val] && !squares[squareNumber(i, j)][val]) {
            setCell(i, j, val);
            if (solve(i + 1, j))
                return true;
            clearCell(i, j); // Undo assignment if no solution found
        }
    }

    return false; // No value in [MinValue, MaxValue] worked, backtrack
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
        int totalRecursiveCalls = 0;
        int boardsSolved = 0;
        while (fin && fin.peek() != 'Z') {
            b1.initialize(fin);
            b1.print();
            cout << "Is Solved: " << (b1.isSolved() ? "Yes" : "No") << endl;
            b1.solve(1, 1);
            if (b1.isSolved()) {
                cout << "Board solved!" << endl;
                b1.print();
                cout << "Recursive calls needed: " << b1.getRecursiveCalls() << endl;
                totalRecursiveCalls += b1.getRecursiveCalls();
                ++boardsSolved;
            } else {
                cout << "Unable to solve board!" << endl;
            }
        }
        cout << "Total boards solved: " << boardsSolved << endl;
        cout << "Total recursive calls: " << totalRecursiveCalls*(-1) << endl;
        if (boardsSolved > 0)
            cout << "Average recursive calls per board: " << (static_cast<double>(totalRecursiveCalls) / boardsSolved)*(-1)<< endl;
    } catch (indexRangeError &ex) {
        cout << ex.what() << endl;
        exit(1);
    }
    return 0;
}
