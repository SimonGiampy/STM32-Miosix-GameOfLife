//
// Created by simon on 08/11/21.
//

#include "my_input.h"
#include "GameOfLife.h"

void startGOF();

int main(int argc, char *argv[]) {
	my_input input;

	//return input.handle();

	//input.basicPrint();
	//return 0;

	//return input.freeMover();

	startGOF();
	return 0;
}

void startGOF() {
	GameOfLife gof;
	gof.compute();
}

