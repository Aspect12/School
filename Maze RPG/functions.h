
#pragma once

#include <string>
#include <vector>

class MapChange;
class Player;
class Chest;
class Item;

#define MAPWIDTH 82
#define MAPHEIGHT 21

// Global Variables
extern char map[MAPHEIGHT][MAPWIDTH];
extern std::vector<MapChange> pendingChanges;
extern std::vector<Chest*> chests;

// Function to register a map change
void RegisterMapChange(char c, int x, int y);

// Function to hide the cursor
void HideCursor();

// Function to print colored characters based on their value
void PrintColored(char& character);

// Function to clear specific lines from the console
void ClearLines(int startLine, int endLine);

// Function to clear the action line and print the new action
void AppendActionResult(std::string action);

// Function to draw the pending changes to the map
void DrawMap();

// Function to draw the game instructions
void DrawSideText(Player* player);

// Function to draw the Boss
void DrawBoss(Player* player, char character, int x, int y);
