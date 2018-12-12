#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

uint32_t process( uint16_t player_count, uint32_t last_marble_score )
{
    std::size_t current_player { 0 };
    std::vector< uint32_t > scores( player_count );
    std::list< uint32_t > marbles;
    marbles.push_back( 0 );
    auto current_marble = marbles.begin();

    for ( uint32_t marble = 1; marble <= last_marble_score; ++marble )
    {
        if ( marble % 23 == 0 )
        {
            scores[ current_player ] += marble;

            for ( int i = 0; i < 7; ++i )
            {
                if ( current_marble == marbles.begin() )
                {
                    current_marble = marbles.end();
                }

                --current_marble;
            }

            scores[ current_player ] += *current_marble;
            current_marble = marbles.erase( current_marble );
        }
        else
        {
            for ( int i = 0; i < 2; ++i )
            {
                ++current_marble;

                if ( current_marble == marbles.end() )
                {
                    current_marble = marbles.begin();
                }
            }

            current_marble = marbles.insert( current_marble, marble );
        }

        current_player = ( current_player + 1 ) % player_count;
    }

    return *std::max_element( scores.cbegin(), scores.cend() );
}

int main()
{
    std::ifstream fs { "../day9/input.txt" };

    std::string line;

    while ( std::getline( fs, line ) )
    {
        uint16_t player_count;
        uint32_t last_marble_score;

        if ( auto i = std::sscanf( line.c_str(), "%hu players; last marble is worth %u points", &player_count, &last_marble_score ); i == 2 )
        {
            std::cout << process( player_count, last_marble_score ) << std::endl;
        }
    }

    return 0;
}
