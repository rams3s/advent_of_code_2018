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

struct raw_entry
{
    uint16_t guard_id;
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t step;
    uint8_t _padding;
};

struct guard_entry
{
    std::vector< std::pair< uint16_t, uint8_t > > logs;
    uint16_t duration;
    uint8_t _padding[6];
};

int main()
{
    std::string filename { "../day4/input.txt" };
    std::ifstream fs { filename };

    std::string line;
    std::vector< raw_entry > raw_entries;
    raw_entries.reserve( 1024 );

    while ( std::getline( fs, line ) )
    {
        raw_entry entry;
        char action[256];

        std::sscanf( line.c_str(), "[%hu-%hhu-%hhu %hhu:%hhu] %[^\n]", &entry.year, &entry.month, &entry.day,
                     &entry.hour, &entry.minute, action );

        switch ( action[ 0 ] )
        {
            case 'G':
            {
                std::sscanf( action, "Guard #%hu begins shift", &entry.guard_id );
                entry.step = 0;
            }
                break;
            case 'f':
            {
                entry.step = 1;
            }
                break;
            case 'w':
            {
                entry.step = 2;
            }
                break;
        }

        raw_entries.push_back( entry );
    }

    std::sort( raw_entries.begin(), raw_entries.end(),
               []( const raw_entry & e1, const raw_entry & e2 )
               {
                   uint64_t id1 =
                           ( ( ( ( e1.year * 100 ) + e1.month ) * 100 + e1.day ) * 100 + e1.hour ) * 100 + e1.minute;
                   uint64_t id2 =
                           ( ( ( ( e2.year * 100 ) + e2.month ) * 100 + e2.day ) * 100 + e2.hour ) * 100 + e2.minute;

                   return id1 < id2;
               } );

    std::map< uint16_t, guard_entry > guard_entries;
    uint16_t current_guard_id = 0;
    uint8_t start = 0;

    for ( auto & entry : raw_entries )
    {
        switch ( entry.step )
        {
            case 0:
            {
                current_guard_id = entry.guard_id;
            }
                break;

            case 1:
            {
                start = entry.minute;
            }
                break;

            case 2:
            {
                auto & guard_entry = guard_entries[ current_guard_id ];
                guard_entry.logs.push_back( { start, entry.minute } );
                guard_entry.duration += entry.minute - start;
            }
                break;
        }
    }

    std::map< uint16_t, std::array< uint16_t, 60 > > per_minute_map;

    for ( auto[guard_id, guard_log] : guard_entries )
    {
        std::array< uint16_t, 60 > per_minute_stats {};

        for ( auto & sleep_log : guard_log.logs )
        {
            for ( std::size_t i = sleep_log.first; i < sleep_log.second; ++i )
            {
                ++per_minute_stats[ i ];
            }
        }

        per_minute_map[ guard_id ] = per_minute_stats;
    }

    auto[guard_id_1, guard_log] = *std::max_element( guard_entries.cbegin(), guard_entries.cend(),
                                                     []( auto & l1, auto & l2 )
                                                     {
                                                         return l1.second.duration < l2.second.duration;
                                                     } );

    auto & per_minute_stats = per_minute_map[ guard_id_1 ];
    auto minute_1 = std::distance( per_minute_stats.cbegin(),
                                   std::max_element( per_minute_stats.cbegin(), per_minute_stats.cend() ) );

    std::cout << "Part 1: " << guard_id_1 * minute_1 << std::endl;

    uint16_t max = 0;
    uint8_t minute_2 = 99;
    uint16_t guard_id_2 = 0;

    for ( auto[guard_id, arr] : per_minute_map )
    {
        auto current = std::max_element( arr.cbegin(), arr.cend() );

        if ( *current > max )
        {
            max = *current;
            minute_2 = static_cast<uint8_t >( std::distance( arr.cbegin(), current ) );
            guard_id_2 = guard_id;
        }
    }

    std::cout << "Part 2: " << guard_id_2 * minute_2 << std::endl;

    return 0;
}
