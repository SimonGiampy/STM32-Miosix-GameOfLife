#include <iostream>
#include "GameOfLife.h"

using namespace std;

void moveCursor(std::ostream& os, int col, int row);

int main() {
	//GameOfLife::compute();


	//before executing this code clear the terminal
	//these are absolute coordinates, starting from the lft-upper corner

	cout << "0, 0, 0, 0, 0, 0, 0, 0, 0, 0\n"
			"0, 0, 1, 0, 0, 0, 0, 0, 0, 0\n"
			"0, 0, 0, 1, 0, 0, 0, 0, 0, 0\n"
			"0, 1, 1, 1, 0, 0, 0, 0, 0, 0\n"
			"0, 0, 0, 0, 0, 0, 0, 0, 0, 0\n"
			"0, 0, 0, 0, 0, 0, 0, 0, 0, 0\n"
			"0, 0, 0, 0, 0, 0, 0, 0, 0, 0\n"
			"0, 0, 0, 0, 0, 0, 0, 0, 0, 0\n"
			"0, 0, 0, 0, 0, 0, 0, 0, 0, 0\n"
			"0, 0, 0, 0, 0, 0, 0, 0, 0, 0";
	moveCursor(std::cout, 20,20);
	std::cout << "writing in posizione 20,20";
	moveCursor(std::cout, 50,60);
	std::cout << "writing in posizione 50,60" << std::endl;

	return 0;
}


void moveCursor(std::ostream& os, int col, int row) {
	os << "\033[" << col << ";" << row << "H";
}



/*
#define ROWS 10
#define COLS 10

void printMatrix(bool matrix[ROWS][COLS]);
void changeMatrix(bool matrix[ROWS][COLS]);

int main() {

	bool state[ROWS][COLS];
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			state[i][j] = false;
		}
	}

	printMatrix(state);
	changeMatrix(state);
	printMatrix(state);


	// dynamic allocation of a matrix in the heap
	bool (*matrix2)[10] = new bool [10][10];

	//array of arrays approach  -- slower in time and heavier in space
	bool **matrix3 = new bool*[10];
	for (int i = 0; i < 10; ++i) {
		matrix3[i] = new bool[10];
	}


    return 0;
}

void printMatrix(bool matrix[ROWS][COLS]) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

void changeMatrix(bool matrix[ROWS][COLS]) {
	matrix[0][2] = true;
}
*/

