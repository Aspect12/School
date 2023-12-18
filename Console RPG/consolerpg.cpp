
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

// Global Enums
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define MAPWIDTH 50
#define MAPHEIGHT 20

// Global Variables
char map[MAPHEIGHT][MAPWIDTH];

// Function to hide the cursor
void HideCursor() {
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// Function to print colored characters based on their value
void PrintColored(char& character) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	switch (character) {
		case '0':
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
			
			break;
		case 'K':
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

			break;
		case 'M':
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED);

			break;
		case 'G':
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);

			break;
	}

	cout << character;
	SetConsoleTextAttribute(hConsole, 7);
}

// Function to clear the top lines of the console
void ClearLines(int startLine, int endLine) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);

	COORD topLeft = {0, startLine};
	DWORD length = consoleInfo.dwSize.X * (endLine - startLine + 1);
	DWORD written;

	FillConsoleOutputCharacter(hConsole, ' ', length, topLeft, &written);
	FillConsoleOutputAttribute(hConsole, consoleInfo.wAttributes, length, topLeft, &written);
	SetConsoleCursorPosition(hConsole, topLeft);
}

// Function to clear the action line and print the new action
void PrintActionResult(string action) {
	ClearLines(MAPHEIGHT + 1, MAPHEIGHT + 1);
	cout << action << endl;
}

// Interaction handling
void Interact(char& item, int& x, int& y) {
	switch (item) {
		case '#':
			PrintActionResult("You cannot interact with the wall!");

			break;
		case 'K':
			PrintActionResult("You pick up the key.");
			map[x][y] = ' ';

			break;
		case 'M':
			PrintActionResult("You attack and kill the monster. It drops a gold coin.");
			map[x][y] = 'G';

			break;
		case 'G':
			PrintActionResult("You pick up the gold coin.");
			map[x][y] = ' ';

			break;
		default:
			PrintActionResult("There is nothing in that direction!");

			break;
	}
}

void DrawMap() {
	for (int i = 0; i < MAPHEIGHT; ++i) {
		for (int j = 0; j < MAPWIDTH; ++j) {
			PrintColored(map[i][j]);
		}

		if (i == 2) {
			cout << "  Movement: WASD";
		} else if (i == 4) {
			cout << "  Interaction: Arrow Keys";
		} else if (i == 6) {
			cout << "  Exit: X";
		}
		
		cout << endl;
	}
}

int main() {
	srand(time(0)); // Set the random seed
	HideCursor(); // Hide the cursor

	for (int i = 0; i < MAPHEIGHT; i++) {
		for (int j = 0; j < MAPWIDTH ; j++) {
			if (i == 0 || i == MAPHEIGHT - 1 || j == 0 || j == MAPWIDTH - 1) {
				map[i][j] = '#';
			} else {
				map[i][j] = ' ';
			}
		}
	}

	map[1 + rand() % (MAPHEIGHT - 2)][1 + rand() % (MAPWIDTH - 2)] = 'K'; // Place a key randomly on the map
	map[1 + rand() % (MAPHEIGHT - 2)][1 + rand() % (MAPWIDTH - 2)] = 'M'; // Place 2 monsters randomly on the map
	map[1 + rand() % (MAPHEIGHT - 2)][1 + rand() % (MAPWIDTH - 2)] = 'M';

	// Set the player in the middle of the map
	map[MAPHEIGHT / 2][MAPWIDTH / 2] = '0';

	// Draw the map for the first time
	DrawMap();

	char control;
	int x, y;
	x = MAPHEIGHT / 2;
	y = MAPWIDTH / 2;

	// Control handling
	do {
		control = getch();

		if (control == 0 || control == 224) { // Arrow key codes
			control = getch(); // Read the second code
		}
		
		int newX, newY;

		switch (control) {
			case 'w': // Move up
				if (map[x - 1][y] != ' ') break;
			
				map[x][y] = ' ';
				x--;
				map[x][y] = '0';

				break;
			case 's': // Move down
				if (map[x + 1][y] != ' ') break;
			
				map[x][y] = ' ';
				x++;
				map[x][y] = '0';

				break;
			case 'a': // Move left
				if (map[x][y - 1] != ' ') break;
			
				map[x][y] = ' ';
				y--;
				map[x][y] = '0';
			
				break;
			case 'd': // Move right
				if (map[x][y + 1] != ' ') break;

				map[x][y] = ' ';
				y++;
				map[x][y] = '0';

				break;
			case KEY_UP: // Interact up
				newX = x - 1;
				Interact(map[newX][y], newX, y);

				break;
			case KEY_DOWN: // Interact down
				newX = x + 1;
				Interact(map[newX][y], newX, y);

				break;
			case KEY_LEFT: // Interact left
				newY = y - 1;
				Interact(map[x][newY], x, newY);

				break;
			case KEY_RIGHT: // Interact right
				newY = y + 1;
				Interact(map[x][newY], x, newY);

				break;
			default:
				break;
		}

		// Re-draw the map after each control
		ClearLines(0, MAPHEIGHT);
		DrawMap();

		cout << endl;
	} while (control != 'x');
}
