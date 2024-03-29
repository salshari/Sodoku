#ifndef PROJ4A_CELL_H
#define PROJ4A_CELL_H
#include <vector>
#include <iostream>
#include <algorithm>

class cell {
public:
    //Blank and pre-populated cell
    cell();
    cell(int val);
    //Get and set cell functions
    int getCellValue();
    void setCellValue(int val);
    //All components for setting and getting conflicts
    vector<int> cellPossibleNumbers;
    void clearCell();
    void addConflict(int a);
    vector<int> getConflicts();
    //Overloaded operator to print the cells
    friend ostream &operator<<(ostream &ostr, const cell& c);
    //boolean for whether the number was a part of the original text file
    bool givenInFile;
private:
    //Cell value and total conflict vectors
    int cellValue;
    vector<int> cellConflicts;

};
//Base constructor with no variable assignment
cell::cell()
{
    givenInFile = false;
}

//Constructor with defined starting value
cell::cell(int val)
{
    //Assigns it to true so it won't be altered later
    cellValue = val;
    //givenInFile = true;
}

//Gets the value contained in the cell of the board
int cell::getCellValue()
{
    return cellValue;
}

//Sets the value for the specified cell of the board
void cell::setCellValue(int val)
{
    cellValue = val;
}

//Clears the value from the cell to make it blank
void cell::clearCell()
{
    //Resets cell value and clears all of its conflicts
    cellValue = -1;
    givenInFile = false;
    cellPossibleNumbers.clear();
    cellConflicts.clear();
}

//Adds a conflict to a specific cell when # doesn't work
void cell::addConflict(int a)
{
    bool containsA = false;
    for (int i = 0; i < cellConflicts.size(); i++)
    {
        if (cellConflicts[i] == a)
        {
            containsA = true;
        }
    }

    //if a is not contained in
    if (containsA == false)
    {
        if (a != -1)
        {
            cellConflicts.push_back(a);
        }
    }
}

//Returns the integer vector of conflicts for a given cell
vector<int> cell::getConflicts()
{
    return cellConflicts;
}

// print cell.  for use in board<< operator.
ostream &operator<<(ostream &ostr, const cell& c)
{
    ostr << c.cellValue;
    return ostr;
}

#endif //PROJ4A_CELL_H