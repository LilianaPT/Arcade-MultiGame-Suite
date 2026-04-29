// ======= TIC TAC TOE ============

#ifndef TICTACTOE_H
#define TICTACTOE_H

/**
 * Class TicTacToe
 * Manages the logic, rendering, and AI for a 3x3 Tic Tac Toe game.
 */
class TicTacToe
{
public: 
	// Constructor: Initializes the board, players, and selector position
	TicTacToe();

	// Main entry point that handles the game mode selection and primary game loop
	void playGame();

private:
	// --- Member Variables ---
	char board[3][3];						// The 3x3 game grid ('X', 'O', or ' ')
	char currentPlayer;						// Tracks whose turn it is ('X' or 'O')
	char winner;							// Stores the symbol of the winner after a match
	int selectorRow,						// Current row position of the user's selection cursor
		selectorCol;						// Current column position of the user's selection cursor
	bool vsPC;								// Flag to toggle between PvP and PvE (Computer) modes

	/*Tracks the winning pattern to facilitate color highlighting:
     * 0: None, 1-3: Rows, 4-6: Columns, 7: Diagonal (\), 8: Diagonal (/)
	 */
	int winType;

	// --- Private Methods (Internal Logic) ---


	void playerInput();					// Handles WASD movement and ENTER selection for the human player
	void printBoard();					// Renders the board to the console with ANSI colors and win-line highlighting
	void switchPlayer();				// Toggles currentPlayer between 'X' and 'O'
	bool checkWin();					// Checks if the board is full with no winner (a tie)
	bool checkDraw();					// Checks if the board is full with no winner (a tie)
	void computerInput();				// Logic for the Computer's turn; priorities blocking and winning moves
	bool findBestMove(char symbol);		// Helper for AI: Checks if a player/computer is one move away from winning
	void handlePostGameOption();		// Displays the Restart/Main Menu prompt after a game concludes
};
#endif
