#include <iostream>

int main()
{
  long long prev = 0;
  long long current = 0;
  long long changes = 0;
  bool is_first = true;

  while (true)
  {
    if (!(std::cin >> current))
    {
      std::cerr << "ошибка\n";
      return 1;
    }

    if (current == 0)
    {
      break;
    }

    if (is_first)
    {
      prev = current;
      is_first = false;
    }
    else
    {
      if (prev > 0 && current < 0)
      {
        changes++;
      }
      else if (prev < 0 && current > 0)
      {
        changes++;
      }
      prev = current;
    }
  }

  std::cout << changes << "\n";
  return 0;
}
