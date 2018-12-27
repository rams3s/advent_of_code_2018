#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <utility>
#include <vector>

struct ivec2 {
    std::size_t x{}, y{};
    bool operator==(const ivec2& other ) const
    {
        return x == other.x && y == other.y;
    }
};

enum class turn_t : int8_t
{
    left = -1,
    straight,
    right,
    count
};

turn_t operator++(turn_t & turn)
{
    turn = static_cast<turn_t>( static_cast<int>(turn) + 1 );
    if ( turn == turn_t::count ) turn = turn_t::left;

    return turn;
}

enum class direction_t : uint8_t
{
    up = 0,
    right,
    down,
    left,
    count
};

struct cart_t
{
    ivec2 position{};
    direction_t direction{};
    turn_t turn{turn_t::left};

    void step( const std::vector<char> & grid, int width, int height )
    {
        switch ( direction )
        {
            case direction_t::up: --position.y; break;
            case direction_t::down: ++position.y; break;
            case direction_t::left: --position.x; break;
            case direction_t::right: ++position.x; break;
            default: assert(false);
        }

        switch ( grid[position.y*width+position.x] )
        {
            case '+':
            {
                int new_direction = static_cast<int>(direction) + static_cast<int>(turn) + static_cast<int>(direction_t::count);
                direction = static_cast< direction_t >( new_direction % static_cast<int>(direction_t::count) );
                ++turn;
            } break;

            case '/':
            {
                switch( direction )
                {
                    case direction_t::up : direction = direction_t::right; break;
                    case direction_t::right : direction = direction_t::up; break;
                    case direction_t::down : direction = direction_t::left; break;
                    case direction_t::left : direction = direction_t::down; break;
                    default: assert( false );
                }
            } break;

            case '\\':
            {
                switch( direction )
                {
                    case direction_t::up : direction = direction_t::left; break;
                    case direction_t::right : direction = direction_t::down; break;
                    case direction_t::down : direction = direction_t::right; break;
                    case direction_t::left : direction = direction_t::up; break;
                    default: assert( false );
                }
            } break;
        }
    }
};

void log( const std::vector<cart_t> & carts, const std::vector<char> & grid, const std::size_t width, const std::size_t height )
{
    for ( std::size_t j = 0; j < height; ++j )
    {
        std::cout << std::setw(3) << j << " ";

        for ( std::size_t i = 0; i < width; ++i )
        {
            if ( auto cart_it = std::find_if( carts.cbegin(), carts.cend(),
                   [i,j](const auto & c)
                   {
                       return c.position == ivec2{i,j};
                   });
                cart_it != carts.cend() )
            {
                switch (cart_it->direction)
                {
                    case direction_t::up: std::cout << "^"; break;
                    case direction_t::right: std::cout << ">"; break;
                    case direction_t::down: std::cout << "v"; break;
                    case direction_t::left: std::cout << "<"; break;
                    default: assert(false);
                }
            }
            else
            {
                std::cout << grid[ j * width + i ];
            }
        }

        std::cout << std::endl;
    }
}

int main()
{
    std::ifstream fs { "../day13/input.txt" };

    std::string line;
    std::vector<char> grid;
    std::vector<cart_t> carts;

    std::getline( fs, line );

    std::size_t height { 0 };
    std::size_t width { line.length() };

    grid.reserve( width * 256 );

    do
    {
        for ( std::size_t index = 0; index < line.size(); ++index )
        {
            char c = line[ index ];

            switch ( c )
            {
                case '<':
                {
                    carts.push_back( {{index, height}, direction_t::left} );
                    grid.push_back( '-' );
                } break;

                case '>':
                {
                    carts.push_back( {{index, height}, direction_t::right} );
                    grid.push_back( '-' );
                } break;

                case 'v':
                {
                    carts.push_back( {{index, height}, direction_t::down} );
                    grid.push_back( '|' );
                } break;

                case '^':
                {
                    carts.push_back( {{index, height}, direction_t::up} );
                    grid.push_back( '|' );
                } break;

                default:
                {
                    grid.push_back( c );
                }
            }
        }

        ++height;
    }
    while ( std::getline( fs, line ) );

    log( carts, grid, width, height );
    std::vector<std::size_t> crashed;

    while ( carts.size() > 1 )
    {
        std::sort( carts.begin(), carts.end(), [](auto a, auto b) { return a.position.y < b.position.y; } );

        for ( std::size_t cart_index = 0; cart_index <carts.size(); ++cart_index )
        {
            auto & cart = carts[cart_index];
            cart.step( grid, width, height );

            for ( std::size_t cart_index2 = 0; cart_index2 < carts.size(); ++cart_index2 )
            {
                auto & cart2 = carts[cart_index2];

                if ( &cart != &cart2 && cart.position == cart2.position )
                {
                    std::cout << "Crash: " << cart.position.x << "," << cart.position.y << std::endl;
                    crashed.push_back( cart_index );
                    crashed.push_back( cart_index2 );
                }
            }
        }

        std::sort( crashed.begin(), crashed.end(), std::greater<std::size_t>() );

        for ( auto it = crashed.cbegin(); it != crashed.cend(); ++it )
        {
            carts.erase( carts.begin() + *it );
        }

        crashed.clear();
    }

    std::cout << "Part 2: " << carts[0].position.x << "," << carts[0].position.y << std::endl;

    return 0;
}
