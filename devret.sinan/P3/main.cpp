
#include <iostream>
#include <fstream>
#include <cstddef>
#include <limits>
#include <new>

namespace devret
{
    const std::size_t MAX_SIZE = 10000;

    int process(std::ifstream& in, const char* outputName,
        int rows, int cols, std::size_t total, int* matrix)
    {
        for (std::size_t i = 0; i < total; ++i)
        {
            if (!(in >> matrix[i]))
            {
                std::cerr << "Error: invalid input matrix\n";
                return 2;
            }
        }

        char extra;
        if (in >> extra || !in.eof())
        {
            std::cerr << "Error: extra or invalid input data\n";
            return 2;
        }

        int count = 0;
        for (int i = 0; i < rows; ++i)
        {
            bool valid = true;
            std::size_t start =
                static_cast<std::size_t>(i) * cols;

            for (int j = 1; j < cols; ++j)
            {
                if (matrix[start + j] == matrix[start + j - 1])
                {
                    valid = false;
                    break;
                }
            }

            if (valid)
            {
                ++count;
            }
        }

        std::ofstream out(outputName);
        if (!out)
        {
            std::cerr << "Error: cannot open output file\n";
            return 2;
        }

        out << count << '\n';
        out.close();

        if (!out)
        {
            std::cerr << "Error: cannot write output file\n";
            return 2;
        }

        return 0;
    }
}

int main(int argc, char* argv[])
{
    if (argc != 4 || argv[1][1] != '\0' ||
        (argv[1][0] != '1' && argv[1][0] != '2'))
    {
        std::cerr << "Error: usage: lab <1|2> <input> <output>\n";
        return 1;
    }

    int mode = argv[1][0] - '0';
    std::ifstream in(argv[2]);

    if (!in)
    {
        std::cerr << "Error: cannot open input file\n";
        return 2;
    }

    int rows, cols;
    if (!(in >> rows >> cols) || rows < 0 || cols < 0)
    {
        std::cerr << "Error: invalid input dimensions\n";
        return 2;
    }

    std::size_t r = static_cast<std::size_t>(rows);
    std::size_t c = static_cast<std::size_t>(cols);
    std::size_t max = std::numeric_limits<std::size_t>::max();

    if (c != 0 && r > max / c)
    {
        std::cerr << "Error: matrix is too large\n";
        return 2;
    }

    std::size_t total = r * c;

    if (total > max / sizeof(int) ||
        (mode == 1 && total > devret::MAX_SIZE))
    {
        std::cerr << "Error: matrix is too large\n";
        return 2;
    }

    if (mode == 1)
    {
        int matrix[devret::MAX_SIZE];
        return devret::process(in, argv[3], rows, cols, total, matrix);
    }

    int* matrix = total == 0 ? nullptr : new (std::nothrow) int[total];

    if (total != 0 && matrix == nullptr)
    {
        std::cerr << "Error: memory allocation failed\n";
        return 2;
    }

    int result = devret::process(in, argv[3], rows, cols, total, matrix);
    delete[] matrix;
    return result;
}
