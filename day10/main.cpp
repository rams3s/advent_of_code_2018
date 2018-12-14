#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool has_star( int x, int y, std::vector< int > xs, std::vector< int > ys )
{
    for ( int i = 0; i < xs.size(); ++i )
    {
        if ( x == xs[i] && y == ys[i] ) return true;
    }

    return false;
}

int main()
{
    std::string filename { "../day10/input.txt" };
    std::ifstream fs { filename };

    std::string line;
    std::vector<int> xs, ys, vxs, vys;

    while ( std::getline( fs, line ) )
    {
        auto & x = xs.emplace_back();
        auto & y = ys.emplace_back();
        auto & vx = vxs.emplace_back();
        auto & vy = vys.emplace_back();
        std::sscanf( line.c_str(), "position=< %d, %d> velocity=<%d, %d>", &x, &y, &vx, &vy );
    }

    int iteration_count = 1'000'000;

    for ( int iteration_index = 1; iteration_index < iteration_count; ++iteration_index )
    {
        for ( int star_index = 0; star_index < xs.size(); ++star_index )
        {
            xs[ star_index ] += vxs[ star_index ];
            ys[ star_index ] += vys[ star_index ];
        }

        auto [xmin,xmax] = std::minmax_element( xs.cbegin(), xs.cend() );
        auto [ymin,ymax] = std::minmax_element( ys.cbegin(), ys.cend() );

        constexpr int letter_height = 10; // found this value after solving the puzzle

        if ( *ymax - *ymin + 1 == letter_height )
        {
            std::cout << iteration_index << std::endl;

            for ( int j = *ymin; j <= *ymax; ++j )
            {
                for ( int i = *xmin; i <= *xmax; ++i )
                {
                    std::cout << ( has_star(i,j,xs,ys) ? "#" : "." );
                }

                std::cout << std::endl;
            }
        }
    }

    return 0;
}
