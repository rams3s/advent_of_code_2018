#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>

std::size_t reduce( const std::string & polymer )
{
    constexpr auto diff = 'a' - 'A';

    std::stack< char > st;

    for ( auto c : polymer )
    {
        if ( st.empty() )
        {
            st.push( c );
            continue;
        }

        if ( abs( c - st.top() ) == diff )
        {
            st.pop();
        }
        else
        {
            st.push( c );
        }
    }

    return st.size();
}

int main()
{
    std::string filename { "../day5/input.txt" };
    std::ifstream fs { filename };

    std::string original_line;

    std::getline( fs, original_line );

    auto size = reduce( original_line );

    std::cout << "Part 1: " << size << std::endl;

    auto min = size;

    for ( int i = 0; i < 26; ++i )
    {
        std::string line { original_line };
        line.erase( std::remove( line.begin(), line.end(), 'a' + i ), line.end() );
        line.erase( std::remove( line.begin(), line.end(), 'A' + i ), line.end() );

        min = std::min( min, reduce( line ) );
    }

    std::cout << "Part 2: " << min << std::endl;

    return 0;
}
