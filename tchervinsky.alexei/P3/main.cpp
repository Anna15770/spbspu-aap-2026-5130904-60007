#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <string>

const int ARGC_MIN = 4;

int main(int argc, char *argv[])
{
  if (argc < ARGC_MIN)
  {
    std::cerr << "Not enough arguments" << '\n';
    return 1;
  }
  else if (argc > ARGC_MIN)
  {
    std::cerr << "Too many arguments" << '\n';
    return 1;
  }

  std::stringstream ss_num(argv[1]);
  int num{0};
  ss_num >> num;
  std::streambuf *pbuf = ss_num.rdbuf();
  const std::streamsize ss_num_avail = pbuf->in_avail();
  if (!ss_num || ss_num_avail != 0)
  {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }

  std::ifstream ifs(argv[2]);
  if (!ifs)
  {
    std::cerr << "Input file not found\n";
    return 1;
  }
  std::string input;
  if (std::getline(ifs, input))
  {
    std::cout << input << '\n';
  }
  ifs.close();

  return 0;
}
