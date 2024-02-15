
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <fstream>
#include <vector>
#include <cmath>
#include <map>

#include "functions.h"
#include "classes.h"
#include "items.h"

using namespace std;

extern vector<Item*> items;

#define KEY_UP			72
#define KEY_DOWN		80
#define KEY_LEFT		75
#define KEY_RIGHT		77

#define KEY_ONE			49
#define KEY_TWO			50
#define KEY_THREE		51
#define KEY_FOUR		52
#define KEY_FIVE		53
#define KEY_SIX			54
#define KEY_SEVEN		55
#define KEY_EIGHT		56
#define KEY_NINE		57

#define KEY_SHIFT_ONE	33
#define KEY_SHIFT_TWO	64
#define KEY_SHIFT_THREE	35
#define KEY_SHIFT_FOUR	36
#define KEY_SHIFT_FIVE	37
#define KEY_SHIFT_SIX	94
#define KEY_SHIFT_SEVEN	38
#define KEY_SHIFT_EIGHT	42
#define KEY_SHIFT_NINE	40

// Global Variables
char map[MAPHEIGHT][MAPWIDTH];
vector<MapChange> pendingChanges;
vector<Chest*> chests;

std::map<int, int> keyShiftMap = {
	{KEY_SHIFT_ONE,		0},
	{KEY_SHIFT_TWO,		1},
	{KEY_SHIFT_THREE,	2},
	{KEY_SHIFT_FOUR,	3},
	{KEY_SHIFT_FIVE,	4},
	{KEY_SHIFT_SIX,		5},
	{KEY_SHIFT_SEVEN,	6},
	{KEY_SHIFT_EIGHT,	7},
	{KEY_SHIFT_NINE,	8}
};

int main() {
	srand(time(0)); // Set the random seed
	HideCursor(); // Hide the cursor

	string playerName;

	cout << endl << "Enter your Hero's name: ";
	getline(cin, playerName);

	if (playerName.length() < 1) {
		cout << "Your Hero's name cannot be smaller than 1 character!" << endl << endl;

		system("pause");
		return 1;
	} else if (playerName.length() > 20) {
		cout << "Your Hero's name cannot exceed 20 characters!" << endl << endl;

		system("pause");
		return 1;
	}

	Sleep(1000);
	system("cls");
	Sleep(1000);

	int count = 0;
	string mapText;

	ifstream mapFile("mazerpgmap.txt");

	// Read the map from the file
	while (getline(mapFile, mapText)) {
		for (int i = 0; i < mapText.size(); ++i) {
			RegisterMapChange(mapText[i], count, i);
		}

		count++;
	}

	mapFile.close();

	// Create the player
	Player* player = new Player(10, 5, playerName, 40, 40, 0);

	// Create all the items
	Weapon shortSword("Short Sword", 6);
	Weapon longsword("Longsword", 8);
	Weapon battleAxe("Battle Axe", 12);
	Armor leatherArmor("Leather Armor", 2);
	Armor chainMail("Chain Mail", 4);
	Armor breastPlate("Breast Plate", 6);
	Armor fullPlate("Full Plate", 8);

	// Create all the chests
	Chest chest(4, 12);
	Chest chest2(7, 50);
	Chest chest3(13, 38);

	// Create all the enemies
	Enemy goblin(11, 18, "Goblin", 20, 20, 4, 2, "Small Health Potion", 10);
	Enemy orc(6, 35, "Orc", 25, 25, 5, 3, "Health Potion", 20);
	Enemy troll(14, 51, "Troll", 30, 30, 6, 4, "Large Health Potion", 30);
	Boss elve(10, 75, "Elve", 35, 35, 6, 4, "Super Health Potion", 50);

	// Draw the map for the first time
	DrawMap();

	// Draw game information on the right side of the map
	DrawSideText(player);

	AppendActionResult(player->currName + " enters the realm...");

	// Control handling
	do {
		char control = getch();

		if (control == 0 || control == 224) { // Arrow key codes
			control = getch(); // Read the second code
		}

		int newX, newY;

		switch (control) {
			case 'w': // Move up
			case 'W':
				if (::map[player->x - 1][player->y] != ' ') break;

				if (player->inCombat != -1) {
					AppendActionResult(player->currName + " cannot move while in combat!");

					break;
				}

				RegisterMapChange(' ', player->x, player->y);
				player->x--;
				RegisterMapChange('0', player->x, player->y);

				break;

			case 's': // Move down
			case 'S':
				if (::map[player->x + 1][player->y] != ' ') break;

				if (player->inCombat != -1) {
					AppendActionResult(player->currName + " cannot move while in combat!");

					break;
				}

				RegisterMapChange(' ', player->x, player->y);
				player->x++;
				RegisterMapChange('0', player->x, player->y);

				break;

			case 'a': // Move left
			case 'A':
				if (::map[player->x][player->y - 1] != ' ') break;

				if (player->inCombat != -1) {
					AppendActionResult(player->currName + " cannot move while in combat!");

					break;
				}

				RegisterMapChange(' ', player->x, player->y);
				player->y--;
				RegisterMapChange('0', player->x, player->y);

				break;

			case 'd': // Move right
			case 'D':
				if (::map[player->x][player->y + 1] != ' ') break;

				if (player->inCombat != -1) {
					AppendActionResult(player->currName + " cannot move while in combat!");

					break;
				}

				RegisterMapChange(' ', player->x, player->y);
				player->y++;
				RegisterMapChange('0', player->x, player->y);

				break;

			case KEY_UP: // Interact up
				newX = player->x - 1;
				player->Interact(::map[player->x - 1][player->y], newX, player->y);

				break;

			case KEY_DOWN: // Interact down
				newX = player->x + 1;
				player->Interact(::map[player->x + 1][player->y], newX, player->y);

				break;

			case KEY_LEFT: // Interact left
				newY = player->y - 1;
				player->Interact(::map[player->x][player->y - 1], player->x, newY);

				break;

			case KEY_RIGHT: // Interact right
				newY = player->y + 1;
				player->Interact(::map[player->x][player->y + 1], player->x, newY);

				break;

			case KEY_ONE: // Inventory Action Slots
			case KEY_TWO:
			case KEY_THREE:
			case KEY_FOUR:
			case KEY_FIVE:
			case KEY_SIX:
			case KEY_SEVEN:
			case KEY_EIGHT:
			case KEY_NINE:
				{
					if ((control - KEY_ONE) >= player->inventory.size()) {
						AppendActionResult(player->currName + " do not have an item in slot " + to_string(control - KEY_ONE + 1) + "!");

						break;
					}

					Item* item = player->inventory[control - KEY_ONE];

					AppendActionResult(item->OnUse(player));

					if (item->oneUse) player->RemoveItem(item->uniqueID, false);

					DrawSideText(player);

					break;
				}

			case KEY_SHIFT_ONE: // Inventory Drop Item Slots
			case KEY_SHIFT_TWO:
			case KEY_SHIFT_THREE:
			case KEY_SHIFT_FOUR:
			case KEY_SHIFT_FIVE:
			case KEY_SHIFT_SIX:
			case KEY_SHIFT_SEVEN:
			case KEY_SHIFT_EIGHT:
			case KEY_SHIFT_NINE:
				{
					int slot = keyShiftMap[control];

					if (slot >= player->inventory.size()) {
						AppendActionResult(player->currName + " do not have an item in slot " + to_string(slot + 1) + "!");

						break;
					}

					Item* item = player->inventory[slot];

					// Find a free space around the player to drop the item
					int dropX = player->x;
					int dropY = player->y;

					int dx, dy;
					bool foundFreeSpot = false;

					for (int i = 0; i < 4; i++) {
						dx = (i == 0) ? -1 : (i == 1) ? 1 : 0;
						dy = (i == 2) ? -1 : (i == 3) ? 1 : 0;
						dropX = player->x + dx;
						dropY = player->y + dy;

						if (::map[dropX][dropY] != ' ') continue;

						foundFreeSpot = true;

						break;
					}

					if (!foundFreeSpot) {
						AppendActionResult(player->currName + " cannot drop the " + item->itemName + " here!");

						break;
					}

					item->x = dropX;
					item->y = dropY;

					AppendActionResult(player->currName + " dropped the " + item->itemName + ".");

					player->RemoveItem(item->uniqueID, true);

					RegisterMapChange(item->mapChar, dropX, dropY);

					// Add the item back to the item list
					items.push_back(item);

					DrawSideText(player);

					break;
				}
		}

		// Re-draw the map after each action
		DrawMap();

		cout << endl;
	} while (true);
}
