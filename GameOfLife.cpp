#include "GameOfLife.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <random>

using namespace std;

/**
The universe of the Game of Life is an infinite, two-dimensional orthogonal grid of square cells,
each of which is in one of two possible states, live or dead, (or populated and unpopulated, respectively).
Every cell interacts with its eight neighbours, which are the cells that are horizontally, vertically,
or diagonally adjacent. At each step in time, the following transitions occur:

Any live cell with fewer than two live neighbours dies, as if by underpopulation.
Any live cell with two or three live neighbours lives on to the next generation.
Any live cell with more than three live neighbours dies, as if by overpopulation.
Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
These rules, which compare the behavior of the automaton to real life, can be condensed into the following:

Any live cell with two or three live neighbours survives.
Any dead cell with three live neighbours becomes a live cell.
All other live cells die in the next generation. Similarly, all other dead cells stay dead.
 */


bool GameOfLife::wasAlive(int x, int y) {
	return this->prevState[x][y];
}
bool GameOfLife::isAlive(int x, int y) {
	return this->currentState[x][y];
}

/**
 * prints the matrix state
 */
void GameOfLife::showState() {
	for (int i = 0; i < ROWS * 2 + 1; i++) {
		for (int j = 0; j < COLS * 3 + 1; j++) {

			if (j % 2 == 0) cout << "-"; // horizontal separator
			else if (i == 0 || (i+1) % 3 == 0) cout << "|"; // vertical separator
			else if (this->currentState[i][j]) cout << "X"; // living cell mark
			else cout << " "; // dead cell mark

			//cout << this->currentState[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	clearScreen();
}

/**
 * returns the number of live neighbours present around a cell at the given coordinates
 * @param x coordinate of the selected cell
 * @param y coordinate of the selected cell
 * @return number of living neighbours (max = 8)
 */
int GameOfLife::liveNeighbours(int x, int y) {

	int count = 0;
	int startX, startY, endX, endY;

	if (x == 0) {
		startX = 0;
		endX = x+1;
	} else if (x == ROWS-1) {
		startX = x-1;
		endX = ROWS - 1;
	} else {
		startX = x-1;
		endX = x+1;
	}

	if (y == 0) {
		startY = 0;
		endY = y+1;
	} else if (y == COLS-1) {
		startY = y-1;
		endY = COLS - 1;
	} else {
		startY = y-1;
		endY = y+1;
	}

	for (int coordX = startX; coordX <= endX; coordX++) {
		for (int coordY = startY; coordY <= endY; coordY++) {
			if (coordX != x || coordY != y) {
				count += (int) wasAlive(coordX, coordY);
			}
		}
	}
	return count;
}

GameOfLife::GameOfLife() {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			currentState[i][j] = false;
			prevState[i][j] = false;
		}
	}
}

/**
 * example: initialization valid for a matrix bigger than 4 columns and 4 rows
 * it shows a classic glider starting from the left upper corner
 */
void GameOfLife::customInitialization() {
	this->currentState[1][2] = true;
	this->currentState[2][3] = true;
	this->currentState[3][1] = true;
	this->currentState[3][2] = true;
	this->currentState[3][3] = true;

	this->prevState[1][2] = true;
	this->prevState[2][3] = true;
	this->prevState[3][1] = true;
	this->prevState[3][2] = true;
	this->prevState[3][3] = true;

}

void GameOfLife::randomInitialization() {
	int x = 1, y = 1;
	std::random_device dev1;
	std::mt19937 rng1(dev1());
	std::random_device dev2;
	std::mt19937 rng2(dev2());
	std::uniform_int_distribution<std::mt19937::result_type> distrow(0, ROWS-1);
	std::uniform_int_distribution<std::mt19937::result_type> distcol(0, COLS-1);

	// 20% of the cells are initialized as living cells
	for (int i = 0; i < ROWS * COLS * 0.2; i++) {
		//these 2 lines create a strange problem when running on miosix
		x = static_cast<int>(distrow(rng1));
		y = static_cast<int>(distcol(rng2));
		this->currentState[x][y] = true;
	}

	for (int i = 0; i < ROWS; i++) { //copy to the previous state
		for (int j = 0; j < COLS; j++) {
			this->prevState[i][j] = isAlive(i, j);
		}
	}

}

void GameOfLife::compute() {
	customInitialization();
	showState();


	for (int t = 0; t < 10; t++) {
		for (int row = 0; row < ROWS; row++) {
			for (int col = 0; col < COLS; col++) {
				if (wasAlive(row, col) &&
					!(liveNeighbours(row, col) == 2 || liveNeighbours(row, col) == 3)) {
					this->currentState[row][col] = false;
				} else if (!wasAlive(row, col) && liveNeighbours(row, col) == 3) {
					this->currentState[row][col] = true;
				}

			}
		}

		showState();

		for (int i = 0; i < ROWS; i++) { //copy to the previous state
			for (int j = 0; j < COLS; j++) {
				this->prevState[i][j] = isAlive(i, j);
			}
		}


	}

}

/**
 * clears terminal window
 */
void GameOfLife::clearScreen() {
	cout << "\033[H\033[J";
}


