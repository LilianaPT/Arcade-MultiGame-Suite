//====== SNAKE GAME ======

#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include <vector>
#include <string>

//enum eDirection
//Represents the current movement state of the snake
enum eDirection{ STOP = 0, LEFT, RIGHT, UP, DOWN };

//struct Point
//Simple coordinate structure for spatial tracking
struct Point {
	int x, y;
};

/*Class Snake
Manages the logic, rendering, difficulty scaling, and fruit spawning.
This class encapsulates the classic Snake arcade mechanics, including
tail growth, collision detection, and dynamic speed adjustment.*/
class SnakeGame
{
public:
	SnakeGame();							//Constructor: Initializes game variables and spawns the first fruit
	void run();								//Main entry point: Controls the outer game loop restart logic

private:
	// --- Member Variable (Game State) ---
	bool gameOver;							//Flag to break the inner movement loop
	const int width = 60;					//Playable area horizontal boundary
	const int height = 20;					//Playable area vertical boundary
	int x, y,								//Coordinates for the snake's head
		fruitX, fruitY,						//Current location of the collectible fruit
		score;								//Current player points (20 pts per fruit)
	std::vector<int> tailX, tailY;			//Vectors to track the dynamic position of each tail segment		
	int nTail;								//Total number of tail segment
	eDirection dir;							//Current direction of travel
	int speed;								//Sleep delay in ms; decreases as difficulty increases

	// --- Private Methods (Game Components) ---
	void draw();							//Clears the screen and renders the game world using a string buffer to prevent flickering
	void input();							//Polls keyboard input for movement (W,A,S,D) and pausing (X)
	void logic();							//Handles physics, tail following, collision, and scoring logic
	void spawnFruit();						//Generates a random coordinate for the next fruit
	void pauseMenu();						//Halts game execution and provides a sub-menu for the user
	void handlePostGameOptions();			//Displays the Game Over screen and handles the Play Again/Exit choice
	void resetGame();						//Resets all member variables to their initial state for a new round
};

#endif
