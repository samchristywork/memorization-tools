#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <util.h>
#include <vector>

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

bool render() {
  if (!findNextWord()) {
    return false;
  }

  makeCursorInvisible();

  setCursorPosition(0, 0);
  cout << guess << " ";

  for (unsigned int i = 0; i < material.size(); i++) {
    setCursorPosition(0, i + 1);

    cout << i << " \t";

    for (unsigned int j = 0; j < material[i]->size(); j++) {
      if (!material[i]->at(j)->heldBack) {
        cout << material[i]->at(j)->content << " ";
      } else if (material[i]->at(j)->show) {
        cout << material[i]->at(j)->content << " ";
      } else {
        for (unsigned int k = 0; k < material[i]->at(j)->content.length();
             k++) {
          cout << "_";
        }
        cout << " ";
      }
    }
  }

  makeCursorVisible();

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
  if (guess == "?") {
    return true;
  }

  return normalizeString(guess) == normalizeString(answer);
}

void eventLoop() {
  clearScreen();
  render();

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

    free(s);
  }
}

void cloze(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " <filename>" << endl;
    return;
  }

  srand(time(NULL));

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

    for (unsigned int j = 0; j < parts.size(); j++) {
      Word *word = new Word();
      word->content = parts[j];
      word->heldBack = rand() % 100 < 3;
      line->push_back(word);
    }

    material.push_back(line);
  }
}
