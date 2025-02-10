
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cctype>

using namespace std;

string encryptSubstitution(string message, int blockSize, string key);
string decryptSubstitution(string message, int blockSize, string key);

string encryptTransposition(string message, int blockSize, string key);
string decryptTransposition(string message, int blockSize, string key);

int main() {
    int encryptionType;
    cout << "Choose encryption type:" << endl;
    cout << "1. Substitution Encryption" << endl;
    cout << "2. Transposition Encryption" << endl;
    cout << "Enter choice: ";
    cin >> encryptionType;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int operation;
    cout << "\nChoose operation:" << endl;
    cout << "1. Encrypt" << endl;
    cout << "2. Decrypt" << endl;
    cout << "Enter choice: ";
    cin >> operation;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // SUBSTITUTION ENCRYPTION
    if (encryptionType == 1) {
        if (operation == 1) { // Encrypt
            string message;
            int blockSize;
            string key;

            // Get message
            while (true) {
                cout << "\nEnter the message: ";

                getline(cin, message);

                while (!message.empty() && isspace(message.back())) message.pop_back();
                if (!message.empty()) break;

                cout << "Message cannot be empty. Please enter a valid message." << endl;
            }

            // Get block size
            while (true) {
                cout << "Enter the block size: ";
                cin >> blockSize;

                if (!cin.fail() && blockSize >= 1) break;

                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Invalid block size. Please enter a positive integer." << endl;
            }
            if (blockSize > (int)message.length()) {
                blockSize = message.length();

                cout << "Block size out of bounds; clamping to " << blockSize << "." << endl;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // Get key
            while (true) {
                cout << "Enter the key (numbers separated by space): ";

                getline(cin, key);
                vector<int> keyNumbers;
                string temp;

                bool valid = true;

                for (char c : key) {
                    if (isdigit(c) || c == ' ') {
						if (c == ' ' && !temp.empty()) {
							try {
								int num = stoi(temp);

								num = num % 26;
								if (num == 0) num = 26;

								keyNumbers.push_back(num);
							} catch (const std::out_of_range& e) {
								cout << "The number \"" << temp << "\" is too large. Please enter smaller numbers for the key." << endl;
							
								valid = false;
								
								break; 
							}
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
					try {
						int num = stoi(temp);

						num = num % 26;
						if (num == 0) num = 26;

						keyNumbers.push_back(num);
					} catch (const std::out_of_range& e) {
						cout << "The number \"" << temp << "\" is too large. Please enter smaller numbers for the key." << endl;
					
						valid = false;
					}
				}

                if (valid && keyNumbers.size() == (unsigned)blockSize) {
                    key.clear();

                    for (int num : keyNumbers) key += to_string(num);

                    break;
                } else if (valid) {
                    cout << "Key length must be equal to block size." << endl;
                }

                if (!valid) {
                    cin.clear();
                //    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }

            cout << "\nOriginal message: " << message << endl;
            string encrypted = encryptSubstitution(message, blockSize, key);
            cout << "Encrypted message: " << encrypted << endl;
        }
        else if (operation == 2) { // Decrypt
            string encryptedMessage;
            int blockSize;
            string key;

            // Get message
            while (true) {
                cout << "\nEnter the encrypted message: ";

                getline(cin, encryptedMessage);
                
				while (!encryptedMessage.empty() && isspace(encryptedMessage.back()))
                    encryptedMessage.pop_back();

                if (!encryptedMessage.empty()) break;

                cout << "Encrypted message cannot be empty. Please enter a valid message." << endl;
            }

            // Get block size
            while (true) {
                cout << "Enter the block size: ";
                cin >> blockSize;

                if (!cin.fail() && blockSize >= 1) break;

                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Invalid block size. Please enter a positive integer." << endl;
            }
            if (blockSize > (int)encryptedMessage.length()) {
                blockSize = encryptedMessage.length();

                cout << "Block size out of bounds; clamping to " << blockSize << "." << endl;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // Get  key
            while (true) {
                cout << "Enter the key (numbers separated by space): ";

                getline(cin, key);
                vector<int> keyNumbers;
                string temp;
                bool valid = true;

                for (char c : key) {
                    if (isdigit(c) || c == ' ') {
                        if (c == ' ' && !temp.empty()) {
							try {
								int num = stoi(temp);

								num = num % 26;
								if (num == 0) num = 26;

								keyNumbers.push_back(num);
							} catch (const std::out_of_range& e) {
								cout << "The number \"" << temp << "\" is too large. Please enter smaller numbers for the key." << endl;
								
								valid = false;

								break;
							}
							temp.clear();
						}
						else if (isdigit(c)) {
                            temp += c;
                        }
                    } else {
                        valid = false;
                        cout << "Key must be a positive integer." << endl;

                        break;
                    }
                }

				if (valid && !temp.empty()) {
					try {
						int num = stoi(temp);

						num = num % 26;
						if (num == 0) num = 26;

						keyNumbers.push_back(num);
					} catch (const std::out_of_range& e) {
						cout << "The number \"" << temp << "\" is too large. Please enter smaller numbers for the key." << endl;
						
						valid = false;
					}
				}

                if (valid && keyNumbers.size() == (unsigned)blockSize) {
                    key.clear();

                    for (int num : keyNumbers) key += to_string(num);

                    break;
                } else if (valid) {
                    cout << "Key length must be equal to block size." << endl;
                }

                if (!valid) {
                    cin.clear();
                //    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }

            cout << "\nEncrypted message: " << encryptedMessage << endl;
            string decrypted = decryptSubstitution(encryptedMessage, blockSize, key);
            cout << "Decrypted message: " << decrypted << endl;
        }
        else {
            cout << "Invalid operation choice." << endl;
        }
    }
    // TRANSPOSITION ENCRYPTION
    else if (encryptionType == 2) {
        if (operation == 1) { // Encrypt
            string message;
            int blockSize;
            string key;

            // Get message
            while (true) {
                cout << "\nEnter the message: ";

                getline(cin, message);
                
				while (!message.empty() && isspace(message.back())) message.pop_back();
                if (!message.empty()) break;

                cout << "Message cannot be empty. Please enter a valid message." << endl;
            }

            // Get block size
            while (true) {
                cout << "Enter the block size: ";
                cin >> blockSize;

                if (!cin.fail() && blockSize >= 1) break;
                
				cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Invalid block size. Please enter a positive integer." << endl;
            }

            // Limit blockSize to 9.
            if (blockSize > (int)message.length() || blockSize > 9) {
                int max;

                if (message.length() <= 9)
                    max = message.length();
                else
                    max = 9;

                blockSize = max;
                cout << "Block size out of bounds; clamping to " << max << "." << endl;
            }

            // Get key
            while (true) {
                cout << "Enter the key: ";
                cin >> key;

                bool valid = true;

                if (key.length() != (unsigned)blockSize) {
                    cout << "Key length must be equal to block size." << endl;
                } else {
                    vector<bool> seen(blockSize + 1, false);

                    for (char c : key) {
                        if (!isdigit(c)) {
                            valid = false;

                            cout << "Each character of the key must be a digit." << endl;

                            break;
                        }

                        int digit = c - '0';

                        if (digit > blockSize || digit <= 0) {
                            valid = false;

                            cout << "Each digit must be between 1 and the block size." << endl;

                            break;
                        }
						
                        if (seen[digit]) {
                            valid = false;

                            cout << "Each digit of the key must be unique." << endl;

                            break;
                        }

                        seen[digit] = true;
                    }

                    if (valid) break;
                }

                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            // Reverse key
            for (int i = 0; i < key.length() / 2; i++) {
                char temp = key[i];

                key[i] = key[key.length() - i - 1];
                key[key.length() - i - 1] = temp;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\nOriginal message: " << message << endl;
            string encrypted = encryptTransposition(message, blockSize, key);
            cout << "Encrypted message: " << encrypted << endl;
        }
        else if (operation == 2) { // Decrypt
            string encryptedMessage;
            int blockSize;
            string key;

            // Get message
            while (true) {
                cout << "\nEnter the encrypted message: ";

                getline(cin, encryptedMessage);

                while (!encryptedMessage.empty() && isspace(encryptedMessage.back()))
                    encryptedMessage.pop_back();

                if (!encryptedMessage.empty()) break;

                cout << "Encrypted message cannot be empty. Please enter a valid message." << endl;
            }

            // Get block size
            while (true) {
                cout << "Enter the block size: ";
                cin >> blockSize;

                if (!cin.fail() && blockSize >= 1) break;

                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Invalid block size. Please enter a positive integer." << endl;
            }

            if (blockSize > (int)encryptedMessage.length() || blockSize > 9) {
                int max;

                if (encryptedMessage.length() <= 9)
                    max = encryptedMessage.length();
                else
                    max = 9;

                blockSize = max;
                cout << "Block size out of bounds; clamping to " << max << "." << endl;
            }

            // Get key
            while (true) {
                cout << "Enter the key: ";
                cin >> key;

                bool valid = true;

                if (key.length() != (unsigned)blockSize) {
                    cout << "Key length must be equal to block size." << endl;
                } else {
                    vector<bool> seen(blockSize + 1, false);
					
                    for (char c : key) {
                        if (!isdigit(c)) {
                            valid = false;
							
                            cout << "Each character of the key must be a digit." << endl;

                            break;
                        }
						
                        int digit = c - '0';

                        if (digit > blockSize || digit <= 0) {
                            valid = false;

                            cout << "Each digit must be between 1 and the block size." << endl;

                            break;
                        }

                        if (seen[digit]) {
                            valid = false;

                            cout << "Each digit of the key must be unique." << endl;

                            break;
                        }

                        seen[digit] = true;
                    }

                    if (valid) break;
                }

                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            // Reverse key
            for (int i = 0; i < key.length() / 2; i++) {
                char temp = key[i];

                key[i] = key[key.length() - i - 1];
                key[key.length() - i - 1] = temp;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\nEncrypted message: " << encryptedMessage << endl;
            string decrypted = decryptTransposition(encryptedMessage, blockSize, key);
            cout << "Decrypted message: " << decrypted << endl;
        }
        else {
            cout << "Invalid operation choice." << endl;
        }
    }
    else {
        cout << "Invalid encryption type." << endl;
    }

    return 0;
}

string encryptSubstitution(string message, int blockSize, string key) {
    // Remove spaces
    for (int i = 0; i < message.length(); i++) {
        if (message[i] != ' ') continue;

        message.erase(i, 1);
    }

    // Convert to uppercase
    for (int i = 0; i < message.length(); i++) {
        if (message[i] >= 'a' && message[i] <= 'z')
            message[i] = message[i] - 32;
    }

    // Split message into blocks
    vector<string> blocks;
    for (int i = 0; i < message.length(); i += blockSize)
        blocks.push_back(message.substr(i, blockSize));

    for (int i = 0; i < blocks.size(); i++) {
        for (int j = 0; j < blocks[i].length(); j++) {
            int substitution = key[j] - '0';
            for (int k = 0; k < substitution; k++) {
                if (blocks[i][j] == 'Z')
                    blocks[i][j] = 'A';
                else
                    blocks[i][j]++;
            }
        }
    }

    // Join the blocks
    message = "";

    for (int i = 0; i < blocks.size(); i++)
        message += blocks[i];

    return message;
}

string decryptSubstitution(string message, int blockSize, string key) {
    // Split the message
    vector<string> blocks;

    for (int i = 0; i < message.length(); i += blockSize)
        blocks.push_back(message.substr(i, blockSize));

    for (int i = 0; i < blocks.size(); i++) {
        for (int j = 0; j < blocks[i].length(); j++) {
            int substitution = key[j] - '0';
            for (int k = 0; k < substitution; k++) {
                if (blocks[i][j] == 'A')
                    blocks[i][j] = 'Z';
                else
                    blocks[i][j]--;
            }
        }
    }

    // Join the blocks
    message = "";

    for (int i = 0; i < blocks.size(); i++)
        message += blocks[i];

    return message;
}

string encryptTransposition(string message, int blockSize, string key) {
    // Remove spaces
    for (int i = 0; i < message.length(); i++) {
        if (message[i] != ' ') continue;

        message.erase(i, 1);
    }

    // Convert to uppercase
    for (int i = 0; i < message.length(); i++) {
        if (message[i] >= 'a' && message[i] <= 'z')
            message[i] = message[i] - 32;
    }

    // Split the message
    vector<string> blocks;

    for (int i = 0; i < message.length(); i += blockSize) {
        string block = message.substr(i, blockSize);

        if (block.length() < blockSize)
            block.append(blockSize - block.length(), 'Z');

        blocks.push_back(block);
    }

    // Rearrange blocks according to key
    for (int i = 0; i < blocks.size(); i++) {
        string block = blocks[i];

        for (int j = 0; j < block.length(); j++) {
            blocks[i][j] = block[key[j] - '1'];
        }
    }

    // Join the blocks
    message = "";

    for (int i = 0; i < blocks.size(); i++)
        message += blocks[i];

    return message;
}

string decryptTransposition(string message, int blockSize, string key) {
    // Split message
    vector<string> blocks;

    for (int i = 0; i < message.length(); i += blockSize)
        blocks.push_back(message.substr(i, blockSize));

    // Reverse transposition
    for (int i = 0; i < blocks.size(); i++) {
        string block = blocks[i];

        for (int j = 0; j < block.length(); j++) {
            blocks[i][key[j] - '1'] = block[j];
        }
    }

    // Join the blocks
    message = "";
	
    for (int i = 0; i < blocks.size(); i++)
        message += blocks[i];

    return message;
}
