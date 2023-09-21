#include <iostream>
#include <string>
#include <sys/ioctl.h>

using namespace std;

struct Word {
  string content;
  bool heldBack;
  bool show;
};

int currentLine = 0;
int currentWord = 0;
string guess;
string word;
vector<vector<Word *> *> material;

char *getInput(int *len) {
  char *s = new char[4];
  s[0] = 0;
  s[1] = 0;
  s[2] = 0;
  s[3] = 0;
  *len = read(STDIN_FILENO, s, 4);

  return s;
}
void cloze(int argc, char *argv[]) {
}
