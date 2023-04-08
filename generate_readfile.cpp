//from:
//https://codereview.stackexchange.com/questions/235517/multi-threaded-high-performance-txt-file-parsing

#include <iostream>
#include <random>
#include <fstream>
#include <iomanip>


int main(int argc, char* argv[]) {

  if (argc < 2) return 1;

  auto gen = std::mt19937{std::random_device{}()};
  std::uniform_int_distribution<unsigned> dist(1'000'000, 2'000'000);

  auto file = std::ofstream{argv[1]}; // NOLINT

  for (std::size_t i = 0; i < 100'000'000; ++i) {
    file << dist(gen) << '\n';
  }

  return 0;
}