#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main()
{
    std::ifstream fs { "../day12/input.txt" };

    constexpr int generation_count = 5000;

    std::string line;
    std::getline( fs, line );

    std::array< std::string, 2 > pots_array;
    pots_array[0].reserve( generation_count * 2 * 2 + line.size() );
    pots_array[0].append( generation_count * 2, '.' );
    pots_array[0].append( line.substr( 15 ) );
    pots_array[0].append( generation_count * 2, '.' );
    pots_array[1].append( pots_array[0].size(), '.' );

    std::vector< std::string > rules;
    rules.reserve( 32 );

    while ( std::getline( fs, line ) )
    {
        char pattern[6];
        char c;

        if ( int scanned = std::sscanf( line.c_str(), "%s => %c ", pattern, &c ); scanned == 2 )
        {
            if ( c == '#' )
            {
                rules.emplace_back( pattern );
            }
        }
    }

    int current_pots_index = 0;

    for ( int generation_index = 0; generation_index < generation_count; ++generation_index )
    {
        std::string & current = pots_array[current_pots_index];
        std::string & destination = pots_array[current_pots_index ^ 1];

        for ( int i = 0; i < current.size() - 5 + 1; ++i )
        {
            if ( std::find( rules.cbegin(), rules.cend(), current.substr( i, 5 ) ) != rules.cend() )
            {
                destination[i+2] = '#';
            }
            else
            {
                destination[i+2] = '.';
            }
        }

        current_pots_index ^= 1;

        if ( generation_index == 19 || generation_index % 1000 == 999 )
        {
            int sum = 0;
            const auto & pots = pots_array[current_pots_index];

            for ( int i = 0; i < pots.size(); ++i )
            {
                if ( pots[i] == '#' )
                {
                    sum += i - 2 * generation_count;
                }
            }

            std::cout << generation_index << ": " << sum << std::endl;
        }
    }

    // from the logs above, we see that sum increases by 73'000 every 1000 generations

    unsigned long long part2 = ( 50'000'000 - 1 );
    part2 *= 73'000;
    part2 += 74776;

    std::cout << "Part 2: " <<  part2  << std::endl;

    return 0;
}
