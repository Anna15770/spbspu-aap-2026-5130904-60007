#include <iostream>
#include <cstddef>

namespace sopin {
  constexpr int ok = 0;
  constexpr int err_input = 1;
  struct Result {
    std::size_t value;
    int status;
  };

  Result countMonDec()
  {
    int prev = 0;
    int next = 0;
    std::size_t total = 0;
    std::size_t curr_len = 0;
    std::size_t max_len = 0;

    while (std::cin >> next && next != 0) {
      ++total;
      if (total > 1 && next <= prev) {
        ++curr_len;
      } else {
        curr_len = 1;
      }
      if (curr_len > max_len) {
        max_len = curr_len;
      }
      prev = next;
    }

    if (!std::cin) {
      return {0, err_input};
    }

    return {max_len, ok};
  }
}

int main()
{
  const sopin::Result res = sopin::countMonDec();

  if (res.status == sopin::err_input) {
    std::cerr << "Invalid input\n";
    return sopin::err_input;
  }

  std::cout << res.value << "\n";
  return 0;
}
