
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <fstream>
#include <vector>
#include <cmath>

#include "functions.h"
#include "classes.h"
#include "items.h"

using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

// Global Variables
char map[MAPHEIGHT][MAPWIDTH];
vector<MapChange> pendingChanges;
vector<Chest*> chests;

int main() {
	srand(time(0)); // Set the random seed
	HideCursor(); // Hide the cursor

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
	Player player(10, 5, "Hero", 40, 40, 0);

	// Create all the items
	Weapon dagger("Dagger", 4);
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

	// Draw the map for the first time
	DrawMap();

	// Draw game information on the right side of the map
	DrawSideText(player);

	char control;

	// Control handling
	do {
		control = getch();

		if (control == 0 || control == 224) { // Arrow key codes
			control = getch(); // Read the second code
		}

		int newX, newY;

		switch (control) {
			case 'w': // Move up
			case 'W':
				if (map[player.x - 1][player.y] != ' ') break;

				RegisterMapChange(' ', player.x, player.y);
				player.x--;
				RegisterMapChange('0', player.x, player.y);

				break;
			case 's': // Move down
			case 'S':
				if (map[player.x + 1][player.y] != ' ') break;

				RegisterMapChange(' ', player.x, player.y);
				player.x++;
				RegisterMapChange('0', player.x, player.y);

				break;
			case 'a': // Move left
			case 'A':
				if (map[player.x][player.y - 1] != ' ') break;

				RegisterMapChange(' ', player.x, player.y);
				player.y--;
				RegisterMapChange('0', player.x, player.y);

				break;
			case 'd': // Move right
			case 'D':
				if (map[player.x][player.y + 1] != ' ') break;

				RegisterMapChange(' ', player.x, player.y);
				player.y++;
				RegisterMapChange('0', player.x, player.y);

				break;
			case KEY_UP: // Interact up
				newX = player.x - 1;
				Interact(player, map[player.x - 1][player.y], newX, player.y);

				break;
			case KEY_DOWN: // Interact down
				newX = player.x + 1;
				Interact(player, map[player.x + 1][player.y], newX, player.y);

				break;
			case KEY_LEFT: // Interact left
				newY = player.y - 1;
				Interact(player, map[player.x][player.y - 1], player.x, newY);

				break;
			case KEY_RIGHT: // Interact right
				newY = player.y + 1;
				Interact(player, map[player.x][player.y + 1], player.x, newY);

				break;
			default:
				break;
		}

		// Re-draw the map after each action
		DrawMap();

		cout << endl;
	} while (true);
}
