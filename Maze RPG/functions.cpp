
#include <iostream>
#include <windows.h>

#include "functions.h"
#include "classes.h"
#include "items.h"

using namespace std;

extern vector<Item*> items;

// Function to register a map change
void RegisterMapChange(char c, int x, int y) {
	map[x][y] = c;

	MapChange change(x, y);
	pendingChanges.push_back(change);
}

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
		case '0': // Player
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE); // Dark blue

			break;
		case 'M': // Monster
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED); // Red

			break;
		case 'C': // Chest
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN); // Yellow

			break;
		case 'H': // Health
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); // Light red

			break;
		case 'W': // Weapon
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Light orange

			break;
		case 'A': // Armor
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Light blue

			break;
	}

	cout << character;
	SetConsoleTextAttribute(hConsole, 7);
}


// Function to clear specific lines from the console
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
	cout << "  " << action << endl;
}

// Function to draw the pending changes to the map
void DrawMap() {
	for (int i = 0; i < pendingChanges.size(); ++i) {
		MapChange change = pendingChanges[i];
		COORD position = {change.changeY, change.changeX};
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);

		PrintColored(map[change.changeX][change.changeY]);
	}

	pendingChanges.clear();
}

// Function to draw the game instructions
void DrawSideText(Player* player) {
	COORD position = {MAPWIDTH + 2, 1};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	cout << "--===== CONTROLS =====--";

	position.Y += 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	cout << "Movement: WASD";

	position.Y += 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	cout << "Interaction: Arrow Keys";

	position.Y += 2;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	cout << "--==== STATISTICS ====--";

	position.Y += 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	cout << "Player Name: " << player->currName;

	position.Y += 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	cout << "                         "; // Cancer
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	cout << "Health: " << player->currHealth << "/" << player->maxHealth;

	position.Y += 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	cout << "                         ";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	cout << "Armor: " << player->currArmor;

	position.Y += 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	cout << "                         ";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	cout << "Attack DMG: " << player->currAttack;

	position.Y += 2;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	cout << "--===== BACKPACK =====--";

	// Loop through the player's inventory and print the items

	// Loop from 1 to 10
	for (int i = 0; i <= 8; ++i) {
		position.Y += 1;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
		cout << "                         ";
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);

		if (i < player->inventory.size()) {
			Item* item = player->inventory[i];

			if (player->activeWeaponID == item->uniqueID || player->activeArmorID == item->uniqueID) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Light green

			cout << i + 1 << ". " << item->itemName;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		} else {
			cout << i + 1 << ". ";
		}
	}
}
