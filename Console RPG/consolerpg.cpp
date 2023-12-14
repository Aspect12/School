
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
			SetConsoleTextAttribute(hConsole, 9);
			cout << character;
			SetConsoleTextAttribute(hConsole, 7);

			break;
		case 'K':
			SetConsoleTextAttribute(hConsole, 6);
			cout << character;
			SetConsoleTextAttribute(hConsole, 7);

			break;
		default:
			cout << character;

			break;
	}
}

// Function to clear the top lines of the console
void ClearTopLines(int linesToClear) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);

    COORD topLeft = {0, 0};
    DWORD length = consoleInfo.dwSize.X * linesToClear;
    DWORD written;

    FillConsoleOutputCharacter(hConsole, ' ', length, topLeft, &written);
    FillConsoleOutputAttribute(hConsole, consoleInfo.wAttributes, length, topLeft, &written);
    SetConsoleCursorPosition(hConsole, topLeft);
}

// Interaction handling
void Interact(char& character, int& x, int& y) {
	switch (character) {
		case '#':
			cout << "You cannot interact with the wall!                              " << endl;

			break;
		case 'K':
			cout << "You pick up the key.                              " << endl;

			break;
		default:
			cout << "There is nothing in that direction!                              " << endl;

			break;
	}
}

void DrawMap(char map[MAPHEIGHT][MAPWIDTH]) {
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

	// Create the map
	char map[MAPHEIGHT][MAPWIDTH];

	for (int i = 0; i < MAPHEIGHT; i++) {
		for (int j = 0; j < MAPWIDTH ; j++) {
			if (i == 0 || i == MAPHEIGHT - 1 || j == 0 || j == MAPWIDTH - 1) {
				map[i][j] = '#';
			} else {
				map[i][j] = ' ';
			}
		}
	}

	map[1 + rand() % (MAPHEIGHT - 2)][1 + rand() % (MAPWIDTH - 2)] = 'K'; // Place a key randomly on the map, not on the edge

	// Set the player in the middle of the map
	map[MAPHEIGHT / 2][MAPWIDTH / 2] = '0';

	// Draw the map for the first time
	DrawMap(map);

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
				Interact(map[x - 1][y], x, y);

				break;
			case KEY_DOWN: // Interact down
				Interact(map[x + 1][y], x, y);

				break;
			case KEY_LEFT: // Interact left
				Interact(map[x][y - 1], x, y);

				break;
			case KEY_RIGHT: // Interact right
				Interact(map[x][y + 1], x, y);

				break;
			default:
				break;
		}

		// Re-draw the map after each control
		ClearTopLines(MAPHEIGHT);
		DrawMap(map);

		cout << endl;
	} while (control != 'x');
}
