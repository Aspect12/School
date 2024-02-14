
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
	Weapon dagger("Dagger", 4);
	activeWeaponID = dagger.uniqueID;
	inventory.push_back(dagger);
	currAttack = 4;

	RegisterMapChange('0', x, y);
};

void Player::AddItem(Item item) {
	inventory.push_back(item);

	// If the item is a weapon or armor, and it's better than the current one, equip it
	if (item.type == ITEM_WEAPON) {
		Weapon* weapon = (Weapon*)&item;

		cout << weapon->damage << endl;
		if (weapon->damage > currAttack) {
			cout << weapon->damage << endl;
			cout << weapon->uniqueID << endl;
			currAttack = weapon->damage;

			activeWeaponID = weapon->uniqueID;
		}
	} else if (item.type == ITEM_ARMOR) {
		Armor* armor = (Armor*)&item;

		if (armor->armor > currArmor) {
			currArmor = armor->armor;

			activeArmorID = armor->uniqueID;
		}
	}

	// Re-draw the side text
	DrawSideText(*this);
}

void Player::RemoveItem(string item) {
	for (int i = 0; i < inventory.size(); ++i) {
		if (inventory[i].itemName != item) continue;

		inventory.erase(inventory.begin() + i);
		break;
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

		chestItems.push_back(*weapon);
		items.erase(items.begin() + index);

		break;
	}

	while (true) {
		int index = rand() % items.size();
		armor = items[index];

		if (armor->type != ITEM_ARMOR) continue;

		chestItems.push_back(*armor);
		items.erase(items.begin() + index);

		break;
	}
};

void Chest::RemoveItem(string item) {
	for (int i = 0; i < chestItems.size(); ++i) {
		if (chestItems[i].itemName != item) continue;

		chestItems.erase(chestItems.begin() + i);
		break;
	}
}

string Chest::GetListOfItems() {
	string items;

	for (int i = 0; i < chestItems.size(); ++i) {
		items += chestItems[i].itemName + ", ";
	}

	if (items.empty()) return "Nothing!";

	return items.substr(0, items.size() - 2);
}
