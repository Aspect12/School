
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>

// Global enums
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_RETURN 13

// Global variables
int lineCount, currentScheme;
bool fullscreen;
char* colorSchemes[5] = {"color 0F", "color F0", "color E0", "color 0A", "color 8F"};

// Game Variables
int plyOneHealth = 20, plyTwoHealth = 20;
int plyOneDamage = 0, plyTwoDamage = 0;
int plyOneSpecials = 2, plyTwoSpecials = 2;
bool plyOneDefending = false, plyTwoDefending = false;

void ToggleWindow(bool maximized) {
	if (maximized) {
		ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	}
	else {
		ShowWindow(GetConsoleWindow(), SW_RESTORE);

		HWND hwnd = GetConsoleWindow();
		RECT rect;
		GetWindowRect(hwnd, &rect);
		int width = 1280;
		int height = 720;
		int x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		int y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
		SetWindowPos(hwnd, 0, x, y, width, height, SWP_NOZORDER | SWP_SHOWWINDOW);
	}
}

void CheckForNewlines(char text[]) {
	for (size_t i = 0; i < strlen(text); i++) {
		if (text[i] == '\n') {
			lineCount++;
		}
	}
}

// printf function but with line count updates
void PrintOut(char *format, ...) {
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	CheckForNewlines(format);
}

// Function to clear the screen and reset the line count
void ClearScreen() {
	system("cls");

	lineCount = 0;
}

void SetCursorPosition(int cursorPos) {
	COORD pos = {0, cursorPos};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void ShowMenu();

void EndGame(bool plyOneWin) {
	Sleep(500);

	if (plyOneWin) {
		PrintOut("Player One Wins!\n\n");
	}
	else {
		PrintOut("Player Two Wins!\n\n");
	}

	PrintOut("Player One dealt %d damage and has %d health remaining.\n", plyOneDamage, plyOneHealth);
	PrintOut("Player Two dealt %d damage and has %d health remaining.\n\n", plyTwoDamage, plyTwoHealth);

	Sleep(500);
	PrintOut("\n\n    Press any key to return to the main menu . . .");

	getch();

	ShowMenu();
}

// Function to dynamically handle the game options.
int HandleGameOption(int options, char* optionNames[], int selection, bool isMenu) {
	int keyCode;

	for (int i = 0; i < options; i++) {
		if (i + 1 == selection) {
			PrintOut("    [X] ");
		} else {
			PrintOut("    [ ] ");
		}

		PrintOut(optionNames[i]);
		PrintOut("\n");
	}

	gameOptionInput:
		keyCode = getch();

		if (keyCode == 0 || keyCode == 224) { // Arrow key codes
			keyCode = getch();

			switch (keyCode) {
				case KEY_UP:
					if (selection == 1) {
						selection = options;
					} else {
						selection--;
					}

					SetCursorPosition(lineCount - options);
					lineCount = lineCount - options;

					for (int i = 0; i < options; i++) {
						if (i + 1 == selection) {
							PrintOut("    [X] ");
						} else {
							PrintOut("    [ ] ");
						}

						PrintOut(optionNames[i]);
						PrintOut("\n");
					}

					goto gameOptionInput;
				case KEY_DOWN:
					if (selection == options) {
						selection = 1;
					} else {
						selection++;
					}

					SetCursorPosition(lineCount - options);
					lineCount = lineCount - options;

					for (int i = 0; i < options; i++) {
						if (i + 1 == selection) {
							PrintOut("    [X] ");
						} else {
							PrintOut("    [ ] ");
						}

						PrintOut(optionNames[i]);
						PrintOut("\n");
					}

					goto gameOptionInput;
				default:
					goto gameOptionInput;
			}
		}
		else if (keyCode == KEY_RETURN) {
			SetCursorPosition(lineCount - options);

			// Wipe all options
			for (int i = 0; i < options; i++) {
				PrintOut("                                                            \n");
			}
			PrintOut("\n");

			// lineCount - options - the wipe above
			lineCount = lineCount - options * 2 - 1;
			SetCursorPosition(lineCount);
			
			if (!isMenu) {
				PrintOut("    >");
				PrintOut(optionNames[selection - 1]);
				PrintOut("\n\n");

				Sleep(500);
			}

			return selection;
		}
		else {
			goto gameOptionInput;
		}
}

void ShowOptions();
void BeginGame();

void ShowMenu() {
	ClearScreen();

	// Title
	PrintOut("\n  $$$$$$\\   $$$$$$\\  $$\\   $$\\  $$$$$$\\   $$$$$$\\  $$\\       $$$$$$$$\\       $$$$$$$$\\ $$$$$$\\  $$$$$$\\  $$\\   $$\\ $$$$$$$$\\ $$$$$$$$\\ $$$$$$$\\\n");
	PrintOut(" $$  __$$\\ $$  __$$\\ $$$\\  $$ |$$  __$$\\ $$  __$$\\ $$ |      $$  _____|      $$  _____|\\_$$  _|$$  __$$\\ $$ |  $$ |\\__$$  __|$$  _____|$$  __$$\\\n");
	PrintOut(" $$ /  \\__|$$ /  $$ |$$$$\\ $$ |$$ /  \\__|$$ /  $$ |$$ |      $$ |            $$ |        $$ |  $$ /  \\__|$$ |  $$ |   $$ |   $$ |      $$ |  $$ |\n");
	PrintOut(" $$ |      $$ |  $$ |$$ $$\\$$ |\\$$$$$$\\  $$ |  $$ |$$ |      $$$$$\\          $$$$$\\      $$ |  $$ |$$$$\\ $$$$$$$$ |   $$ |   $$$$$\\    $$$$$$$  |\n");
	PrintOut(" $$ |      $$ |  $$ |$$ \\$$$$ | \\____$$\\ $$ |  $$ |$$ |      $$  __|         $$  __|     $$ |  $$ |\\_$$ |$$  __$$ |   $$ |   $$  __|   $$  __$$< \n");
	PrintOut(" $$ |  $$\\ $$ |  $$ |$$ |\\$$$ |$$\\   $$ |$$ |  $$ |$$ |      $$ |            $$ |        $$ |  $$ |  $$ |$$ |  $$ |   $$ |   $$ |      $$ |  $$ |\n");
	PrintOut(" \\$$$$$$  | $$$$$$  |$$ | \\$$ |\\$$$$$$  | $$$$$$  |$$$$$$$$\\ $$$$$$$\\        $$ |      $$$$$$\\ \\$$$$$$  |$$ |  $$ |   $$ |   $$$$$$$$\\ $$ |  $$ |\n");
	PrintOut("  \\______/  \\______/ \\__|  \\__| \\______/  \\______/ \\________|\\________|      \\__|      \\______| \\______/ \\__|  \\__|   \\__|   \\________|\\__|  \\__|\n\n");
	
	
	PrintOut(" Welcome. Select an option below to begin.\n");
	PrintOut(" Navigate using the arrow keys.\n\n");

	int menuSelection = HandleGameOption(3, (char*[]){"Start Game", "Options", "Exit"}, 1, true);

	switch (menuSelection) {
		case 1: // Start Game
			BeginGame();
			ShowMenu();

			return;
		case 2: // Options
			ShowOptions();
			ShowMenu();

			return;
		case 3: // Exit
			exit(0);

			return;
	}
}

void ShowOptions() {
	ClearScreen();

	// Title
	PrintOut("\n   /$$$$$$  /$$$$$$$  /$$$$$$$$ /$$$$$$  /$$$$$$  /$$   /$$  /$$$$$$ \n");
	PrintOut("  /$$__  $$| $$__  $$|__  $$__/|_  $$_/ /$$__  $$| $$$ | $$ /$$__  $$\n");
	PrintOut(" | $$  \\ $$| $$  \\ $$   | $$     | $$  | $$  \\ $$| $$$$| $$| $$  \\__/\n");
	PrintOut(" | $$  | $$| $$$$$$$/   | $$     | $$  | $$  | $$| $$ $$ $$|  $$$$$$ \n");
	PrintOut(" | $$  | $$| $$____/    | $$     | $$  | $$  | $$| $$  $$$$ \\____  $$\n");
	PrintOut(" | $$  | $$| $$         | $$     | $$  | $$  | $$| $$\\  $$$ /$$  \\ $$\n");
	PrintOut(" |  $$$$$$/| $$         | $$    /$$$$$$|  $$$$$$/| $$ \\  $$|  $$$$$$/\n");
	PrintOut("  \\______/ |__/         |__/   |______/ \\______/ |__/  \\__/ \\______/ \n\n");
	
	PrintOut(" Configure your game experience.\n\n");

	int menuSelection = 1;
	optionSelection:
		menuSelection = HandleGameOption(3, (char*[]){"Toggle Full Screen", "Cycle Color Scheme", "Return"}, menuSelection, true);

		switch (menuSelection) {
			case 1: // Toggle Full Screen
				ToggleWindow(!fullscreen);
				fullscreen = !fullscreen;

				goto optionSelection;
			case 2: // Cycle Color Scheme
				currentScheme++;
				if (currentScheme > 4) currentScheme = 0;

				system(colorSchemes[currentScheme]);

				goto optionSelection;
			case 3: // Return
				return;
		}
}

void PlayerTwoTurn();

void PlayerOneTurn() {
	PrintOut("Player One's Turn:\n\n");

	plyOneDefending = false;
	
	int selection = HandleGameOption(3, (char*[]){"Attack", "Defend", "Special"}, 1, false);
	int damage;

	switch (selection) {
		case 1:
			damage = rand() % 3 + 1;

			if (plyTwoDefending) {
				damage--;
				plyTwoHealth = max(0, plyTwoHealth - damage);
				plyOneDamage += damage;

				PrintOut("Player One Attacks! Player Two's defense removes 1 point from Player One's attack, dealing %d points of damage and leaving them at %d Health!\n\n", damage, plyTwoHealth);
			}
			else {
				plyTwoHealth = max(0, plyTwoHealth - damage);
				plyOneDamage += damage;
				
				PrintOut("Player One Attacks, dealing %d points of damage to Player Two, leaving them at %d Health!\n\n", damage, plyTwoHealth);
			}

			break;
		case 2:
			PrintOut("Player One Defends!\n\n");

			plyOneDefending = true;

			break;
		case 3:
			if (plyOneSpecials == 0) {
				PrintOut("You do not have any Special Attacks left!\n\n");

				PlayerOneTurn();
				return;
			}

			plyOneSpecials--;

			if (plyTwoDefending) {
				plyTwoHealth = max(0, plyTwoHealth - 3);
				plyOneDamage += 3;

				PrintOut("Player One uses Special Attack, dealing 3 points of damage (1 removed from Player Two's defense) and leaving them at %d Health! They have %d Special Attacks left.\n\n", plyTwoHealth, plyOneSpecials);
			}
			else {
				plyTwoHealth = max(0, plyTwoHealth - 4);
				plyOneDamage += 4;

				PrintOut("Player One uses Special Attack, dealing 4 points of damage and leaving them at %d Health! They have %d Special Attacks left.\n\n", plyTwoHealth, plyOneSpecials);

				plyOneHealth += 2;

				PrintOut("Player One gains 2 Health points by dealing more than 3 damage, leaving them at %d Health!\n\n", plyOneHealth);
			}

			break;
	}

	if (plyTwoHealth <= 0) {
		EndGame(true);

		return;
	}

	PlayerTwoTurn();
}

void PlayerTwoTurn() {
	PrintOut("Player Two's Turn:\n\n");

	plyTwoDefending = false;

	int selection = HandleGameOption(3, (char*[]){"Attack", "Defend", "Special"}, 1, false);
	int damage;

	switch (selection) {
		case 1:
			damage = rand() % 3 + 1;

			if (plyOneDefending) {
				damage--;
				plyOneHealth = max(0, plyOneHealth - damage);
				plyTwoDamage += damage;

				PrintOut("Player Two Attacks! Player One's defense removes 1 point from Player Two's attack, dealing %d points of damage and leaving them at %d Health!\n\n", damage, plyOneHealth);
			}
			else {
				plyOneHealth = max(0, plyOneHealth - damage);
				plyTwoDamage += damage;
				
				PrintOut("Player Two Attacks, dealing %d points of damage to Player One, leaving them at %d Health!\n\n", damage, plyOneHealth);
			}

			break;
		case 2:
			PrintOut("Player Two Defends!\n\n");

			plyTwoDefending = true;

			break;
		case 3:
			if (plyTwoSpecials == 0) {
				PrintOut("You do not have any Special Attacks left!\n\n");

				PlayerTwoTurn();
				return;
			}

			plyTwoSpecials--;

			if (plyOneDefending) {
				plyOneHealth = max(0, plyOneHealth - 3);
				plyTwoDamage += 3;

				PrintOut("Player Two uses Special Attack, dealing 3 points of damage (1 removed from Player One's defense) and leaving them at %d Health! They have %d Special Attacks left.\n\n", plyOneHealth, plyTwoSpecials);
			}
			else {
				plyOneHealth = max(0, plyOneHealth - 4);
				plyTwoDamage += 4;

				PrintOut("Player Two uses Special Attack, dealing 4 points of damage and leaving them at %d Health! They have %d Special Attacks left.\n\n", plyOneHealth, plyTwoSpecials);

				plyTwoHealth += 2;

				PrintOut("Player Two gains 2 Health points by dealing more than 3 damage, leaving them at %d Health!\n\n", plyTwoHealth);
			}

			break;
	}

	if (plyOneHealth <= 0) {
		EndGame(false);

		return;
	}

	PlayerOneTurn();
}

void BeginGame() {
	ClearScreen();
	Sleep(1000);

	plyOneHealth = 20, plyTwoHealth = 20;
	plyOneDamage = 0, plyTwoDamage = 0;
	plyOneSpecials = 2, plyTwoSpecials = 2;
	plyOneDefending = false, plyTwoDefending = false;

	PlayerOneTurn();
}

int main() {
	// Setup
	system("title Console Fighter");
	ToggleWindow(false);

	srand(time(NULL));

	system(colorSchemes[0]);

	ShowMenu();

	return 0;
}
