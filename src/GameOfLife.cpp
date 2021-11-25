#include "includes/GameOfLife.h"

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

/**
 * constructs the cellular automaton given the number of rows and columns
 * @param rows number of rows
 * @param cols number of columns
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
 * destructor deallocates the dynamically allocated the matrices of the game representation
 */
GameOfLife::~GameOfLife() {
	delete[] currentState[0];
	delete[] currentState;
	delete[] previousState[0];
	delete[] previousState;
}

/**
 * checks if a certain cell is alive in the previous state
 * @param x matrix coordinate
 * @param y matrix coordinate
 * @return true if it lived
 */
bool GameOfLife::wasAlive(int x, int y) {
	return this->previousState[x][y];
}

/**
 * checks if a certain cell is alive in the current state
 * @param x matrix coordinate
 * @param y matrix coordinate
 * @return true if it lives
 */
bool GameOfLife::isAlive(int x, int y) {
	return this->currentState[x][y];
}

/**
 * switches the cell status: if it was dead, now it lives, and vice versa
 * @param x coordinate of the cursor column position
 * @param y coordinate of the cursor row position
 */
void GameOfLife::changeCellState(int x, int y) {
	currentState[y/2][x/3] = !currentState[y/2][x/3];
	clearScreen();
	showState();
}

/**
 * prints the matrix state, by building up a single string
 */
void GameOfLife::showState() {
	std::string matrix;
	for (int j = -1; j < rows * 2; j++) {
		for (int i = -1; i < cols * 3; i++) {

			if (j == -1 || j % 2 == 1) {
				matrix.append("-"); // horizontal dividers
			} else {
				if (i == -1 || i % 3 == 2) matrix.append("|"); // vertical cell dividers
				else if (this->currentState[j/2][i/3]) matrix.append(living); // living cells
				else matrix.append(" "); //empty cells
			}
		}
		matrix.append("\n\r");
	}

	std::cout << matrix;
	std::cout.flush();
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
 * puts a glider on the matrix a classic glider starting from the left upper corner
 * @param x coordinate of the column of the cursor position
 * @param y coordinate of the row of the cursor position
 */
void GameOfLife::spawnGlider(int x, int y) {
	int temp = x;
	x = y/2;
	y = temp/3;

	this->currentState[x][y+1] = true;
	this->currentState[x+1][y+2] = true;
	this->currentState[x+2][y] = true;
	this->currentState[x+2][y+1] = true;
	this->currentState[x+2][y+2] = true;

	clearScreen();
	showState();
}

/**
 * spaws a mid-size spaceship in the cellular automaton at the requested coordinates
 * @param x coordinate of the column of the cursor position
 * @param y coordinate of the row of the cursor position
 */
void GameOfLife::spawnSpaceship(int x, int y) {
	int temp = x;
	x = y/2;
	y = temp/3;

	this->currentState[x][y] = true;
	this->currentState[x+2][y] = true;
	this->currentState[x+3][y+1] = true;
	this->currentState[x+3][y+2] = true;
	this->currentState[x+3][y+3] = true;
	this->currentState[x+3][y+4] = true;
	this->currentState[x+2][y+4] = true;
	this->currentState[x+1][y+4] = true;
	this->currentState[x][y+3] = true;

	clearScreen();
	showState();
}

/**
 * this function only works on linux, because this random function is not present on miosix.
 * Use this function for testing purpose only (instead of manually input every living cell)
 */
void GameOfLife::randomInitialization() {
	int x, y;
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

/**
 * shows the evolution in time of the cellular automaton. Main thread of the program
 */
void GameOfLife::compute() {

	Controller controller;
	controller.startReader();

	clearScreen();

	// initialization of the previous state matrix to be equal the previous one
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			this->previousState[i][j] = isAlive(i, j);
		}
	}

	bool still = false, exit = false; //empty life check mechanism
	for (int t = 0; !still && !exit; t++) { // iteration over time
		for (int row = 0; row < rows; row++) {
			for (int col = 0; col < cols; col++) {
				//checks for the evolution of the cellular automaton
				if (wasAlive(row, col) && !(liveNeighbours(row, col) == 2 || liveNeighbours(row, col) == 3)) {
					this->currentState[row][col] = false;
				} else if (!wasAlive(row, col) && liveNeighbours(row, col) == 3) {
					this->currentState[row][col] = true;
				}
				still |= this->currentState[row][col];
			}
		}

		still = !still;
		//std::cout << "simulation #" << t << "\n\r";
		showState();
		if (!still) { // the cellular automaton is not a still life
			for (int i = 0; i < rows; i++) { //copy to the previous state
				for (int j = 0; j < cols; j++) {
					this->previousState[i][j] = isAlive(i, j);
				}
			}
			exit = controller.inputManager();

			if (!exit) clearScreen();
		} else {
			controller.setTermination(); // empty life leads to the automatic termination of the program
		}
		// if the cellular automaton is a still life, so the simulation reaches its end
	}
}

/**
 * clears terminal window
 */
void GameOfLife::clearScreen() {
	std::cout << "\033[H\033[J";
}


