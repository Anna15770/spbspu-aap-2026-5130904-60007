#include <iostream>
#include <fstream>

namespace ivanova{
bool isNumber(const char* s)
{
    if (s[0] == '\0') return false;
    int i = 0;
    if (s[0] == '-' || s[0] == '+') i = 1;
    if (s[i] == '\0') return false;
    while (s[i] != '\0') {
        if (s[i] < '0' || s[i] > '9') return false;
        i++;
    }
    return true;
}

bool toNumber(const char* s, long long& result)
{
    if (!isNumber(s)) return false;
    bool minus = false;
    int i = 0;
    if (s[0] == '-') { minus = true; i = 1; }
    else if (s[0] == '+') { i = 1; }

    long long value = 0;
    while (s[i] != '\0') {
        int digit = s[i] - '0';

        if (value > (9223372036854775807LL - digit) / 10) return false;
        value = value * 10 + digit;
        i++;
    }
    result = minus ? -value : value;
    return true;
}

bool readMatrix(const char* filename, int& rows, int& cols, int* data, int maxSize)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) return false;

    const int MAX_TOKENS = 200000;
    static char* tokens[MAX_TOKENS];
    int tokenCount = 0;

    static char buffer[1 << 20];
    fin.read(buffer, sizeof(buffer) - 1);
    int n = static_cast<int>(fin.gcount());
    buffer[n] = '\0';
    fin.close();

    char* p = buffer;
    while (*p != '\0' && tokenCount < MAX_TOKENS) {
        while (*p == ' ' || *p == '\n' || *p == '\r' || *p == '\t') p++;
        if (*p == '\0') break;
        tokens[tokenCount] = p;
        tokenCount++;
        while (*p != '\0' && *p != ' ' && *p != '\n' && *p != '\r' && *p != '\t') p++;
        if (*p != '\0') { *p = '\0'; p++; }
    }

    if (tokenCount < 2) return false;

    long long r, c;
    if (!toNumber(tokens[0], r)) return false;
    if (!toNumber(tokens[1], c)) return false;
    if (r < 0 || c < 0) return false;
    if (r * c >static_cast<long long>(maxSize)) return false;

    rows = static_cast<int>(r);
    cols = static_cast<int>(c);

    int need = rows * cols;
    if (tokenCount - 2 != need) return false;

    for (int i = 0; i < need; i++) {
        long long v;
        if (!toNumber(tokens[2 + i], v)) return false;
        data[i] = static_cast<int>(v);
    }
    return true;
}

int findColumn(const int* data, int rows, int cols)
{
    if (rows <= 0 || cols <= 0) return 0;

    int bestCol = 1;
    int bestLen = 0;

    for (int j = 0; j < cols; j++) {
        int curLen = 1;
        int maxInCol = 1;
        for (int i = 1; i < rows; i++) {
            int cur = data[i * cols + j];
            int prev = data[(i - 1) * cols + j];
            if (cur == prev) {
                curLen++;
                if (curLen > maxInCol) maxInCol = curLen;
            } else {
                curLen = 1;
            }
        }
        if (maxInCol > bestLen) {
            bestLen = maxInCol;
            bestCol = j + 1;
        }
    }
    return bestCol;
}

}

int main(int argc, char* argv[])
{
    using namespace ivanova;

    if (argc != 4) {
        std::cerr << "Error: need 3 arguments\n";
        return 1;
    }

    if (argv[1][0] != '1' && argv[1][0] != '2') {
        std::cerr << "Error: first argument must be 1 or 2\n";
        return 1;
    }
    if (argv[1][1] != '\0') {
        std::cerr << "Error: first argument must be 1 or 2\n";
        return 1;
    }
    int mode = argv[1][0] - '0';

    const char* inputFile = argv[2];
    const char* outputFile = argv[3];

    int rows = 0;
    int cols = 0;

    if (mode == 1) {
        int data[10000];
        if (!readMatrix(inputFile, rows, cols, data, 10000)) {
            std::cerr << "Error: cannot read matrix\n";
            return 2;
        }
        int result = findColumn(data, rows, cols);

        std::ofstream fout(outputFile);
        fout << rows << " " << cols << " " << result << "\n";
    }
    else {
        std::ifstream probe(inputFile);
        if (!probe.is_open()) {
            std::cerr << "Error: cannot open file\n";
            return 2;
        }

        static int temp[1000000];
        if (!readMatrix(inputFile, rows, cols, temp, 1000000)) {
            std::cerr << "Error: cannot read matrix\n";
            return 2;
        }

        int total = rows * cols;
        int* data = 0;
        if (total > 0) {
            data = new int[total];
        }

        for (int i = 0; i < total; i++) {
            data[i] = temp[i];
        }

        int result = findColumn(data, rows, cols);

        std::ofstream fout(outputFile);
        fout << rows << " " << cols << " " << result << "\n";

        if (data != 0) {
            delete[] data;
        }
    }

    return 0;
}
