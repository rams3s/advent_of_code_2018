#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

int main()
{
    int puzzle_input = 793031;
    int recipe_count = puzzle_input + 10;
    std::ostringstream input_ss;
    input_ss << puzzle_input;
    std::string input_str = input_ss.str();

    std::string s;
    s.reserve( recipe_count );

    s.push_back( 3 + '0');
    s.push_back( 7 + '0');

    std::size_t first_index { 0 };
    std::size_t second_index { 1 };

    while ( 1 )
    {
        char first = s[first_index] - '0';
        char second = s[second_index] - '0';
        char sum = first + second;

        if ( int recipe = sum / 10; recipe > 0 )
            s.push_back( recipe + '0' );

        // use std::basic_string::ends_with() in c++20
        if ( std::equal( s.cend() - input_str.size(), s.cend(), input_str.cbegin(), input_str.cend() ) )
        {
            break;
        }

        s.push_back( sum % 10 + '0');

        if ( std::equal( s.cend() - input_str.size(), s.cend(), input_str.cbegin(), input_str.cend() ) )
        {
            break;
        }

        first_index = ( first_index + 1 + first ) % s.size();
        second_index = ( second_index + 1 + second ) % s.size();
    }

    std::cout << "Test: " << s.substr( 9, 10 ) << std::endl;
    std::cout << "Test: " << s.substr( 5, 10 ) << std::endl;
    std::cout << "Test: " << s.substr( 18, 10 ) << std::endl;
    std::cout << "Test: " << s.substr( 2018, 10 ) << std::endl;
    std::cout << "Part 1: " << s.substr( puzzle_input, 10 ) << std::endl;

    if ( auto index = s.rfind( input_str ); index != std::string::npos )
    {
        std::cout << "Part 2: " << index;
    }

    return 0;
}

