#include <cloze.h>
#include <firstletter.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " <tool> <args>" << endl;
    return EXIT_FAILURE;
  }

  string toolName = argv[1];

  if (toolName == "cloze") {
    Cloze(argc - 1, argv + 1);
  } else if (toolName == "firstletter") {
    FirstLetter(argc - 1, argv + 1);
  } else {
    cout << "Unknown tool: " << argv[1] << endl;
    return EXIT_FAILURE;
  }
}
