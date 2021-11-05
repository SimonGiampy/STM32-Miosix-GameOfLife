#include "GameOfLife.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <random>

using namespace std;

/**
 * The universe of the Game of Life is an infinite, two-dimensional orthogonal grid of square cells,
 * each of which is in one of two possible states, live or dead, (or populated and unpopulated, respectively).
 * Every cell interacts with its eight neighbours, which are the cells that are horizontally, vertically,
 * or diagonally adjacent. At each step in time, the following transitions occur:

Any live cell with fewer than two live neighbours dies, as if by underpopulation.
Any live cell with two or three live neighbours lives on to the next generation.
Any live cell with more than three live neighbours dies, as if by overpopulation.
Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
These rules, which compare the behavior of the automaton to real life, can be condensed into the following:

Any live cell with two or three live neighbours survives.
Any dead cell with three live neighbours becomes a live cell.
All other live cells die in the next generation. Similarly, all other dead cells stay dead.
 */


void GameOfLife::updateState(int x, int y, bool b) {
	this->currentState[x][y] = b;
}

void GameOfLife::updatePrevState(int x, int y, bool b) {
	this->prevState[x][y] = b;
}

bool GameOfLife::wasAlive(int x, int y) {
	return this->prevState[x][y];
}
bool GameOfLife::isAlive(int x, int y) {
	return this->currentState[x][y];
}

void GameOfLife::showState() {
	for (int i = 0; i < ROWS; i++) { //testing
		for (int j = 0; j < COLS; j++) {
			cout << this->currentState[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	cout << "\033[H\033[J"; //clear terminal
}


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

void GameOfLife::customInitialization() {
	updateState(1, 2, true);
	updateState(2, 3, true);
	updateState(3, 1, true);
	updateState(3, 2, true);
	updateState(3, 3, true);

	updatePrevState(1, 2, true);
	updatePrevState(2, 3, true);
	updatePrevState(3, 1, true);
	updatePrevState(3, 2, true);
	updatePrevState(3, 3, true);

}

void GameOfLife::randomInitialization() {
	int x = 1, y = 1;
	std::random_device dev1;
	std::mt19937 rng1(dev1());
	std::random_device dev2;
	std::mt19937 rng2(dev2());
	std::uniform_int_distribution<std::mt19937::result_type> distrow(0, ROWS-1);
	std::uniform_int_distribution<std::mt19937::result_type> distcol(0, COLS-1);
	for (int i = 0; i < 20; i++) {
		//x = static_cast<int>(distrow(rng1));
		//y = static_cast<int>(distcol(rng2));
		updateState(x, y, true);
	}
	for (int i = 0; i < ROWS; i++) { //copy to the previous state
		for (int j = 0; j < COLS; j++) {
			updatePrevState(i, j, isAlive(i, j));

		}
	}

}

void GameOfLife::compute() {
	GameOfLife game;

	game.customInitialization();
	game.showState();


	for (int t = 0; t < 10; t++) {
		for (int row = 0; row < ROWS; row++) {
			for (int col = 0; col < COLS; col++) {
				if (game.wasAlive(row, col) &&
					!(game.liveNeighbours(row, col) == 2 || game.liveNeighbours(row, col) == 3)) {
					game.updateState(row, col, false);

				} else if (!game.wasAlive(row, col) && game.liveNeighbours(row, col) == 3) {
					game.updateState(row, col, true);
				}

			}
		}

		game.showState();

		for (int i = 0; i < ROWS; i++) { //copy to the previous state
			for (int j = 0; j < COLS; j++) {
				game.updatePrevState(i, j, game.isAlive(i, j));
			}
		}


	}
	cout << "\033[<3>A";
	string input;
	cin >> input;

}

void GameOfLife::clearScreen() {
	//cout << "\033[H\033[J"; //clear terminal
}


