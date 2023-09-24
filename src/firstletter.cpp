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

void eventLoopFirstletter() {
  clearScreen();
  renderFirstletter();

  while (true) {
    int len;
    char *s = getInput(&len);

    if (len == 1 && s[0] == 27) {
      break;
    } else if (len == 1) {
      Word *word = materialFirstletter[curLine]->at(curWord);
      word->show = true;
      word->correct = word->content[0] == s[0];

      curWord++;
      if (curWord >= materialFirstletter[curLine]->size()) {
        curWord = 0;
        curLine++;
        if (curLine >= materialFirstletter.size()) {
          break;
        }
      }
    } else {
      printf("%d %d %d %d - %d\n", s[0], s[1], s[2], s[3], len);
    }

    if (!renderFirstletter()) {
      break;
    }

    free(s);
  }
}

void firstletter(int argc, char *argv[]) {

  if (argc < 2) {
    cout << "Usage: " << argv[0] << " <filename>" << endl;
    return;
  }

  char *filename = argv[1];

  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    cout << "Could not open file: " << filename << endl;
    return;
  }

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer = (char *)malloc(size + 1);
  fread(buffer, 1, size, file);
  fclose(file);

  buffer[size] = 0;
  vector<string> lines = split(string(buffer), '\n');
}
