#include <firstletter.h>
#include <iostream>
#include <util.h>

using namespace std;

bool FirstLetter::render() {
  clearScreen();
  setCursorPosition(0, 0);
  makeCursorInvisible();

  for (unsigned int i = 0; i < material.size(); i++) {
    setCursorPosition(0, i + 1);
    grey();
    cout << i;
    resetColors();

    setCursorPosition(4, i + 1);
    for (unsigned int j = 0; j < material[i]->size(); j++) {
      if (i == curLine && j == curWord) {
        blue();
      }

      if (!material[i]->at(j)->heldBack) {
        cout << material[i]->at(j)->content << " ";
      } else if (material[i]->at(j)->show) {
        if (material[i]->at(j)->correct) {
          green();
        } else {
          red();
        }
        cout << material[i]->at(j)->content << " ";
      } else {
        for (unsigned int k = 0; k < material[i]->at(j)->content.length();
             k++) {

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

void FirstLetter::eventLoop() {
  clearScreen();
  render();

  while (true) {
    int len;
    char *s = getInput(&len);

    if (len == 1 && s[0] == 27) {
      break;
    } else if (len == 1) {
      Word *word = material[curLine]->at(curWord);
      word->show = true;
      word->correct = word->content[0] == s[0];

      curWord++;
      if (curWord >= material[curLine]->size()) {
        curWord = 0;
        curLine++;
        if (curLine >= material.size()) {
          break;
        }
      }
    } else {
      printf("%d %d %d %d - %d\n", s[0], s[1], s[2], s[3], len);
    }

    if (!render()) {
      break;
    }

    free(s);
  }
}

FirstLetter::FirstLetter(int argc, char *argv[]) {
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

  for (unsigned int i = 0; i < lines.size(); i++) {
    vector<string> parts = split(lines[i], ' ');

    vector<Word *> *line = new vector<Word *>();

    int len = 0;
    for (unsigned int j = 0; j < parts.size(); j++) {
      Word *word = new Word();
      word->content = parts[j];
      word->heldBack = false;
      word->correct = true;

      len += parts[j].length() + 1;
      if (len > 80) {
        material.push_back(line);
        line = new vector<Word *>();
        len = parts[j].length() + 1;
      }

      line->push_back(word);
    }

    material.push_back(line);
  }

  int counter = 1;
  for (unsigned int i = 0; i < material.size(); i++) {
    for (unsigned int j = 0; j < material[i]->size(); j++) {
      material[i]->at(j)->heldBack = counter % 2 == 0;

      if (material[i]->at(j)->content.length() == 0) {
        material[i]->at(j)->heldBack = false;
      }

      counter++;
    }
  }

  alternateScreen();
  setRawTerminal();
  eventLoop();
  resetTerminal();
  normalScreen();
}
