/*******************************************************************
                          CIS-121 Spring 2026
Type of Assignment: Final Project (Personal Work)
Programmer: Liliana Pantoja
Section: 400/M40
Date Due: 5/10/2026

Purpose: Demonstrate proficiency in C++ class structures, 
encapsulation, and console-based game engine mechanics. This project
serves as a centralized hub for interactive games, showcasing logic 
for AI-driven opponents, collision detection, and real-time screen 
buffering for flicker-free gameplay.
********************************************************************/

#include <iostream>
#include <conio.h>
#include "TicTacToe.h"
#include "SnakeGame.h"
#include "ConsoleUtils.h"
#include "CarGame.h"

using namespace std;

// Function Prototypes: Defines the game-launching wrappers and menu structure
void GameMenu();
void Snake();
void TicTacToeGame();
void Car();

 //Main execution function.
 //Launches the GameMenu to start the application lifecycle.
int main()
{
    GameMenu();
    
    return 0;
}

//Wrapper to initialize and run the Snake Game.
//After the game ends, the console is cleared to return to the main menu cleanly.
void Snake() {
    SnakeGame game;
    game.run();
    system("cls");          // Cleanup screen before returning to menu
}

//Wrapper to initialize and run the Tic Tac Toe Game.
void TicTacToeGame() {
    TicTacToe game;
    game.playGame();
    system("cls");

}

//Wrapper to initialize and run the Car Racing Game.
//Manages the transition from the menu to the racing game environment.
void Car() {
    CarGame car;
    car.play();
    system("cls");
}

/*The primary User Interface for the application.
 * Uses a 'while' loop to keep the program running until the user selects 'Exit'.
 * Includes input validation to prevent crashes if a user enters non-integer data.
 */
void GameMenu() {
    bool exitApp = false;
    int choice;

    while (!exitApp) {
        ConsoleUtils::showCursor();             // Ensure cursor is visible for menu selection
        system("cls");

        cout << "\t======================================" << endl;
        cout << "\t      [ARCADE MULTIGAME SYSTEM]       " << endl;
        cout << "\t======================================" << endl;
        cout << "\t  1. Play Snake Game" << endl;
        cout << "\t  2. Play Tic Tac Toe" << endl;
        cout << "\t  3. Play Car Racing" << endl;
        cout << "\t  4. Exit" << endl;
        cout << "\t======================================" << endl;
        cout << "\t Select an option: ";

        // INPUT VALIDATION: Prevents infinite loops if letters are entered
        if (!(cin >> choice)) {
            cin.clear();                        // Clear error flags
            cin.ignore(1000, '\n');             // Discard invalid input buffer
            continue;
        }

        // Routing logic based on user input
        switch (choice) {
        case 1:
            Snake();
            break;
        case 2:
            TicTacToeGame();
            break;
        case 3:
            Car();
            break;
        case 4:
            exitApp = true;             // Breaks the loop to close the application
            break;
        default:
            cout << "\nInvalid Choice!";
            Sleep(1000);                // Give user a second to read the error
            break;
        }
    }
}

