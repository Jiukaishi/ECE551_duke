#include "story.h"
int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "check input arg!\n";
    exit(EXIT_FAILURE);
  }
  std::string directory = argv[1];
  Story story(directory);
  story.parse_each_line();
  if (story.check_valid() == false) {
    throw Step2Error();
  }
  story.interact();
  return EXIT_SUCCESS;
}
