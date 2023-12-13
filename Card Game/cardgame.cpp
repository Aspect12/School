
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

using namespace std;

int main() {
	srand(time(0));
	
	vector<string> suits = {"Spades", "Diamonds", "Clubs", "Hearts"};
	vector<string> ranks = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

	vector<string> deck;

	for (int i = 0; i < suits.size(); ++i) {
		for (int j = 0; j < ranks.size(); ++j) {
			deck.push_back(ranks[j] + " of " + suits[i]);
		}
	}

	cout << "Initial Deck (" << deck.size() << "):" << endl;

	for (int i = 0; i < deck.size(); ++i) {
		cout << "	" << deck[i] << endl;
	}

	shuffle(deck.begin(), deck.end(), default_random_engine(time(0)));

	cout << endl << "Shuffled Deck (" << deck.size() << "):" << endl;

	for (int i = 0; i < deck.size(); ++i) {
		cout << "	" << deck[i] << endl;
	}

	vector<string> player1Inventory, player2Inventory;

	for (int i = 0; i < 5; ++i) {
		int random = rand() % deck.size();

		player1Inventory.push_back(deck[random]);
		deck.erase(deck.begin() + random);

		random = rand() % deck.size();

		player2Inventory.push_back(deck[random]);
		deck.erase(deck.begin() + random);
	}

	cout << endl << "Player 1's cards:" << endl;
	
	for (int i = 0; i < player1Inventory.size(); ++i) {
		cout << "	" << player1Inventory[i] << endl;
	}

	cout << endl << "Player 1, select which cards you'd like to swap:" << endl;
	cout << "	0. Stop" << endl;

	for (int i = 0; i < player1Inventory.size(); ++i) {
		cout << "	" << i + 1 << ". " << player1Inventory[i] << endl;
	}

	cout << endl;
	
	for (int i = 0; i < player1Inventory.size(); ++i) {
		int choice;
		cin >> choice;

		if (choice == 0) {
			cout << "You have stopped swapping cards." << endl;
			
			break;
		}

		int random = rand() % deck.size();

		cout << "You have swapped the " << player1Inventory[choice - 1] << " with the " << deck[random] << endl;

		player1Inventory[choice - 1] = deck[random];
		deck.erase(deck.begin() + random);
	}

	cout << endl << "Player 2's cards:" << endl;

	for (int i = 0; i < player2Inventory.size(); ++i) {
		cout << "	" << player2Inventory[i] << endl;
	}

	cout << endl << "Player 2, select which cards you'd like to swap:" << endl;
	cout << "	0. Stop" << endl;

	for (int i = 0; i < player2Inventory.size(); ++i) {
		cout << "	" << i + 1 << ". " << player2Inventory[i] << endl;
	}

	cout << endl;
	
	for (int i = 0; i < player2Inventory.size(); ++i) {
		int choice;
		cin >> choice;

		if (choice == 0) {
			cout << "You have stopped swapping cards." << endl;

			break;
		}

		int random = rand() % deck.size();

		cout << "You have swapped the " << player2Inventory[choice - 1] << " with the " << deck[random] << endl;

		player2Inventory[choice - 1] = deck[random];
		deck.erase(deck.begin() + random);
	}

	cout << endl << "Final Deck (" << deck.size() << "):" << endl;

	for (int i = 0; i < deck.size(); ++i) {
		cout << "	" << deck[i] << endl;
	}

	cout << endl << "Player 1's cards:" << endl;

	for (int i = 0; i < player1Inventory.size(); ++i) {
		cout << "	" << player1Inventory[i] << endl;
	}

	cout << endl << "Player 2's cards:" << endl;

	for (int i = 0; i < player2Inventory.size(); ++i) {
		cout << "	" << player2Inventory[i] << endl;
	}
}
