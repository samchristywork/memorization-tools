#ifndef CLOZE_H
#define CLOZE_H

#include <data.h>
#include <string>
#include <vector>

using namespace std;

class Cloze {
public:
  string guess;
  string answer;
  int currentLine = 0;
  int currentWord = 0;
  vector<vector<Word *> *> material;

  void eventLoop();
  bool render();
  bool findNextWord();
  Cloze(int argc, char *argv[]);
};

#endif
