#include <iostream>
#include <numeric>
#include <sstream>
#include <unordered_set>
#include <vector>

int main()
{
    using type_t = int;

    std::ios_base::sync_with_stdio( false );
    std::cin.tie( nullptr );

    std::string line;
    std::vector<type_t> v;

    while ( std::getline( std::cin, line ) )
    {
        std::istringstream line_ss{ line };
        type_t i;
        line_ss >> i;
        v.push_back( i );
    }

    std::cout << "Part 1: " << std::accumulate( v.cbegin(), v.cend(), 0 ) << std::endl;

    std::unordered_set<type_t> encountered;
    type_t sum = 0;

    encountered.insert( sum );

    while ( true )
    {
        for ( auto val : v )
        {
            sum += val;

            if ( auto[it, inserted] = encountered.insert( sum ); !inserted )
            {
                std::cout << "Part 2: " << sum << std::endl;
                return 0;
            }
        }
    }
}
