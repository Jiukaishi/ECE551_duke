#include<iostream>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <istream>
#include <vector>

void doSort(std::istream & is) {
  std::string line;
  std::vector<std::string> lines;

  while (getline(is, line)) {
    lines.push_back(line);
  }
  if (!is.eof()) {
    std::cerr << "failed to read file" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::sort(lines.begin(), lines.end());

  /* the first clause is so awkward */
  for (auto it = lines.begin();it != lines.end();++it) {
    std::cout << *it << std::endl;
  }
}

int main(int argc, char ** argv) {
  if (argc < 2) {
    doSort(std::cin);
  }
  else {
    for (int i = 1; i < argc; i++) {
      std::ifstream ifs(argv[i]);
      if (!ifs) {
        std::cerr << "failed to open " << argv[i] << std::endl;
        exit(EXIT_FAILURE);
      }
      doSort(ifs);
    }
  }
  return 0;
}

// vim: ts=2:sw=2:et
