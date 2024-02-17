
#include <iostream>

#include "functions.h"
#include "classes.h"
#include "items.h"

using namespace std;

extern vector<Item*> items;

int itemIDIterator = 1;

Item::Item(string name, bool addToItemPool) {
	x = 0;
	y = 0;
	itemName = name;
	uniqueID = itemIDIterator++;
	oneUse = false;

	if (!addToItemPool) return;

	items.push_back(this);
}

string Item::OnUse(Player* player, bool noNotify) {
	return "";
}

void Item::OnRemoved(Player* player) {};

Potion::Potion(int newX, int newY, string name, int value, bool addToItemPool) : Item(name, addToItemPool) {
	x = newX;
	y = newY;
	health = value;
	type = ITEM_POTION;
	oneUse = true;
	mapChar = 'H';

	RegisterMapChange('H', x, y);
}

string Potion::OnUse(Player* player, bool noNotify) {
	int currHealth = player->currHealth;

	player->currHealth = min(player->maxHealth, currHealth + health);

	if (noNotify) return "";

	return player->currName + " drank the " + itemName + " and gained " + to_string(health) + " HP!";
}

Weapon::Weapon(string name, int value, bool addToItemPool) : Item(name, addToItemPool) {
	damage = value;
	type = ITEM_WEAPON;
	mapChar = 'W';
}

string Weapon::OnUse(Player* player, bool noNotify) {
	if (player->activeWeaponID == uniqueID) {
		if (noNotify) return "";

		return player->currName + " already have the " + itemName + " equipped!";
	}

	player->currAttack = damage;
	player->activeWeaponID = uniqueID;

	if (noNotify) return "";

	return player->currName + " equipped the " + itemName + ". " + player->currName + "'s new attack damage is " + to_string(damage) + "!";
}

void Weapon::OnRemoved(Player* player) {
	// When the weapon is removed, find the next best weapon and equip it
	int bestDamage = 0;
	int bestIndex = -1;

	for (int i = 0; i < player->inventory.size(); ++i) {
		if (player->inventory[i]->type != ITEM_WEAPON) continue;
		if (player->inventory[i]->uniqueID == player->activeWeaponID) continue;

		Weapon* weapon = (Weapon*)player->inventory[i];

		if (weapon->damage <= bestDamage) continue;

		bestDamage = weapon->damage;
		bestIndex = i;
	}

	if (bestIndex == -1) {
		player->currAttack = 0;
		player->activeWeaponID = -1;
	} else {
		Weapon* bestWeapon = (Weapon*)player->inventory[bestIndex];
		bestWeapon->OnUse(player);
	}
}

Armor::Armor(string name, int value, bool addToItemPool) : Item(name, addToItemPool) {
	armor = value;
	type = ITEM_ARMOR;
	mapChar = 'A';
}

string Armor::OnUse(Player* player, bool noNotify) {
	if (player->activeArmorID == uniqueID) {
		if (noNotify) return "";

		return player->currName + " already have the " + itemName + " equipped!";
	}

	player->currArmor = armor;
	player->activeArmorID = uniqueID;

	if (noNotify) return "";

	return player->currName + " equipped the " + itemName + ". " + player->currName + "'s new armor is " + to_string(armor) + "!";
}

void Armor::OnRemoved(Player* player) {
	// When the armor is removed, find the next best armor and equip it
	int bestArmor = 0;
	int bestIndex = -1;

	for (int i = 0; i < player->inventory.size(); ++i) {
		if (player->inventory[i]->type != ITEM_ARMOR) continue;
		if (player->inventory[i]->uniqueID == player->activeArmorID) continue;

		Armor* armor = (Armor*)player->inventory[i];

		if (armor->armor <= bestArmor) continue;

		bestArmor = armor->armor;
		bestIndex = i;
	}

	if (bestIndex == -1) {
		player->currArmor = 0;
		player->activeArmorID = -1;
	} else {
		Armor* bestArmor = (Armor*)player->inventory[bestIndex];
		bestArmor->OnUse(player);
	}
}
