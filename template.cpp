#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <limits>
#include <numeric>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

int main()
{
    std::string filename { "../dayX/input.txt" };
    std::ifstream fs { filename };

    using type_t = int;

    std::string line;
    std::vector<type_t> v;

    while ( std::getline( fs, line ) )
    {
        std::istringstream line_ss{ line };
        std::string token;

        while ( std::getline( line_ss, token, ' ' ) )
        {
            std::istringstream token_ss{ token };
            type_t i;
            token_ss >> i;
            v.push_back( i );
        }

        std::cout << "line: " << line << std::endl;
    }

    std::cout << "Part 1: " << 0 << std::endl;

    return 0;
}
