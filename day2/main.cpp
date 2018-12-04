#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main()
{
    using type_t = std::string;

    std::ios_base::sync_with_stdio( false );
    std::cin.tie( nullptr );

    std::string line;
    std::vector<type_t> v;

    while ( std::getline( std::cin, line ) )
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
    }

    int twice_count = 0;
    int thrice_count = 0;

    for ( auto & s : v )
    {
        uint8_t map[26];

        memset( map, 0, sizeof( map ) );

        for ( char c : s )
        {
            ++map[ c - 'a' ];
        }

        bool found_2 = false;
        bool found_3 = false;

        for ( auto count : map )
        {
            if ( !found_2 && count == 2 )
            {
                ++twice_count;
                found_2 = true;

                if ( found_3 ) break;
            }

            if ( !found_3 && count == 3 )
            {
                ++thrice_count;
                found_3 = true;

                if ( found_2 ) break;
            }
        }
    }

    std::cout << "Part 1: " << twice_count * thrice_count << std::endl;

    for ( auto i = 0u; i < v.size(); ++i )
    {
        const auto & s1 = v[i];

        for ( auto j = i + 1u; j < v.size(); ++j )
        {
            const auto & s2 = v[j];
            uint8_t diff = 0;
            std::size_t last_diff_index = 0;

            for ( auto k = 0u; k < s1.size(); ++k )
            {
                if ( s1[k] != s2[k] )
                {
                    ++diff;
                    last_diff_index = k;
                }
            }

            if ( diff == 1 )
            {
                std::cout << "Part 2: " << s1.substr( 0, last_diff_index ) << s1.substr( last_diff_index + 1 ) << std::endl;

                return 0;
            }
        }
    }

    return 0;
}
