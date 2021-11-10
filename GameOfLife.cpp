#include "GameOfLife.h"

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

GameOfLife::GameOfLife(int rows, int cols) {
	this->rows = rows;
	this->cols = cols;

	currentState = new bool*[rows]; // array of pointers to the rows of the equivalent matrix
	currentState[0] = new bool[rows * cols]; //pointer to an array containing all the matrix elements

	previousState = new bool*[rows]; // array of pointers to the rows of the equivalent matrix
	previousState[0] = new bool[rows * cols]; //pointer to an array containing all the matrix elements

	for (int i = 1; i < rows; i++) {
		currentState[i] = currentState[0] + i * cols;
		previousState[i] = previousState[0] + i * cols;
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			*(currentState[i] + j) = false;
			*(previousState[i] + j) = false;
		}
	}
}

/**
 * destructor deallocates the dynamically allocated matrix of the game representation
 */
GameOfLife::~GameOfLife() {
	delete[] currentState[0];
	delete[] currentState;
	delete[] previousState[0];
	delete[] previousState;
}


bool GameOfLife::wasAlive(int x, int y) {
	return this->previousState[x][y];
}
bool GameOfLife::isAlive(int x, int y) {
	return this->currentState[x][y];
}

void GameOfLife::changeCellState(int x, int y) {
	currentState[y/2][x/3] = !currentState[y/2][x/3];
	clearScreen();
	showState();
}

/**
 * prints the matrix state
 */
void GameOfLife::showState() {

	for (int j = -1; j < rows * 2; j++) {
		for (int i = -1; i < cols * 3; i++) {

			if (j == -1 || j % 2 == 1) {
				cout << "-";
			} else {
				if (i == -1 || i % 3 == 2) cout << "|";
				else if (this->currentState[j/2][i/3]) cout << "X";
				else cout << " ";
			}
		}
		cout << endl;
	}

	cout << endl << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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
	} else if (x == rows-1) {
		startX = x-1;
		endX = rows - 1;
	} else {
		startX = x-1;
		endX = x+1;
	}

	if (y == 0) {
		startY = 0;
		endY = y+1;
	} else if (y == cols-1) {
		startY = y-1;
		endY = cols - 1;
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

/**
 * example: initialization valid for a matrix bigger than 4 columns and 4 rows
 * it shows a classic glider starting from the left upper corner
 */
void GameOfLife::spawnGlider(int x, int y) {
	this->currentState[x][y+1] = true;
	this->currentState[x+1][y+2] = true;
	this->currentState[x+2][y] = true;
	this->currentState[x+2][y+1] = true;
	this->currentState[x+2][y+2] = true;

	this->previousState[x][y+1] = true;
	this->previousState[x+1][y+2] = true;
	this->previousState[x+2][y] = true;
	this->previousState[x+2][y+1] = true;
	this->previousState[x+2][y+2] = true;

}

void GameOfLife::randomInitialization() {
	int x = 1, y = 1;
	std::random_device dev1;
	std::mt19937 rng1(dev1());
	std::random_device dev2;
	std::mt19937 rng2(dev2());
	std::uniform_int_distribution<std::mt19937::result_type> distrow(0, rows-1);
	std::uniform_int_distribution<std::mt19937::result_type> distcol(0, cols-1);

	// 20% of the cells are initialized as living cells
	for (int i = 0; i < rows * cols * 0.2; i++) {
		//these 2 lines create a strange problem when running on miosix
		x = static_cast<int>(distrow(rng1));
		y = static_cast<int>(distcol(rng2));
		this->currentState[x][y] = true;
	}

	for (int i = 0; i < rows; i++) { //copy to the previous state
		for (int j = 0; j < cols; j++) {
			this->previousState[i][j] = isAlive(i, j);
		}
	}

}

void GameOfLife::compute() {

	//spawnGlider(1, 1);
	//clearScreen();

	showState();
	clearScreen();
	for (int t = 0; t < 10; t++) {
		for (int row = 0; row < rows; row++) {
			for (int col = 0; col < cols; col++) {
				if (wasAlive(row, col) && !(liveNeighbours(row, col) == 2 || liveNeighbours(row, col) == 3)) {
					this->currentState[row][col] = false;
				} else if (!wasAlive(row, col) && liveNeighbours(row, col) == 3) {
					this->currentState[row][col] = true;
				}

			}
		}

		showState();
		if (t < 9) {
			clearScreen();
		}

		for (int i = 0; i < rows; i++) { //copy to the previous state
			for (int j = 0; j < cols; j++) {
				this->previousState[i][j] = isAlive(i, j);
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


