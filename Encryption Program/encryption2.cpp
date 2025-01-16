
#include <iostream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

string encrypt(string message, int blockSize, string key);
string decrypt(string message, int blockSize, string key);

int main() {
	string message;
	int blockSize;
	string key;

 	// Ask for input
	while (true) {
		cout << "Enter the message: ";
		getline(cin, message);

		while (!message.empty() && isspace(message.back())) {
			message.pop_back();
		}

		if (!message.empty()) break;

		cout << "Message cannot be empty. Please enter a valid message." << endl;
	}
	
	while (true) {
		cout << "Enter the block size: ";
		cin >> blockSize;

		if (!cin.fail() && blockSize >= 1) break;

		// Clear the error and discard the input
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		
		cout << "Invalid block size. Please enter a positive integer." << endl;
	}

	if (blockSize > message.length() || blockSize > 9) {
		int max;
		
		if (message.length() <= 9) {
			max = message.length();
		} else {
			max = 9;
		}

		blockSize = max;

		cout << "Block size out of bounds; clamping to " << max << "." << endl;
	}

	while (true) {
		cout << "Enter the key: ";
		cin >> key;

		bool valid = true;

		if (key.length() != blockSize) {
			cout << "Key length must be equal to block size." << endl;
		} else {
			vector<bool> seen(blockSize + 1, false);
			
			for (char c : key) {
				if (!isdigit(c)) {
					valid = false;

					cout << "Each character of the key must be a digit." << endl;

					break;
				}

				if ((c - '0') > blockSize || (c - '0') <= 0) {
					valid = false;

					cout << "Each digit of the key must be a positive integer between 1 and the block size." << endl;
				
					break;
				}

				if (seen[c - '0']) {
					valid = false;

					cout << "Each digit of the key must be unique." << endl;

					break;
				}

				seen[c - '0'] = true;
			}

			if (valid) break;
		}

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	// Reverse the key
	for (int i = 0; i < key.length() / 2; i++) {
		char temp = key[i];
		
		key[i] = key[key.length() - i - 1];
		key[key.length() - i - 1] = temp;
	}

	cout << "Original message: " << message << endl;

	string encrypted = encrypt(message, blockSize, key);
	cout << "Encrypted message: " << encrypted << endl;

	string decrypted = decrypt(encrypted, blockSize, key);
	cout << "Decrypted message: " << decrypted << endl;

	return 0;
}

string encrypt(string message, int blockSize, string key) {
	// Spaces
	for (int i = 0; i < message.length(); i++) {
		if (message[i] != ' ') continue;

		message.erase(i, 1);
	}

	// Capitals
	for (int i = 0; i < message.length(); i++) {
		if (message[i] < 'a' || message[i] > 'z') continue;

		message[i] = message[i] - 32;
	}

	// Split
	vector<string> blocks;

	for (int i = 0; i < message.length(); i += blockSize) {
		string block = message.substr(i, blockSize);

		if (block.length() < blockSize) {
			block.append(blockSize - block.length(), 'Z');
		}

		blocks.push_back(block);
	}

	// Move
	for (int i = 0; i < blocks.size(); i++) {
		string block = blocks[i];

		for (int j = 0; j < block.length(); j++) {
			blocks[i][j] = block[key[j] - '1'];
		}
	}

	// Join
	message = "";

	for (int i = 0; i < blocks.size(); i++) {
		message += blocks[i];
	}

	return message;
}

string decrypt(string message, int blockSize, string key) {
	// Split
	vector<string> blocks;

	for (int i = 0; i < message.length(); i += blockSize) {
		blocks.push_back(message.substr(i, blockSize));
	}

	// Move
	for (int i = 0; i < blocks.size(); i++) {
		string block = blocks[i];

		for (int j = 0; j < block.length(); j++) {
			blocks[i][key[j] - '1'] = block[j];
		}
	}

	// Join
	message = "";

	for (int i = 0; i < blocks.size(); i++) {
		message += blocks[i];
	}

	return message;
}
