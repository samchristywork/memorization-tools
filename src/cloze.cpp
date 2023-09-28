#include <cloze.h>
#include <data.h>
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <util.h>
#include <vector>

using namespace std;

bool Cloze::findNextWord() {
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

bool Cloze::render() {
  if (!findNextWord()) {
    return false;
  }

  makeCursorInvisible();

  setCursorPosition(0, 0);
  cout << guess << " ";

  for (unsigned int i = 0; i < material.size(); i++) {
    setCursorPosition(0, i + 1);
    grey();
    cout << i;
    resetColors();

    setCursorPosition(4, i + 1);
    for (unsigned int j = 0; j < material[i]->size(); j++) {
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

          blue();
          cout << "_";
        }
        cout << " ";
      }
      resetColors();
    }

    cout << endl;
  }

  makeCursorVisible();
  setCursorPosition(0, 0);

  return true;
}

string normalizeString(string s) {
  string result = "";
  for (unsigned int i = 0; i < s.length(); i++) {
    if (s[i] >= 'A' && s[i] <= 'Z') {
      result += s[i] - 'A' + 'a';
    } else if (s[i] >= 'a' && s[i] <= 'z') {
      result += s[i];
    } else {
    }
  }
  return result;
}

bool checkGuess(string guess, string answer) {
  return normalizeString(guess) == normalizeString(answer);
}

void Cloze::eventLoop() {
  clearScreen();
  render();
  fflush(stdout);

  while (true) {
    int len;
    char *s = getInput(&len);

    if (len == 1 && s[0] == 27) {
      break;
    } else if (len == 1 && s[0] == 127) {
      if (guess.length() > 0) {
        guess = guess.substr(0, guess.length() - 1);
      }
      clearScreen();
    } else if (len == 1 && s[0] == 10) {
      clearScreen();
      setCursorPosition(0, 0);

      if (checkGuess(guess, answer)) {
        material[currentLine]->at(currentWord)->show = true;
      } else {
        material[currentLine]->at(currentWord)->correct = false;
      }

      if (guess == "?") {
        material[currentLine]->at(currentWord)->show = true;
      }

      guess = "";
    } else if (len == 1) {
      guess += s[0];
    } else {
      printf("%d %d %d %d - %d\n", s[0], s[1], s[2], s[3], len);
    }

    if (!render()) {
      break;
    }
    fflush(stdout);

    free(s);
  }
}

Cloze::Cloze(int argc, char *argv[]) {
  if (argc < 3) {
    cout << "Usage: " << argv[0] << " <filename> <percent>" << endl;
    return;
  }

  srand(time(NULL));

  char *filename = argv[1];
  int percentHeldOut = atoi(argv[2]);

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
  vector<string> *lines = split(string(buffer), '\n');

  material = vector<vector<Word *> *>();
  gather(material, lines);

  for (unsigned int i = 0; i < material.size(); i++) {
    for (unsigned int j = 0; j < material[i]->size(); j++) {
      if (rand() % 100 < percentHeldOut) {
        material[i]->at(j)->heldBack = true;
      }

      if (material[i]->at(j)->content.length() == 0) {
        material[i]->at(j)->heldBack = false;
      }
    }
  }

  alternateScreen();
  setRawTerminal();
  eventLoop();
  resetTerminal();
  normalScreen();
}
