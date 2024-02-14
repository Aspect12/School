
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

// Interaction handling
void Interact(Player& player, char& object, int& x, int& y) {
	string potionName;
	int healthAmount;

	switch (object) {
		case '#': // Wall
			PrintActionResult("You push the wall. Nothing happens.");

			break;
		case 'M': // Monster
			// Drop a random health potion from the items list once the monster is killed
			switch (rand() % 4) {
				case 0:
					potionName = "Small Health Potion";
					healthAmount = 10;

					break;
				case 1:
					potionName = "Medium Health Potion";
					healthAmount = 20;

					break;
				case 2:
					potionName = "Large Health Potion";
					healthAmount = 30;

					break;
				case 3:
					potionName = "Super Health Potion";
					healthAmount = 40;

					break;
			}

			PrintActionResult("You attack and kill the monster. It drops a " + potionName + ".");

			break;
		case 'C': // Chest
			for (int i = 0; i < chests.size(); ++i) {
				if (chests[i]->x != x || chests[i]->y != y) continue;

				Chest* chest = chests[i];

				PrintActionResult("You open the chest and find: " + chest->GetListOfItems());

				// Give the items to the player
				for (int j = 0; j < chest->chestItems.size(); ++j) {
					player.AddItem(chest->chestItems[j]);
				}

				// Remove the items from the chest
				chest->chestItems.clear();

				break;
			}

			break;
		case 'H': // Health
			// Find the potion in the items list based on its position and add it to the player's inventory
			for (int i = 0; i < items.size(); ++i) {
				cout << items[i]->x << " " << items[i]->y << endl;

				if (items[i]->x != x || items[i]->y != y) continue;

				Potion* potion = (Potion*)items[i];

				PrintActionResult("You pick up a " + potion->itemName + ".");
				player.AddItem(*potion);
				items.erase(items.begin() + i);

				DrawSideText(player);

				break;
			}

			break;
		default:
			PrintActionResult("There is nothing in that direction!");

			break;
	}

	// If a monster was killed, drop a health potion
	if (!potionName.empty()) {
		Potion potion(x, y, potionName, healthAmount);

		items.push_back(&potion);
	}
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
void DrawSideText(Player& player) {
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
	cout << "Player Name: " << player.currName;

	position.Y += 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	cout << "Health: " << player.currHealth << "/" << player.maxHealth;

	position.Y += 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	cout << "Armor: " << player.currArmor;

	position.Y += 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	cout << "Attack DMG: " << player.currAttack;

	position.Y += 2;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	cout << "--===== BACKPACK =====--";

	// Loop through the player's inventory and print the items
	for (int i = 0; i < player.inventory.size(); ++i) {
		position.Y += 1;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);

		Item& item = player.inventory[i];

		if (player.activeWeaponID == item.uniqueID || player.activeArmorID == item.uniqueID) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Light green

		cout << i + 1 << ". " << item.itemName;

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
}
