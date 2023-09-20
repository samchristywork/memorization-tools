#ifndef UTIL_H
#define UTIL_H

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

#endif
