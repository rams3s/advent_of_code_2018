#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct rect
{
    uint16_t x, y, width, height;
};

int main()
{
    std::string filename { "../day3/input.txt" };
    std::ifstream stream { filename };

    std::string line;
    std::map<uint16_t, rect> m;

    while ( std::getline( stream, line ) )
    {
        uint16_t id;
        rect r;
        std::sscanf( line.c_str(), "#%hu @ %hu,%hu: %hux%hu", &id, &r.x, &r.y, &r.width, &r.height );

        m[ id ] = r;
    }

    std::vector<uint16_t> fabric( 1000 * 1000 );

    for ( auto [id, r] : m )
    {
        for ( auto i = r.x; i < r.x + r.width; ++i )
        {
            for ( auto j = r.y; j < r.y + r.height; ++j )
            {
                ++fabric[ j * 1000 + i ];
            }
        }
    }

    int total = 0;

    for ( auto i : fabric )
    {
        if ( i >= 2 ) ++total;
    }

    std::cout << "Part 1: " << total << std::endl;

    for ( auto [id, r] : m )
    {
        bool ok = true;

        for ( auto i = r.x; i < r.x + r.width; ++i )
        {
            for ( auto j = r.y; j < r.y + r.height; ++j )
            {
                if ( fabric[ j * 1000 + i ] != 1 )
                {
                    ok = false;
                    break;
                }
            }

            if ( !ok ) break;
        }

        if ( ok )
            std::cout << "Part 2: " << id << std::endl;
    }

    return 0;
}
