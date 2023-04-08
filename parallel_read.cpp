#include "flat_hash_map/bytell_hash_map.hpp"
#include "toolbelt/os/fs.hpp"
#include <cstdint>
#include <future>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using uint64 = std::uint64_t;
using map_t  = ska::bytell_hash_map<uint64, uint64>;

std::pair<const char* const, const char* const> from_sv(std::string_view sv) {
  return std::make_pair(sv.data(), sv.data() + sv.size());
}

std::string_view to_sv(const char* const begin, const char* const end) {
  return std::string_view{begin, static_cast<std::size_t>(end - begin)};
}

map_t parse(std::string_view buf) {
  auto map          = map_t{};
  auto [begin, end] = from_sv(buf);
  const char* curr  = begin;
  uint64      val   = 0;
  while (curr != end) {
    if (*curr == '\n') {
      map[val] += val;
      val = 0;
    } else {
      val = val * 10 + (*curr - '0');
    }
    ++curr; // NOLINT
  }
  return map;
}

std::vector<std::string_view> chunk(std::string_view whole, int n_chunks, char delim = '\n') {
  auto [whole_begin, whole_end] = from_sv(whole);
  auto        chunk_size        = std::ptrdiff_t{(whole_end - whole_begin) / n_chunks};
  auto        chunks            = std::vector<std::string_view>{};
  const char* end               = whole_begin;
  for (int i = 0; end != whole_end && i < n_chunks; ++i) {
    const char* begin = end;
    if (i == n_chunks - 1) {
      end = whole_end; // always ensure last chunk goes to the end
    } else {
      end = std::min(begin + chunk_size, whole_end);   // NOLINT std::min for OOB check
      while (end != whole_end && *end != delim) ++end; // NOLINT ensure we have a whole line
      if (end != whole_end) ++end;                     // NOLINT one past the end
    }
    chunks.push_back(to_sv(begin, end));
  }
  return chunks;
}

uint64 yahtzee_upper(const std::string& filename) {
  auto     mfile     = os::fs::MemoryMappedFile{filename};
  unsigned n_threads = std::thread::hardware_concurrency();
  std::cout << "Threads detected: " << n_threads << "\n";

  auto fut_maps = std::vector<std::future<map_t>>{};
  for (std::string_view chunk: chunk(mfile.get_buffer(), n_threads)) { // NOLINT
    fut_maps.push_back(std::async(std::launch::async, parse, chunk));
  }

  uint64 max_total = 0;
  auto   final_map = map_t{};
  for (auto&& fut_map: fut_maps) {
    map_t map = fut_map.get();
    for (auto pair: map) {
      uint64 total = final_map[pair.first] += pair.second;
      if (total > max_total) max_total = total;
    }
  }
  std::cout << final_map.size() << "\n";
  return max_total;
}

int main(int argc, char* argv[]) {
  if (argc < 2) return 1;
  std::cout << yahtzee_upper(argv[1]) << '\n'; // NOLINT
  return 0;
}