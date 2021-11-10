//
// Created by simon on 08/11/21.
//

#include "MyInput.h"
#include "GameOfLife.h"

void startGOF();

int main(int argc, char *argv[]) { // input arguments needed only when handling mouse clicks as inputs
	MyInput input;
	input.initialConfiguration();
	return 0;
}

void startGOF() {
	GameOfLife gof(10, 10);
	gof.compute();
}

