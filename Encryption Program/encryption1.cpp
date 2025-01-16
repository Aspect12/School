
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

	if (blockSize > message.length()) {
		blockSize = message.length();

		cout << "Invalid block size entered; clamping to " << blockSize << "." << endl;
	}

	while (true) {
		cout << "Enter the key: ";
		cin >> key;

		bool valid = true;

		if (key.length() != blockSize) {
			valid = false;
			cout << "Key length must be equal to block size." << endl;
		} else {
			for (char c : key) {
				if (isdigit(c)) continue;
			
				valid = false;
				cout << "Key must be a positive integer." << endl;

				break;
			}
		}

		if (valid) break;

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
		blocks.push_back(message.substr(i, blockSize));
	}

	// Move
	for (int i = 0; i < blocks.size(); i++) {
		for (int j = 0; j < blocks[i].length(); j++) {
			int shift = key[j] - '0';

			for (int k = 0; k < shift; k++) {
				if (blocks[i][j] == 'Z') {
					blocks[i][j] = 'A';
				} else {
					blocks[i][j]++;
				}
			}
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
		for (int j = 0; j < blocks[i].length(); j++) {
			int shift = key[j] - '0';

			for (int k = 0; k < shift; k++) {
				if (blocks[i][j] == 'A') {
					blocks[i][j] = 'Z';
				} else {
					blocks[i][j]--;
				}
			}
		}
	}

	// Join
	message = "";

	for (int i = 0; i < blocks.size(); i++) {
		message += blocks[i];
	}

	return message;
}
