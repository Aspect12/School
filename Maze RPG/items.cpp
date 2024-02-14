
#include <iostream>

#include "functions.h"
#include "classes.h"
#include "items.h"

using namespace std;

extern vector<Item*> items;

int itemIterator = 1;

Item::Item(string name) {
	x = 0;
	y = 0;
	itemName = name;
	uniqueID = itemIterator++;

	items.push_back(this);
}

string Item::OnUse(Player*) {
	return "";
}

Potion::Potion(int newX, int newY, string name, int value) : Item(name) {
	x = newX;
	y = newY;
	health = value;
	type = ITEM_POTION;

	RegisterMapChange('H', x, y);
}

string Potion::OnUse(Player* player) {
	int currHealth = player->currHealth;

	player->currHealth = min(player->maxHealth, currHealth + health);

	return "You drank the " + itemName + " and gained " + to_string(health) + ".";
}

Weapon::Weapon(string name, int value) : Item(name) {
	damage = value;
	type = ITEM_WEAPON;
}

string Weapon::OnUse(Player* player) {
	player->currAttack = damage;

	return "You equipped the " + itemName + " and gained " + to_string(damage) + " attack!";
}

Armor::Armor(string name, int value) : Item(name) {
	armor = value;
	type = ITEM_ARMOR;
}

string Armor::OnUse(Player* player) {
	player->currArmor = armor;

	return "You equipped the " + itemName + " and gained " + to_string(armor) + " armor!";
}
