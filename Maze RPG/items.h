
#pragma once

#include <iostream>

class Player;

enum ItemType {
	ITEM_POTION,
	ITEM_WEAPON,
	ITEM_ARMOR
};

class Item {
public:
	int x, y;
	std::string itemName;
	ItemType type;
	int uniqueID;

	Item(std::string name);

	virtual std::string OnUse(Player* player);
};

class Potion : public Item {
public:
	int health;

	Potion(int newX, int newY, std::string name, int health);

	std::string OnUse(Player* player) override;
};

class Weapon : public Item {
public:
	int damage;

	Weapon(std::string name, int damage);

	std::string OnUse(Player* player) override;
};

class Armor : public Item {
public:
	int armor;

	Armor(std::string name, int armor);

	std::string OnUse(Player* player) override;
};
