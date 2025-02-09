
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

		cout << "Block size out of bounds; clamping to " << blockSize << "." << endl;
	}

	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	while (true) {
		cout << "Enter the key (numbers separated by space): ";
		getline(cin, key);

		vector<int> keyNumbers;
		string temp;
		bool valid = true;

		for (char c : key) {
			if (isdigit(c) || c == ' ') {
				if (c == ' ' && !temp.empty()) {
					int num = stoi(temp) % 26;
					if (num == 0) num = 26;

					keyNumbers.push_back(num);
					temp.clear();
				} else if (isdigit(c)) {
					temp += c;
				}
			} else {
				valid = false;
				cout << "Key must be a positive integer." << endl;

				break;
			}
		}

		if (valid && !temp.empty()) {
			int num = stoi(temp) % 26;
			if (num == 0) num = 26;
			
			keyNumbers.push_back(num);
		}

		if (valid && keyNumbers.size() == blockSize) {
			key.clear();

			for (int num : keyNumbers) {
				key += to_string(num);
			}
			
			break;
		} else {
			cout << "Key length must be equal to block size." << endl;
			valid = false;
		}

		if (!valid) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
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
