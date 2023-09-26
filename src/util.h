#ifndef UTIL_H
#define UTIL_H

#include <data.h>
#include <string>
#include <vector>

using namespace std;

void clearScreen();

void alternateScreen();

void normalScreen();

void setRawTerminal();

void resetTerminal();

void setCursorPosition(int x, int y);

void invertColors();

void resetColors();

void makeCursorInvisible();

void makeCursorVisible();

void yellow();

void green();

void red();

void grey();

void blue();

int getTerminalWidth();

vector<string> split(string content, char delimiter);

char *getInput(int *len);

void gather(vector<vector<Word *> *> material, vector<string> *lines);

#endif
