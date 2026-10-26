#include <iostream>
#include <fstream>
#include <string>

namespace mukhametdinov {

int countColsWithoutConsecutive(const int* matrix, int rows, int cols) {
    int count = 0;
    for (int j = 0; j < cols; ++j) {
        bool has_consecutive = false;
        for (int i = 0; i < rows - 1; ++i) {
            if (matrix[i * cols + j] == matrix[(i + 1) * cols + j]) {
                has_consecutive = true;
                break;
            }
        }
        if (!has_consecutive) {
            ++count;
        }
    }
    return count;
}

}

int main(int argc, char* argv[]) {
    const int argc_count = 4;
    int num = 0;
    const int max_elements = 10000;

    if (argc < argc_count) {
        std::cerr << "Not enough arguments\n";
        return 1;
    }
}
