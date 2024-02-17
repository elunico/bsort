#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>

template <class It>
std::vector<std::string> tokenize(It start, It const end, char const sep) {
  std::vector<std::string> result{};

  auto ptr = start;
  while (ptr != end) {
    if (*ptr == sep) {
      std::string s{start, ptr};
      result.push_back(s);
      start = ptr + 1;
    }
    ptr++;
  }
  result.emplace_back(start, end);
  return result;
}

bool emptyws(std::string const &s) {
  bool isempty = true;
  for (char const &c : s) {
    if (!std::isspace(c)) {
      isempty = false;
    }
  }
  return isempty;
}

struct arguments {
  enum class field_type { INT, LONG, FLOAT, DOUBLE, STRING };
  int field = -1;
};

void usage() { fprintf(stderr, "usage: ./bsort -f FIELD \n"); }

arguments getargs(int argc, char *const argv[]) {
  arguments args{};
  for (;;) {
    switch (getopt(argc, argv, "f:")) {
    case 'f':
      args.field = strtol(optarg, NULL, 10);
      break;
    case 'h':
      usage();
      goto done;
    case '?':
      exit(1);
    case -1:
      goto done;
    }
  }
done:
  if (args.field < 0) {
    usage();
    exit(1);
  }
  return args;
}

std::optional<double> tryNumeric(std::string const &s) {
  double d = strtod(s.c_str(), nullptr);
  std::stringstream ss;
  ss << d;
  auto t = ss.str();
  if (t.size() != s.size())
    return std::nullopt;
  return d;
}

int main(int argc, char *const argv[]) {
  arguments const args = getargs(argc, argv);
  int const field = args.field;

  std::vector<std::string> lines{};

  while (true) {
    std::string s{};
    std::getline(std::cin, s);
    if (std::cin.eof()) {
      break;
    }
    if (!emptyws(s)) {
      lines.push_back(s);
    }
  }

  std::vector<std::vector<std::string>> tokens{};

  tokens.reserve(lines.size());
  for (auto &line : lines) {
    tokens.emplace_back(tokenize(line.begin(), line.end(), ' '));
  }

  std::sort(tokens.begin(), tokens.end(),
            [&field](std::vector<std::string> &l, std::vector<std::string> &r) {
              auto n = tryNumeric(l[field]);
              auto m = tryNumeric(r[field]);
              if (n.has_value() && m.has_value()) {
                return *n < *m;
              } else {
                return l[field] < r[field];
              }
            });

  for (auto const &token : tokens) {
    for (auto const &tok : token) {
      std::cout << tok << " ";
    }
    std::cout << std::endl;
  }
}
