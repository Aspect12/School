
#pragma once

#include <iostream>
#include <vector>

#include "items.h"

// A 'MapChange' class to store map changes pending to be redrawn
class MapChange {
public:
	int changeX;
	int changeY;

	MapChange(int x, int y);
};

// A 'Player' class to represent the player
class Player {
public:
	int x, y;
	std::string currName;
	std::vector<Item> inventory;
	int currHealth;
	int maxHealth;
	int currArmor;
	int currAttack;

	int activeWeaponID;
	int activeArmorID;

	Player(int newX, int newY, std::string name, int health, int maxHealth, int armor);

	void AddItem(Item item);

	void RemoveItem(std::string item);
};

// A 'chest' class to store items
class Chest {
public:
	int x, y;
	std::vector<Item> chestItems;

	Chest(int x, int y);

	void RemoveItem(std::string item);

	std::string GetListOfItems();
};
