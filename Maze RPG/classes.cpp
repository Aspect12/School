
#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>

#include "functions.h"
#include "classes.h"
#include "items.h"

using namespace std;

vector<Item*> items;
vector<Enemy*> enemies;

int enemyIDIterator = 1;

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

	maxItems = 7;
	inCombat = -1;
//	activeWeaponID = -1;
	activeArmorID = -1;

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

		// if (!noDestroy) delete inventory[i];

		inventory.erase(inventory.begin() + i);

		break;
	}
}

void Player::Interact(char& object, int& x, int& y) {
	switch (object) {
		case '#': // Wall
			AppendActionResult(currName + " push the wall. Nothing happens.");

			break;
		case 'M': // Monster
		case 'B': // Boss
			if (activeWeaponID == -1) {
				AppendActionResult(currName + " don't have a weapon to attack with!");

				break;
			}

			for (int i = 0; i < enemies.size(); ++i) {
				if (enemies[i]->x != x || enemies[i]->y != y) continue;

				Enemy* enemy = enemies[i];

				if (inCombat == -1 && inCombat != enemy->uniqueID) {
					AppendActionResult(currName + " engaged in combat with the " + enemy->name + "! ");
					inCombat = enemy->uniqueID;

					Sleep(1000);

					// Roll a d20 for initiative
					int playerInitiative = rand() % 20 + 1;
					int enemyInitiative = rand() % 20 + 1;

					if (playerInitiative > enemyInitiative || playerInitiative == enemyInitiative) {
						AppendActionResult(currName + " gains initiative!");
					} else {
						AppendActionResult("The " + enemy->name + " gains initiative and strikes first!");

						Sleep(1000);

						enemy->Attack(this);
					}

					break;
				}

				string result = currName + " attacks the " + enemy->name + " for " + to_string(currAttack) + " damage, ";

				enemy->health -= currAttack;

				if (enemy->health <= 0) {
					if (((Boss*)enemy)->maxHealth == 35 && !((Boss*)enemy)->enraged) {
						result += "slaying it! ";

						AppendActionResult(result);

						DrawBoss(this, ' ', x, y);
						DrawMap();

						Sleep(2000);

						AppendActionResult(currName + " have defeated the " + enemy->name + "! You win! Press any key to exit...");

						getch();
						exit(0);
					} else {
						result += "slaying it! It drops a " + enemy->healthPotName + ".";
						enemies.erase(enemies.begin() + i);

						Item* potion = new Potion(x, y, enemy->healthPotName, enemy->healthPotValue);
						items.push_back(potion);

						inCombat = -1;

						AppendActionResult(result);
					}
				} else {
					result += "leaving it with " + to_string(enemy->health) + " health!";

					AppendActionResult(result);

					Sleep(1000);

					// If the enemy's health is at 50% or less, turn it into a Boss
					if (enemy->health <= enemy->maxHealth * 0.5 && ((Boss*)enemy)->maxHealth == 35 && !((Boss*)enemy)->enraged) {
						AppendActionResult("The " + enemy->name + " becomes enraged!");

						Boss* boss = (Boss*)enemy;

						boss->name = "Enraged " + boss->name;
						boss->enraged = true;
						boss->health = 60;
						boss->armor = 5;
						boss->attack = 12;

						RegisterMapChange('B', x, y);

						// Make the boss larger
						DrawBoss(this, 'B', x, y);
						DrawMap();

						Sleep(2000);
					}

					if (((Boss*)enemy)->enraged) {
						((Boss*)enemy)->BossAttack(this);
					} else {
						enemy->Attack(this);
					}
				}

				break;
			}

			break;
		case 'C': // Chest
			for (int i = 0; i < chests.size(); ++i) {
				if (chests[i]->x != x || chests[i]->y != y) continue;

				Chest* chest = chests[i];

				AppendActionResult(currName + " open the chest and find: " + chest->GetListOfItems());

				// Give the items to the player
				int itemsTaken = 0;

				for (int j = 0; j < chest->chestItems.size(); ++j) {
					if (inventory.size() >= maxItems) {
						AppendActionResult(currName + "'s inventory is full! They cannot take any more items from the chest.");

						break;
					}

					this->AddItem(chest->chestItems[j]);
					itemsTaken++;
				}

				// Remove the items from the chest
				chest->chestItems.erase(chest->chestItems.begin(), chest->chestItems.begin() + itemsTaken);

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

				if (inventory.size() >= maxItems) {
					AppendActionResult(currName + "'s inventory is full! They cannot pick up the " + item->itemName + ".");

					break;
				}

				AppendActionResult(currName + " pick up a " + item->itemName + ".");
				this->AddItem(item);
				items.erase(items.begin() + i);

				RegisterMapChange(' ', x, y);

				DrawSideText(this);

				break;
			}

			break;
		default:
			AppendActionResult("There is nothing in that direction!");

			break;
	}
}

// An 'Enemy' class to represent an enemy
Enemy::Enemy(int newX, int newY, string newName, int newHealth, int newMaxHealth, int newAttack, int newArmor, string potName, int potValue) {
	x = newX;
	y = newY;
	name = newName;
	health = newHealth;
	maxHealth = newMaxHealth;
	attack = newAttack;
	armor = newArmor;
	healthPotName = potName;
	healthPotValue = potValue;

	uniqueID = enemyIDIterator++;

	enemies.push_back(this);
	RegisterMapChange('M', x, y);
};

void Enemy::Attack(Player* player) {
	int damage = player->currArmor - attack;

	if (player->currArmor > 0) {
		Armor* armor;

		for (int i = 0; i < player->inventory.size(); ++i) {
			if (player->inventory[i]->uniqueID != player->activeArmorID) continue;

			armor = (Armor*)player->inventory[i];
			break;
		}

		if (damage <= 0) {
			AppendActionResult("The " + name + " attacks " + player->currName + " for " + to_string(attack) + " damage, breaking " + player->currName + "'s " + armor->itemName + "!");

			player->RemoveItem(player->activeArmorID, false);
		} else {
			player->currArmor = damage;

			armor->armor = damage;

			AppendActionResult("The " + name + " attacks " + player->currName + " for " + to_string(attack) + " damage, leaving them with " + to_string(damage) + " armor!");
		}

		DrawSideText(player);
	} else {
		player->currHealth = max(0, player->currHealth - attack);

		DrawSideText(player);

		if (player->currHealth == 0) {
			AppendActionResult("The " + name + " attacks " + player->currName + " for " + to_string(attack) + " damage, killing them!");

			RegisterMapChange(' ', player->x, player->y);
			DrawMap();

			Sleep(2000);

			// Game over
			AppendActionResult("Game over! Press any key to exit...");

			getch();
			exit(0);
		} else {
			AppendActionResult("The " + name + " attacks " + player->currName + " for " + to_string(attack) + " damage, leaving them with " + to_string(player->currHealth) + " health!");
		}
	}
}

// A 'Boss' class to represent a boss, based on the 'Enemy' class
Boss::Boss(int newX, int newY, string newName, int newHealth, int newMaxHealth, int newAttack, int newArmor, string potName, int potValue) : Enemy(newX, newY, newName, newHealth, newMaxHealth, newAttack, newArmor, potName, potValue) {
	enraged = false;
};

void Boss::BossAttack(Player* player) {
	int damage = player->currArmor - attack;

	if (player->currArmor > 0) {
		Armor* armor;

		for (int i = 0; i < player->inventory.size(); ++i) {
			if (player->inventory[i]->uniqueID != player->activeArmorID) continue;

			armor = (Armor*)player->inventory[i];
			break;
		}

		if (damage <= 0) {
			AppendActionResult("The " + name + " strikes " + player->currName + " for " + to_string(attack) + " damage, destroying " + player->currName + "'s " + armor->itemName + "!");

			player->RemoveItem(player->activeArmorID, false);
		} else {
			player->currArmor = damage;

			armor->armor = damage;

			AppendActionResult("The " + name + " strikes " + player->currName + " for " + to_string(attack) + " damage, leaving them with " + to_string(damage) + " armor!");
		}

		DrawSideText(player);
	} else {
		player->currHealth = max(0, player->currHealth - attack);

		DrawSideText(player);

		if (player->currHealth == 0) {
			AppendActionResult("The " + name + " strikes " + player->currName + " for " + to_string(attack) + " damage, slaying them!");

			RegisterMapChange(' ', player->x, player->y);
			DrawMap();

			Sleep(2000);

			// Game over
			AppendActionResult("Game over! Press any key to exit...");

			getch();
			exit(0);
		} else {
			AppendActionResult("The " + name + " strikes " + player->currName + " for " + to_string(attack) + " damage, leaving them with " + to_string(player->currHealth) + " health!");
		}
	}
}

// A 'Chest' class to store items
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
