
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>

// Global enums
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_RETURN 13

// Global variables
int lineCount, currentScheme;
bool fullscreen;
char* colorSchemes[5] = {"color 0F", "color F0", "color E0", "color 0A", "color 8F"};

// Global game variables
bool hasKeycode, knowsLayout, gateOpen;

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

// Function to slowly type out text
void TypeText(int speed, char text[]) {
	CheckForNewlines(text);
	
	for (size_t i = 0; i < strlen(text); i++) {
		PrintOut("%c", text[i]);
		Sleep(speed);
	}
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

void EndGame() {
	Sleep(500);

	TypeText(200, "The End.");
	Sleep(500);
	TypeText(50, "\n\n    Press any key to return to the main menu . . .");

	getch();	
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
	PrintOut("\n  /$$        /$$$$$$  /$$$$$$$        /$$$$$$$$  /$$$$$$   /$$$$$$   /$$$$$$  /$$$$$$$  /$$$$$$$$\n");
	PrintOut(" | $$       /$$__  $$| $$__  $$      | $$_____/ /$$__  $$ /$$__  $$ /$$__  $$| $$__  $$| $$_____/ \n");
	PrintOut(" | $$      | $$  \\ $$| $$  \\ $$      | $$      | $$  \\__/| $$  \\__/| $$  \\ $$| $$  \\ $$| $$       \n");
	PrintOut(" | $$      | $$$$$$$$| $$$$$$$       | $$$$$   |  $$$$$$ | $$      | $$$$$$$$| $$$$$$$/| $$$$$    \n");
	PrintOut(" | $$      | $$__  $$| $$__  $$      | $$__/    \\____  $$| $$      | $$__  $$| $$____/ | $$__/    \n");
	PrintOut(" | $$      | $$  | $$| $$  \\ $$      | $$       /$$  \\ $$| $$    $$| $$  | $$| $$      | $$       \n");
	PrintOut(" | $$$$$$$$| $$  | $$| $$$$$$$/      | $$$$$$$$|  $$$$$$/|  $$$$$$/| $$  | $$| $$      | $$$$$$$$\n");
	PrintOut(" |________/|__/  |__/|_______/       |________/ \\______/  \\______/ |__/  |__/|__/      |________/\n\n");
	
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

void BeginGame() {
	ClearScreen();
	Sleep(1000);

	// Reset game variables
	hasKeycode = false;
	knowsLayout = false;
	gateOpen = false;

	int selection;

	TypeText(25, "\nYou wake up inside a small, pristine lab. You're wearing some sort of jumpsuit, and you are strapped to a chair in the center of the room.\n\n");
	Sleep(500);

	selection = HandleGameOption(3, (char*[]){"Break out", "Yell for help", "Do nothing"}, 1, false);

	switch (selection) {
		case 1:
			TypeText(25, "You shake around, managing to eventually fall over, breaking the chair.\n");
			Sleep(250);
			TypeText(25, "You free yourself from the restraints and stand up.\n\n");

			case_2_1:
				selection = HandleGameOption(2, (char*[]){"Look around the room", "Yell for help"}, 1, false);

				switch (selection) {
					case 1:
						case_3_1:
							TypeText(25, "You look around the room. There is a computer on the desk, a door, and several cabinets and counters.\n\n");
							
							case_3_1_2:
								Sleep(500);
								selection = HandleGameOption(3, (char*[]){"Access the computer", "Go through the door", "Search the cabinets"}, 1, false);

								switch (selection) {
									case 1:
										TypeText(25, "You inspect the computer. It is locked, requiring a password to access.\n");

										if (hasKeycode) {
											Sleep(250);
											TypeText(25, "You enter the keycode you found earlier. The computer unlocks, and you are greeted with several documents.\n");
											Sleep(250);
											TypeText(25, "You read through the documents, and eventually find a blueprint of the facility. You study it carefully before closing the computer.\n\n");
											
											knowsLayout = true;

											goto case_3_1_2;
										}
										else {
											PrintOut("\n");
											goto case_3_1_2;
										}

										break;
									case 2:
										TypeText(25, "You open the door and walk out of the room.\n");

										if (knowsLayout) {
											Sleep(250);
											TypeText(25, "Knowing the layout of the facility, you make your way through it and eventually out into the parking lot.\n\n");

											case_4_2:
												Sleep(500);

												selection = HandleGameOption(4, (char*[]){"Break into a car", "Run off on foot", "Go to the front gate", "Go back"}, 1, false);

												switch (selection) {
													case 1:
														TypeText(25, "You run up to a nearby car, breaking the window and getting inside.\n\n");
														Sleep(500);

														selection = HandleGameOption(2, (char*[]){"Search the car", "Hotwire the car"}, 1, false);

														switch (selection) {
															case 1:
																TypeText(25, "You search the car, finding various items of interest. Among them are a spare set of keys.\n");
																Sleep(250);
																TypeText(25, "You insert the keys into the ignition, and the car starts.\n\n");

																case_6_1:
																	Sleep(500);

																	selection = HandleGameOption(2, (char*[]){"Drive off", "Honk the horn"}, 1, false);

																	switch (selection) {
																		case 1:
																			if (gateOpen) {
																				TypeText(25, "You hit the gas, driving through the open gate and away from the facility.\n");
																			}
																			else {
																				TypeText(25, "You hit the gas, ramming the closed gate of the facility. It violently breaks apart, allowing you to drive off.\n");
																			}

																			Sleep(250);
																			TypeText(25, "You drive off into the distance, never to return.\n\n    ");

																			EndGame();

																			return;
																		case 2:
																			TypeText(25, "You honk the horn, attracting the attention of several guards. They all sprint towards the car, opening fire.\n\n");
																			Sleep(250);
																			TypeText(25, "You get shot several times, the car glass breaking and getting everywhere. Everything begins to turn black as you slowly bleed out in the car.\n\n    ");

																			EndGame();

																			return;
																	}

																	break;
															case 2:
																TypeText(25, "You slowly hotwire the car, managing to get it started.\n\n");

																goto case_6_1;
														}
														
														break;
													case 2:
														TypeText(25, "You attempt to run off on foot. As you sprint down the road, you see a red dot on the road in front of you.\n");
														Sleep(250);
														TypeText(25, "You look up, and see a guard aiming his rifle at you from a watchtower. He shoots you in the head.\n");
														Sleep(250);
														TypeText(25, "You fall down to the ground. Everything begins to turn black as you slowly bleed out on the road.\n\n    ");

														EndGame();

														return;
													case 3:
														if (gateOpen) {
															TypeText(25, "You run up to the front gate to open it, but it is already wide open.\n\n");
														}
														else {
															TypeText(25, "You run up to the front gate, opening it completely.\n\n");

															gateOpen = true;
														}

														goto case_4_2;
													case 4:
														TypeText(25, "You walk back inside the facility. As you wander around, you eventually bump into another guard.\n");
														Sleep(250);
														TypeText(25, "He sees you, and immediately trains his rifle at you. You try to run, but the guard shoots you in the back.\n");
														Sleep(250);
														TypeText(25, "You fall down to the ground. Everything begins to turn black as you slowly bleed out on the road.\n\n    ");

														EndGame();

														return;
												}
										}
										else {
											case_4_3:
												Sleep(500);

												PrintOut("\n");
												selection = HandleGameOption(4, (char*[]){"Go left", "Go right", "Go straight", "Go back"}, 1, false);

												switch (selection) {
													case 1:
														TypeText(25, "You go left. You eventually reach an unlocked door, you open it, finding another guard and a couple of scientists inside.\n");
														Sleep(250);
														TypeText(25, "The scientists turn to run as the guard trains his rifle at you.\n");
														Sleep(250);
														TypeText(25, "You try to run, but the guard shoots you in the back.\n");
														Sleep(250);
														TypeText(25, "You fall down to the ground. Everything begins to turn black as you slowly bleed out on the floor.\n\n    ");

														EndGame();

														return;
													case 2:
														TypeText(25, "You go right. You eventually reach an unlocked door and open it. It leads you outside through the back of the facility.\n\n");

														goto case_4_2;
													case 3:
														TypeText(25, "You go straight. You find a bunch of locked doors on either side, with a window at the end of the hallway.\n");
														Sleep(250);
														TypeText(25, "You look through the window, seeing a parking lot outside, surrounded by a dense forest.\n");
														Sleep(250);
														TypeText(25, "You turn around and go back to where you started.\n");

														goto case_4_3;
													case 4:
														TypeText(25, "You turn around and go back into the lab room.\n\n");

														goto case_3_1_2;
												}
										}

										break;
									case 3:
										TypeText(25, "You search the cabinets. You find all sorts of lab equipment such as vials, small machines, gadgets, etc.\n");
										Sleep(250);
										TypeText(25, "Eventually, you find a note with 4 numbers printed on it. 4026.\n\n");

										hasKeycode = true;

										goto case_3_1_2;
								}

								break;
					case 2:
						TypeText(25, "You yell for help. A few seconds later, an armed guard comes rushing into the room.\n");
						Sleep(250);
						TypeText(25, "Upon seeing you freed and the chair broken, he trains his rifle at you.\n\n");
						Sleep(500);

						selection = HandleGameOption(2, (char*[]){"Run to cover", "Charge him"}, 1, false);

						switch (selection) {
							case 1:
								TypeText(25, "You run to cover behind a counter. The guard opens fire but misses you.\n\n");
								Sleep(500);

								selection = HandleGameOption(3, (char*[]){"Search the cabinet", "Try to run out of the room", "Do nothing"}, 1, false);

								switch (selection) {
									case 1:
										TypeText(25, "You search the cabinet. You find a small vial of a green liquid.\n\n");

										selection = HandleGameOption(2, (char*[]){"Drink the liquid", "Throw the vial at the guard"}, 1, false);

										switch (selection) {
											case 1:
												TypeText(25, "You drink the liquid. It begins to burn your insides, causing internal bleeding.\n");
												Sleep(250);
												TypeText(25, "You fall down to the ground. Everything begins to turn black as you slowly bleed out on the lab floor.\n\n    ");

												EndGame();

												return;
											case 2:
												TypeText(25, "You peek up and throw the vial at the guard, managing to hit him in the face.\n");
												Sleep(250);
												TypeText(25, "He begins screaming out in pain, eventually dropping to the ground, his face almost completely melted off.\n\n");
												Sleep(250);
												
												goto case_3_1;
										}

										break;
									case 2:
										TypeText(25, "You try to run out of the room. The guard quickly aims at you and shoots you in the back.\n");
										Sleep(250);
										TypeText(25, "You fall down to the ground. Everything begins to turn black as you slowly bleed out on the lab floor.\n\n    ");

										EndGame();

										return;
									case 3:
										TypeText(25, "You stay in cover and do nothing. The guard eventually walks around and shoots you in the chest.\n");
										Sleep(250);
										TypeText(25, "You fall down to the ground. Everything begins to turn black as you slowly bleed out on the lab floor.\n\n    ");

										EndGame();

										return;
								}

								break;
							case 2:
								TypeText(25, "You charge the guard. He pulls the trigger, shooting you in the chest.\n");
								Sleep(250);
								TypeText(25, "You fall down to the ground. Everything begins to turn black as you slowly bleed out on the lab floor.\n\n    ");

								EndGame();

								return;
						}
				}

			break;
		case 2:
			TypeText(25, "You desperately yell for help. A few seconds later, a man in a white labcoat comes rushing into the room.\n");
			goto case_2_3;

			break;
		case 3:
			TypeText(25, "You do nothing. Eventually, after some time has passed, a man in a white labcoat enters the room.\n");
			
			case_2_3:
				Sleep(250);
				TypeText(25, "As he sees you awake, he begins panicking. He grabs a needle from the counter and attempts to jab it into your arm.\n\n");
				Sleep(500);

				selection = HandleGameOption(2, (char*[]){"Try to dodge", "Do nothing"}, 1, false);

				switch (selection) {
					case 1:
						TypeText(25, "You lunge forward and dodge the needle. In your haste, you manage to knock the man over, causing him to hit his head on the corner and fall unconsious.\n");
						Sleep(250);
						TypeText(25, "As you lunge forward, you fall over, managing to break the chair with your weight.\n");
						Sleep(250);
						TypeText(25, "You free yourself from the restraints and stand up.\n\n");

						goto case_2_1;

						break;
					case 2:
						TypeText(25, "You let the man jab the needle into your arm. You feel a sharp pain, and then everything goes black.\n\n    ");
						
						EndGame();

						return;
				}
	}
}

int main() {
	// Setup
	system("title Lab Escape");
	ToggleWindow(false);

	system(colorSchemes[0]);

	ShowMenu();

	return 0;
}
