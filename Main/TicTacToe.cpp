#include "TicTacToe.h"
#include "ConsoleUtils.h"
#include <iostream>
#include <conio.h>

using namespace std;

//Constructor where initializes a clean board and sets default game states
TicTacToe::TicTacToe() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			board[i][j] = ' ';
		}
	}
	selectorRow = 1;					//Start selector in the middle
	selectorCol = 1;
	currentPlayer = 'X';
	winner = ' ';
	winType = 0;						//0 means no winning line detected yet
}

//Renders the game board
//Uses ANSI escape codes for colors and logic to highlight the winning line
void TicTacToe::printBoard() {
	system("cls");
	ConsoleUtils::gotoxy(0, 0);
	cout << "\033[36m   TIC TAC TOE ARCADE\033[0m\n\n";

	for (int r = 0; r < 3; r++) {
		cout << "   |   |   " << endl;
		for (int c = 0; c < 3; c++) {
			//Determine if the current cell is part of the winning line for color highlighting
			bool isWinCell = false;
			if (winType >= 1 && winType <= 3 && r == winType - 1) isWinCell = true; // Rows
			if (winType >= 4 && winType <= 6 && c == winType - 4) isWinCell = true; // Columns
			if (winType == 7 && r == c) isWinCell = true;

			//Display logic: Selector [.] vs Winning Cell vs Normal Cell
			if (r == selectorRow && c == selectorCol && winType == 0) {
				cout << "\033[1;33m[" << (board[r][c] == ' ' ? '.' : board[r][c]) << "]\033[0m";
			}
			else if (isWinCell) {
				cout << " \033[1;93m" << board[r][c] << "\033[0m ";		//Bright Yellow for winner
			}
			else {
				char display = (board[r][c] == ' ' ? ' ' : board[r][c]);
				cout << " " << display << " ";
			}

			if (c < 2) { 
				cout << "|"; 
			}

		}
		cout << endl;
		if (r < 2) {
			cout << "   |   |   " << endl;
			cout << "--- --- ---" << endl;
		}
		else {
			cout << "   |   |   " << endl;
		}
	}

	cout << "\n\n  Player \033[1;32m" << currentPlayer << "\033[0m's turn." << endl;
	cout << "  \033[33mUse W,A,S,D to move, ENTER to select.\033[0m" << endl;
	cout << "                                      " << endl;
}

//Handles user player movement.
//Uses _getch() for instant response to W,A,S,D or Arrow keys.
void TicTacToe::playerInput() {
	bool moveConfirmed = false;

	while (!moveConfirmed) {
		printBoard();
		int key = _getch();

		//Handle special keys/arrows
		if (key == 0 || key == 224) {
			key = _getch();
		}

		switch (key) {
		case 'w': case 'W': case 72: if (selectorRow > 0) selectorRow--; break;
		case 's': case 'S': case 80: if (selectorRow < 2) selectorRow++; break;
		case 'a': case 'A': case 75: if (selectorCol > 0)selectorCol--; break;
		case 'd': case'D': case 77: if (selectorCol < 2)selectorCol++; break;
		//Enter key
		case 13:	
			if (board[selectorRow][selectorCol] == ' ') {
				board[selectorRow][selectorCol] = currentPlayer;
				moveConfirmed = true;
			}
			else {
				//System beep for invalid move
				cout << "\a";
			}
			break;
		}
	}
}

//Change the player after each movement
void TicTacToe::switchPlayer() {
	if (currentPlayer == 'X') {
		currentPlayer = 'O';
	}
	else {
		currentPlayer = 'X';
	}
}

//Victory condition check.
//Scans board and sets winType to indicate which line to highlight in color.
bool TicTacToe::checkWin() {
	//Check Rows and Columns
	for (int i = 0; i < 3; i++) {
		if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
			winType = i + 1;		//Row 1, 2, or 3.
			return true;
		}
		if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
			winType = i + 4;		//Col 1, 2, or 3.
			return true;
		}
	}

	//Checking diagonals
	if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
		winType = 7;
		return true;
	}
	if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
		winType = 8;
		return true;
	}
	winType = 0;
	return false;
}

//Draw condition check
bool TicTacToe::checkDraw() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == ' ') {
				return false;
			}
		}
	}
	return true;
}

//AI logic
//1. Try to win. 2. Block player from winning. 3. Take the center. 4. Move randomly
void TicTacToe::computerInput() {
	//Priority 1: Win
	if (findBestMove('O')) return;
	//Priority 2: Block player
	if (findBestMove('X')) return;
	//Priority 3: Center
	if (board[1][1] == ' ') {
		board[1][1] = 'O';
		selectorRow = 1;
		selectorCol = 1;
		return;
	}
	//Priority 4: Random available spot
	srand((unsigned int)time(0));
	int attempts = 0;
	while (attempts < 100) {
		int r = rand() % 3;
		int c = rand() % 3;
		if (board[r][c] == ' ') {
			board[r][c] = 'O';
			selectorRow = r;
			selectorCol = c;
			return;
		}
		attempts++;
	}
}

//Helper for computer AI to scan for winning or blocking opportunities
bool TicTacToe::findBestMove(char symbol) {
	//This logic simulates placing a piece in every empty spot to see if it results in a win
	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 3; c++) {
			if (board[r][c] == ' ') {
				board[r][c] = symbol;		//Test move
				if (checkWin()) {
					board[r][c] = 'O';		//Real move
					selectorRow = r;
					selectorCol = c;
					return true;
				}
				board[r][c] = ' ';			//Undo test move
				winType = 0;				//Reset winType because checkWin() would have set it during testing
			}
		}
	}
	return false;
}

//Main game loop controller
//Handles the flow between turns and manage the end-game conditions
void TicTacToe::playGame() {
	ConsoleUtils::hideCursor();
	system("cls");

	//Game Mode Selection
	cout << "\t==================================" << endl;
	cout << "\t	SELECT GAME MODE      " << endl;
	cout << "\t==================================" << endl;
	cout << "\t 1. Player vs Player" << endl;
	cout << "\t 2. Player vs Computer" << endl;
	cout << "\t==================================" << endl;
	cout << "\t Selection: ";

	char mode;
	while (true) {
		mode = _getch();
		if (mode == '1') {
			vsPC = false;
			break;
		}
		if (mode == '2') {
			vsPC = true;
			break;
		}
	}

	bool gameOver = false;
	
	while (!gameOver) {
		playerInput();

		if (checkWin()) {
			printBoard();
			cout << "\n\033[1;32mPlayer " << currentPlayer << " wins!\033[0m" << endl;
			gameOver = true;
		}
		else if (checkDraw()) {
			printBoard();
			cout << "\n\033[1;33mIt's a tie! The board is full.\033[0m" << endl;
			gameOver = true;
		}
		else {
			switchPlayer();

			if (vsPC && !gameOver && currentPlayer == 'O') {
				printBoard();
				cout << "\n Computer is thinking..." << endl;
				Sleep(1000);
				computerInput();

				if (checkWin()) {
					printBoard();
					cout << "\n\033[1;31mComputer wins!\033[0m" << endl;
					gameOver = true;
				}
				else if (checkDraw()) {
					printBoard();
					gameOver = true;
				}
				else {
					switchPlayer();
				}
			}
		}
	}

	handlePostGameOption();

}

//End-of-game menu
//Allows the user to reset the class state or return to the main arcade menu
void TicTacToe::handlePostGameOption() {
	cout << "\n\n\033[1;36m(R)estart\033[0m or \033[1;31m(M)ain Menu\033[0m? ";

	while (true) {
		char choice = _getch();
		if (choice == 'r' || choice == 'R') {
			//Reset the board and state
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					board[i][j] = ' ';
				}
			}
			winType = 0;
			currentPlayer = 'X';
			selectorRow = 1;
			selectorCol = 1;

			//Re-run the game loop
			playGame();
			break;
		}
		else if (choice == 'm' || choice == 'M') {
			//Simply exit the function to return to the Arcade Main Menu
			break;
		}
	}
}

