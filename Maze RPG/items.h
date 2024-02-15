
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
	bool oneUse;
	char mapChar;

	Item(std::string name, bool addToItemPool = true);

	virtual std::string OnUse(Player* player, bool noNotify = false);

	virtual void OnRemoved(Player* player);
};

class Potion : public Item {
public:
	int health;

	Potion(int newX, int newY, std::string name, int health, bool addToItemPool = true);

	std::string OnUse(Player* player, bool noNotify = false) override;
};

class Weapon : public Item {
public:
	int damage;

	Weapon(std::string name, int damage, bool addToItemPool = true);

	std::string OnUse(Player* player, bool noNotify = false) override;

	void OnRemoved(Player* player) override;
};

class Armor : public Item {
public:
	int armor;

	Armor(std::string name, int armor, bool addToItemPool = true);

	std::string OnUse(Player* player, bool noNotify = false) override;

	void OnRemoved(Player* player) override;
};
