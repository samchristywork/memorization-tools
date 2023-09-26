#include <iostream>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <util.h>

void clearScreen() { cout << "\033[2J"; }

void alternateScreen() { cout << "\033[?1049h\033[H"; }

void normalScreen() { cout << "\033[?1049l"; }

void setRawTerminal() {
  struct termios t;
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void resetTerminal() {
  struct termios t;
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag |= (ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void setCursorPosition(int x, int y) {
  x++;
  y++;
  cout << "\033[" << y << ";" << x << "H";
}

void invertColors() { cout << "\033[7m"; }

void resetColors() { cout << "\033[0m"; }

void makeCursorInvisible() { cout << "\033[?25l"; }

void makeCursorVisible() { cout << "\033[?25h"; }

void yellow() { cout << "\033[33m"; }

void green() { cout << "\033[32m"; }

void red() { cout << "\033[31m"; }

void grey() { cout << "\033[90m"; }

void blue() { cout << "\033[34m"; }

int getTerminalWidth() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_col;
}

vector<string> *split(string content, char delimiter) {
  vector<string> *result = new vector<string>();

  string current = "";
  for (unsigned int i = 0; i < content.length(); i++) {
    if (content[i] == delimiter) {
      result->push_back(current);
      current = "";
    } else {
      current += content[i];
    }
  }
  result->push_back(current);
  return result;
}

char *getInput(int *len) {
  char *s = new char[4];
  s[0] = 0;
  s[1] = 0;
  s[2] = 0;
  s[3] = 0;
  *len = read(STDIN_FILENO, s, 4);

  return s;
}

void gather(vector<vector<Word *> *> material, vector<string> *lines) {
  for (unsigned int i = 0; i < lines->size(); i++) {
    string l = lines->at(i);
    vector<string> *parts = split(l, ' ');

    vector<Word *> *line = new vector<Word *>();

    int len = 0;
    for (unsigned int j = 0; j < parts->size(); j++) {
      Word *word = new Word();
      word->content = parts->at(j);
      word->heldBack = false;
      word->correct = true;

      len += parts->at(j).length() + 1;
      if (len > 80) {
        material.push_back(line);
        line = new vector<Word *>();
        len = parts->at(j).length() + 1;
      }

      line->push_back(word);
    }

    material.push_back(line);
  }
}
