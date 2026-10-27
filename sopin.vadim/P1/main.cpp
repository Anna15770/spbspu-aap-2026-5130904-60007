#include <iostream>
#include <cstddef>

int main()
{
  int a{0};
  int prev{0};
  bool has_prev{false};
  std::size_t result{0};

  while ((std::cin >> a) && (a != 0))
  {
    if (has_prev && (a > prev))
    {
      ++result;
    }
    prev = a;
    has_prev = true;
  }
  if (!std::cin)
  {
    std::cerr << "Unexpected input\n";
    return 1;
  }
  std::cout << result << "\n";
  return 0;
}
