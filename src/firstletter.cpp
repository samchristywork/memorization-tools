#include <iostream>
#include <string>
#include <util.h>
#include <vector>

using namespace std;

struct Word {
  string content;
  bool heldBack;
  bool show;
  bool correct;
};

unsigned int curLine = 0;
unsigned int curWord = 0;
vector<vector<Word *> *> materialFirstletter;

bool renderFirstletter() {
  clearScreen();
  setCursorPosition(0, 0);
  makeCursorInvisible();

  for (unsigned int i = 0; i < materialFirstletter.size(); i++) {
    setCursorPosition(0, i + 1);
    grey();
    cout << i;
    resetColors();

    setCursorPosition(4, i + 1);
    for (unsigned int j = 0; j < materialFirstletter[i]->size(); j++) {
      if (i == curLine && j == curWord) {
        blue();
      }

      if (!materialFirstletter[i]->at(j)->heldBack) {
        cout << materialFirstletter[i]->at(j)->content << " ";
      } else if (materialFirstletter[i]->at(j)->show) {
        if (materialFirstletter[i]->at(j)->correct) {
          green();
        } else {
          red();
        }
        cout << materialFirstletter[i]->at(j)->content << " ";
      } else {
        for (unsigned int k = 0;
             k < materialFirstletter[i]->at(j)->content.length(); k++) {

          cout << "_";
        }
        cout << " ";
      }
      resetColors();
    }

    cout << endl;
  }

  fflush(stdout);
  setCursorPosition(0, 0);
  makeCursorVisible();
  return true;
}
