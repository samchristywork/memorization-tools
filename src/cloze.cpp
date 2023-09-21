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
string answer;
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

bool findNextWord() {
  for (unsigned int line = 0; line < material.size(); line++) {
    for (unsigned int word = 0; word < material[line]->size(); word++) {
      if (material[line]->at(word)->heldBack == true &&
          material[line]->at(word)->show == false) {
        currentLine = line;
        currentWord = word;
        answer = material[line]->at(word)->content;
        return true;
      }
    }
  }

  return false;
}

void cloze(int argc, char *argv[]) {
}
