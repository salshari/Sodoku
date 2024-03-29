#include "Cell.h"

Cell::Cell()
{
}

Cell::Cell(int num) // this constructor is only called by initializing the board
{
	value = num;
	assignedByBoard = true;
}

Cell::~Cell()
{
}

void Cell::clearCell()
{
	value = -1;
	assignedByBoard = false;
	possibilities.clear();
	possibleConflicts.clear();
}

void Cell::setValue(int setVal)
{
	value = setVal;
}

int Cell::getValue()
{
	return value;
}

void Cell::addConflict(int x)
{
	bool y = false;
	for (int i = 0; i < possibleConflicts.size(); i++)
	{
		if (possibleConflicts[i] == x)
		{
			y = true;
		}
	}

	if (y == false)
	{
		if (x != -1)
		{
			possibleConflicts.push_back(x);
		}
	}
}


vector<int> Cell::getConflicts()
{
	return possibleConflicts;
}


ostream &operator<<(ostream &ostr, Cell cell)
{
	ostr << cell.value << endl;
	return ostr;
}
