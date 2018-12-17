#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <tuple>
#include <vector>

constexpr int get_power( int x, int y, int serial )
{
    int rack_id = x + 10;
    int power = rack_id * y;
    power += serial;
    power *= rack_id;
    power /= 100;
    power %= 10;
    power -= 5;

    return power;
}

std::tuple< int, int, int > process( int subgrid_extent, int serial )
{
    constexpr int grid_extent = 300;
    std::vector< int > grid( grid_extent * grid_extent );

    for ( int y = 0; y < grid_extent; ++y )
    {
        for ( int x = 0; x < grid_extent; ++x )
        {
            grid[ y * grid_extent + x ] = get_power( x + 1, y + 1, serial );
        }
    }

    std::vector< int > horizontal_sum( grid_extent * grid_extent );

    for ( int y = 0; y < grid_extent; ++y )
    {
        for ( int x = 0; x < grid_extent - subgrid_extent + 1; ++x )
        {
            int index = y * grid_extent + x;
            int cell_sum = 0;

            for ( int i = index; i < index + subgrid_extent; ++i )
            {
                cell_sum += grid[ i ];
            }

            horizontal_sum[ index ] = cell_sum;
        }
    }

    std::vector< int > sum( grid_extent * grid_extent );

    for ( int y = 0; y < grid_extent - subgrid_extent + 1; ++y )
    {
        for ( int x = 0; x < grid_extent; ++x )
        {
            int index = y * grid_extent + x;
            int cell_sum = 0;

            for ( int i = 0; i < subgrid_extent; ++i )
            {
                cell_sum += horizontal_sum[ index + i * grid_extent ];
            }

            sum[ index ] = cell_sum;
        }
    }

    auto max_cell_it = std::max_element( sum.cbegin(), sum.cend() );
    auto cell_index = std::distance( sum.cbegin(), max_cell_it );
    auto cell_x = cell_index % grid_extent;
    auto cell_y = cell_index / grid_extent;

    return { *max_cell_it, cell_x + 1, cell_y + 1 };
}

int main()
{
    assert( 4 == get_power( 3, 5, 8 ) );
    assert( -5 == get_power( 122, 79, 57 ) );
    assert( 0 == get_power( 217, 196, 39 ) );
    assert( 4 == get_power( 101, 153, 71 ) );

    assert( std::make_tuple( 29, 33, 45 ) == process( 3, 18 ) );
    assert( std::make_tuple( 30, 21, 61 ) == process( 3, 42 ) );

    constexpr int serial = 5177;

    auto[value, x, y ] = process( 3, serial );
    std::cout << "Part 1: " << x << ", " << y << "; val: " << value << std::endl;

    int best_power = std::numeric_limits<int>::min();
    int best_x, best_y, best_extent;

    for ( int subgrid_extent = 1; subgrid_extent <= 300; ++subgrid_extent )
    {
        auto[value, x, y ] = process( subgrid_extent, serial );

        if ( value > best_power )
        {
            best_power = value;
            best_x = x;
            best_y = y;
            best_extent = subgrid_extent;
        }
    }

    std::cout << "Part 2: " << best_x << ", " << best_y << ", " << best_extent << "; val: " << best_power << std::endl;

    return 0;
}
