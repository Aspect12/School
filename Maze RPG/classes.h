
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
	std::vector<Item*> inventory;
	std::string currName;

	int x, y;
	int currHealth;
	int maxHealth;
	int currArmor;
	int currAttack;

	int maxItems;
	int activeWeaponID;
	int activeArmorID;
	int inCombat;

	Player(int newX, int newY, std::string name, int health, int maxHealth, int armor);

	void AddItem(Item* item);

	void RemoveItem(int uniqueID, bool noDestroy);

	void Interact(char& object, int& x, int& y);
};

// An 'Enemy' class to represent an enemy
class Enemy {
public:
	int x, y;
	std::string name;
	int health;
	int maxHealth;
	int attack;
	int armor;
	int uniqueID;
	std::string healthPotName;
	int healthPotValue;

	Enemy(int newX, int newY, std::string newName, int newHealth, int newMaxHealth, int newAttack, int newArmor, std::string potName, int potValue);

	void Attack(Player* player);
};

// A 'Boss' class to represent a boss, based on the 'Enemy' class
class Boss : public Enemy {
public:
	bool enraged;

	Boss(int newX, int newY, std::string newName, int newHealth, int newMaxHealth, int newAttack, int newArmor, std::string potName, int potValue);

	void BossAttack(Player* player);
};

// A 'Chest' class to store items
class Chest {
public:
	int x, y;
	std::vector<Item*> chestItems;

	Chest(int x, int y);

	void RemoveItem(std::string item);

	std::string GetListOfItems();
};
