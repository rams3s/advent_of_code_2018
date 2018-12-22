#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <stack>
#include <string>
#include <utility>
#include <vector>

std::pair< int, int > eval( const std::vector< char > & grid, int extent, int minx, int miny, int maxx, int maxy )
{
    int count_any_water = 0;
    int count_water_at_rest = 0;

    for ( int j = miny; j <= maxy; ++j )
    {
        for ( int i = minx - 1; i <= maxx + 1; ++i )
        {
            auto cell = grid[ j * extent + i ];

            std::cout << cell;

            if ( cell == '~' )
            {
                ++count_any_water;
                ++count_water_at_rest;
            }
            else if ( cell == '|' )
            {
                ++count_any_water;
            }
        }

        std::cout << std::endl;
    }

    return { count_any_water, count_water_at_rest };
}

auto process( const std::string & filename )
{
    std::ifstream fs { filename };

    std::string line;

    std::vector< char > grid;
    int extent = 2048;
    grid.resize( extent * extent, '.' );

    auto get_cell = [ & ]( int x, int y ) -> char & { return grid[ y * extent + x ]; };

    int minx = std::numeric_limits< int >::max();
    int miny = std::numeric_limits< int >::max();
    int maxx = std::numeric_limits< int >::min();
    int maxy = std::numeric_limits< int >::min();

    while ( std::getline( fs, line ) )
    {
        char a, b;
        int a1, b1, b2;
        if ( int scanned = std::sscanf( line.c_str(), "%c=%d, %c=%d..%d", &a, &a1, &b, &b1, &b2 ); scanned == 5 )
        {
            bool swap = a == 'y';

            minx = std::min( minx, swap ? b1 : a1 );
            miny = std::min( miny, swap ? a1 : b1 );
            maxx = std::max( maxx, swap ? b2 : a1 );
            maxy = std::max( maxy, swap ? a1 : b2 );

            if ( swap )
            {
                for ( int i = b1; i <= b2; ++i )
                {
                    grid[ a1 * extent + i ] = '#';
                }
            }
            else
            {
                for ( int i = b1; i <= b2; ++i )
                {
                    grid[ i * extent + a1 ] = '#';
                }
            }
        }
    }

    std::stack< std::pair< int, int>> open_list;
    open_list.emplace( 500, 0 );
    auto & initial = get_cell( 500, 0 );
    initial = '|';

    while ( !open_list.empty() )
    {
        auto[x, y] = open_list.top();
        auto & current = get_cell( x, y );

        open_list.pop();

        if ( current != '|' ) continue;

        if ( y == maxy ) continue;

        auto & below = get_cell( x, y + 1 );

        switch ( below )
        {
            case '.':
            {
                below = '|';
                open_list.emplace( x, y );
                open_list.emplace( x, y + 1 );
            } break;

            case '~':
            case '#':
            {
                if ( auto & cell = get_cell( x - 1, y ); cell == '.' )
                {
                    cell = '|';
                    open_list.emplace( x - 1, y );
                }
                else if ( cell == '#' )
                {
                    for ( int i = x; i <= maxx; ++i )
                    {
                        if ( auto & cell = get_cell( i, y ); cell != '|' )
                        {
                            if ( cell == '#' )
                            {
                                for ( int ii = x; ii < i; ++ii )
                                {
                                    get_cell( ii, y ) = '~';
                                }
                            }

                            break;
                        }
                    }
                }

                if ( auto & cell = get_cell( x + 1, y ); cell == '.' )
                {
                    cell = '|';
                    open_list.emplace( x + 1, y );
                }
                else if ( cell == '#' )
                {
                    for ( int i = x; i >= minx; --i )
                    {
                        if ( auto & cell = get_cell( i, y ); cell != '|' )
                        {
                            if ( cell == '#' )
                            {
                                for ( int ii = x; ii > i; --ii )
                                {
                                    get_cell( ii, y ) = '~';
                                }
                            }

                            break;
                        }
                    }
                }
            } break;
        }
    }

    return eval( grid, extent, minx, miny, maxx, maxy );
}

int main()
{
    auto test_count = process( "../day17/test.txt" );
    std::cout << "Part 1 Test: " << test_count.first << std::endl;
    std::cout << "Part 2 Test: " << test_count.second << std::endl;

    auto count = process( "../day17/input.txt" );
    std::cout << "Part 1 Test: " << count.first << std::endl;
    std::cout << "Part 2 Test: " << count.second << std::endl;

    return 0;
}
