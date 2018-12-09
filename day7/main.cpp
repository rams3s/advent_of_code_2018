#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using step_id_t = uint8_t;

struct step_t
{
    std::vector< step_id_t > children;
    std::vector< step_id_t > parents;
    uint32_t remaining_time;
    uint32_t padding;
};

bool can_proceed( const step_t step, const std::array< bool, 26 > & is_processed_table )
{
    return std::all_of( step.parents.cbegin(), step.parents.cend(), [ &is_processed_table ]( auto p ) { return is_processed_table[ p ]; } );
}

uint32_t process(
    std::array< step_t, 26 > & steps,
    const uint8_t worker_count,
    const uint8_t delay
)
{
    uint32_t duration = 0;
    std::array< bool, 26 > is_processed_table {};
    std::vector< step_id_t > ready;
    std::vector< step_id_t > active;

    auto root = std::find_if( steps.cbegin(), steps.cend(), []( auto & s ) { return s.parents.empty() && !s.children.empty(); } );
    auto root_id = static_cast< step_id_t >( std::distance( steps.cbegin(), root ) );
    active.push_back( root_id );

    for ( uint8_t i = 0; i < steps.size(); ++i )
    {
        steps[ i ].remaining_time = delay + i + 1;
    }

    while ( !ready.empty() || !active.empty() )
    {
        uint8_t finished_count = 0;

        std::sort( active.begin(), active.end(), [ &steps ]( auto & a, auto & b ) { return steps[ a ].remaining_time > steps[ b ].remaining_time; } );
        auto delta = steps[ active.back() ].remaining_time;
        duration += delta;

        for ( auto active_step_id : active )
        {
            step_t & active_step = steps[ active_step_id ];

            active_step.remaining_time -= delta;
            assert( active_step.remaining_time >= 0 );

            if ( active_step.remaining_time <= 0 )
            {
                std::cout << char( active_step_id + 'A' );
                is_processed_table[ active_step_id ] = true;
                ++finished_count;

                for ( auto it = active_step.children.crbegin(); it != active_step.children.crend(); ++it )
                {
                    if ( can_proceed( steps[ *it ], is_processed_table ) )
                    {
                        ready.push_back( *it );
                    }
                }
            }
        }

        sort( ready.begin(), ready.end(), std::greater< step_id_t >() );
        active.resize( active.size() - finished_count );

        while ( !ready.empty() && active.size() < worker_count )
        {
            auto step = ready.back();
            ready.pop_back();

            if ( is_processed_table[ step ] ) { continue; }

            active.push_back( step );
        }
    }

    return duration;
}

int main()
{
    std::string filename { "../day7/input.txt" };
    std::ifstream fs { filename };

    std::string line;
    std::array< step_t, 26 > steps {};

    while ( std::getline( fs, line ) )
    {
        char a, b;
        std::sscanf( line.c_str(), "Step %c must be finished before step %c can begin.", &a, &b );

        auto parent = static_cast< step_id_t >( a - 'A' );
        auto child = static_cast< step_id_t >( b - 'A' );
        steps[ parent ].children.push_back( child );
        steps[ child ].parents.push_back( parent );
    }

    std::cout << "Part 1: ";
    auto duration = process( steps, 1, 0 );
    std::cout << std::endl << duration << std::endl;

    std::cout << "Part 2: ";
    duration = process( steps, 5, 60 );
    std::cout << std::endl << duration << std::endl;

    return 0;
}
