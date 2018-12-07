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

struct point
{
    uint16_t x, y;
};

int main()
{
    std::string filename { "../day6/input.txt" };
    std::ifstream fs { filename };

    std::string line;
    std::vector<point> points;
    point min { std::numeric_limits< uint16_t >::max(), std::numeric_limits< uint16_t >::max()};
    point max { std::numeric_limits< uint16_t >::min(), std::numeric_limits< uint16_t >::min()};

    while ( std::getline( fs, line ) )
    {
        uint16_t x, y;
        std::sscanf( line.c_str(), "%hu, %hu", &x, &y );
        points.push_back( { x, y } );
        min.x = std::min( min.x, x );
        min.y = std::min( min.y, y );
        max.x = std::max( max.x, x );
        max.y = std::max( max.y, y );
    }

    std::array< uint16_t, 256 > counts {};
    std::array< bool, 256 > infinites {};

    for ( auto x = min.x; x <= max.x; ++x )
    {
        for ( auto y = min.y; y <= max.y; ++y )
        {
            auto min_dist = std::numeric_limits< uint16_t >::max();
            uint8_t current_min = 255;

            for ( std::size_t i = 0; i < points.size(); ++i )
            {
                const auto & p = points[ i ];
                auto dist = static_cast< uint16_t >( std::abs( x - p.x ) + std::abs( y - p.y ) );

                if ( dist < min_dist )
                {
                    min_dist = dist;
                    current_min = static_cast< uint8_t >( i );
                }
                else if ( dist == min_dist )
                {
                    current_min = 255;
                }
            }

            if ( current_min != 255 )
            {
                if ( x == min.x || x == max.x || y == min.y || y == max.y )
                {
                    infinites[ current_min ] = true;
                }

                ++( counts[ current_min ] );
            }
        }
    }

    for ( std::size_t i = 0; i < infinites.size(); ++i )
    {
        if ( infinites[ i ] )
        {
            counts[ i ] = 0;
        }
    }

    std::cout << "Part 1: " << *std::max_element( counts.cbegin(), counts.cend() ) << std::endl;

    constexpr auto limit = 10'000;
    uint16_t within_limit_count = 0;

    for ( auto x = min.x; x <= max.x; ++x )
    {
        for ( auto y = min.y; y <= max.y; ++y )
        {
            uint16_t total_distance = 0;

            for ( std::size_t i = 0; i < points.size(); ++i )
            {
                const auto & p = points[ i ];
                total_distance += std::abs( x - p.x ) + std::abs( y - p.y );
            }

            if ( total_distance < limit )
            {
                ++within_limit_count;
            }
        }
    }

    std::cout << "Part 2: " << within_limit_count << std::endl;

    return 0;
}
