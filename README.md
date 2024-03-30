# Sodoku
A program that solves Sudoku puzzles. 
The input to Sudoku is a 9x9 board that is subdivided into 3x3 squares. 
Each cell is either blank or contains an integer from 1 to 9.
A solution to a puzzle is the same board with every blank cell filled in with a digit from 1 to 9 such
that every digit appears exactly once in every row, column, and square.
The input to the program is a text file containing a collection of Sudoku boards, with one board
per line. 
For example:
.....2.......7...17..3...9.8..7......2.89.6...13..6....9..5.824.....891..........
3...8.......7....51..............36...2..4....7...........6.13..452...........8..Z
For each board that is read, the output is a printout of the board correctly filled in