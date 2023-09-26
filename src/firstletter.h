#ifndef FIRSTLETTER_H
#define FIRSTLETTER_H

#include <data.h>
#include <string>
#include <vector>

using namespace std;

class FirstLetter {
public:
  unsigned int curLine = 0;
  unsigned int curWord = 0;
  vector<vector<Word *> *> material;

  bool render();
  void eventLoop();
  FirstLetter(int argc, char *argv[]);
};

#endif
