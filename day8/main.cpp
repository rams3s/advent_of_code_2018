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
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

struct node_t
{
    std::vector< std::size_t > children;
    std::vector< std::size_t > metadata;
};

std::size_t parse_node( std::istream & s, std::vector< node_t > & nodes )
{
    auto node_index = nodes.size();
    nodes.emplace_back();

    uint32_t child_count, metadata_count;
    s >> child_count >> metadata_count;

    for ( std::size_t i = 0; i < child_count; ++i )
    {
        auto child_node_index = parse_node( s, nodes );
        nodes[ node_index ].children.emplace_back( child_node_index );
    }

    auto & node = nodes[ node_index ];

    for ( std::size_t i = 0; i < metadata_count; ++i )
    {
        uint32_t metadata;
        s >> metadata;

        node.metadata.push_back( metadata );
    }

    return node_index;
}

int main()
{
    std::string filename { "../day8/input.txt" };
    std::ifstream fs { filename };

    std::vector<node_t> nodes;

    parse_node( fs, nodes );

    std::cout << "Part 1: " << std::accumulate(
            nodes.cbegin(),
            nodes.cend(),
            0,
            [] (auto sum, auto & node)
            {
                return std::accumulate( node.metadata.cbegin(), node.metadata.cend(), sum );
            } ) << std::endl;

    std::vector<uint32_t> node_values( nodes.size() );

    for ( int i = nodes.size() - 1; i >= 0; --i )
    {
        const auto & node = nodes[ i ];
        auto & value = node_values[ i ];

        if ( node.children.empty() )
        {
            value = std::accumulate( node.metadata.cbegin(), node.metadata.cend(), 0 );
        }
        else
        {
            value = std::accumulate( node.metadata.cbegin(), node.metadata.cend(), 0,
                [&node, &node_values] ( auto sum, auto metadata )
                {
                    --metadata;
                    if ( metadata < node.children.size() )
                    {
                        sum += node_values[ node.children[ metadata ] ];
                    }
                    return sum;
                } );
        }
    }

    std::cout << "Part 2: " << node_values[ 0 ] << std::endl;

    return 0;
}
