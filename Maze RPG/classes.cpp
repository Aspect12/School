
#include <iostream>
#include <vector>

#include "functions.h"
#include "classes.h"
#include "items.h"

using namespace std;

vector<Item*> items;

// A 'MapChange' class to store the changes to the map so I don't redraw the entire thing every time
MapChange::MapChange(int x, int y)	{
	changeX = x;
	changeY = y;
};

// A 'Player' class to represent the player
Player::Player(int newX, int newY, string name, int health, int maxHP, int armor) {
	x = newX;
	y = newY;
	currName = name;
	currHealth = health;
	maxHealth = maxHP;
	currArmor = armor;

	// Give the player a starting weapon
	Weapon* dagger = new Weapon("Dagger", 4, false);
	activeWeaponID = dagger->uniqueID;
	inventory.push_back(dagger);
	currAttack = 4;

	RegisterMapChange('0', x, y);
};

void Player::AddItem(Item* item) {
	inventory.push_back(item);

	// If the item is a weapon or armor, and it's better than the current one, equip it
	if (item->type == ITEM_WEAPON) {
		Weapon* weapon = (Weapon*)item;

		if (weapon->damage > currAttack) {
			weapon->OnUse(this, true);
		}
	} else if (item->type == ITEM_ARMOR) {
		Armor* armor = (Armor*)item;

		if (armor->armor > currArmor) {
			armor->OnUse(this, true);
		}
	}

	// Re-draw the side text
	DrawSideText(this);
}

void Player::RemoveItem(int uniqueID, bool noDestroy) {
	for (int i = 0; i < inventory.size(); ++i) {
		if (inventory[i]->uniqueID != uniqueID) continue;

		inventory[i]->OnRemoved(this);

		if (!noDestroy) delete inventory[i];

		inventory.erase(inventory.begin() + i);

		break;
	}
}

void Player::Interact(char& object, int& x, int& y) {
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
					this->AddItem(chest->chestItems[j]);
				}

				// Remove the items from the chest
				chest->chestItems.clear();

				break;
			}

			break;
		case 'H': // Health
		case 'A': // Armor
		case 'W': // Weapon
			// Find the item in the items list based on its position and add it to the player's inventory
			for (int i = 0; i < items.size(); ++i) {
				if (items[i]->x != x || items[i]->y != y) continue;

				Item* item = items[i];

				PrintActionResult("You pick up a " + item->itemName + ".");
				this->AddItem(item);
				items.erase(items.begin() + i);

				RegisterMapChange(' ', x, y);

				DrawSideText(this);

				break;
			}

			break;
		default:
			PrintActionResult("There is nothing in that direction!");

			break;
	}

	if (!potionName.empty()) {
		Item* potion = new Potion(x, y, potionName, healthAmount);

		items.push_back(potion);
	}
}

// A 'chest' class to store items
Chest::Chest(int newX, int newY) {
	x = newX;
	y = newY;

	chests.push_back(this);
	RegisterMapChange('C', x, y);

	// Add some items to the chest
	Item* weapon;
	Item* armor;

	while (true) {
		int index = rand() % items.size();
		weapon = items[index];

		if (weapon->type != ITEM_WEAPON) continue;

		chestItems.push_back(weapon);
		items.erase(items.begin() + index);

		break;
	}

	while (true) {
		int index = rand() % items.size();
		armor = items[index];

		if (armor->type != ITEM_ARMOR) continue;

		chestItems.push_back(armor);
		items.erase(items.begin() + index);

		break;
	}
};

void Chest::RemoveItem(string item) {
	for (int i = 0; i < chestItems.size(); ++i) {
		if (chestItems[i]->itemName != item) continue;

		chestItems.erase(chestItems.begin() + i);
		break;
	}
}

string Chest::GetListOfItems() {
	string items;

	for (int i = 0; i < chestItems.size(); ++i) {
		items += chestItems[i]->itemName + ", ";
	}

	if (items.empty()) return "Nothing!";

	return items.substr(0, items.size() - 2);
}
