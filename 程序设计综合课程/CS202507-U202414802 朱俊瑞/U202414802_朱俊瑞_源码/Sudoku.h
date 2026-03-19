#pragma once
#ifndef SUDOKU
#define SUDOKU

struct place
{
	int x,y;
};
void shufflenumber(int* num);
bool issatified(int chessboard[11][11], int x, int y, int num);
bool generatebase(int chessboard[11][11]);
void shuffleplace(place* places);
int findsolutions(int chessboard[11][11]);
void generatesudoku(int chessboard[11][11], int answer[11][11], int leftnumber);
void sudoku();


#endif