#include "metrics_utils.hpp"

using namespace ccl::metrics;

long ccl::metrics::get_proc_value(const std::string &key)
{
#ifndef _WIN32

    std::ifstream file( "/proc/self/status" ); // Open the /proc/self/status file
    std::string curr_line;
    
    // Iterate for each line in the file and get the one
    // starting with the input key.
    while ( std::getline( file, curr_line ) )
    {
        if ( curr_line.rfind( key, 0 ) != 0 ) continue;

        // Gather only the numeric information associated with
        // the input key. 
        std::string value_str;
        for ( char ch : curr_line )
        {
            if ( !std::isdigit(ch) ) continue;
            value_str += ch;
        }

        // If there is any results, converts it into long and returns
        if (!value_str.empty()) return std::stol(value_str);
    }

#else

    // Windows Implementation

#endif

    return 0;
}

long ccl::metrics::get_cpu_time()
{
#ifndef _WIN32

    struct rusage usage;

    // Get the resource usage for the current process (RUSAGE_SELF).
    if (getrusage(RUSAGE_SELF, &usage) == -1) return 0;

    // Return the sum of user CPU time (ru_utime) and system CPU time (ru_stime)
    // in microseconds, which provides better resolution.
    return (usage.ru_utime.tv_sec * 1000000) + usage.ru_utime.tv_usec +
        (usage.ru_stime.tv_sec * 1000000) + usage.ru_stime.tv_usec;

#else

    // Windows Implementation

#endif
}

long ccl::metrics::get_stack_usage( int tid )
{
#ifndef _WIN32

    std::string file_name = "/proc/self/task/" + std::to_string( tid ) + "/maps";
    std::ifstream maps_file( file_name );
    std::string curr_line;
    long stack_high = 0;

    // Find the base address of the stack
    while ( std::getline( maps_file, curr_line ) )
    {
        if ( curr_line.find( "[stack]" ) != std::string::npos )
        {
            // The stack address is given in the format <start>-<end> both
            // hexadecimal numbers. We can use regex with capturing groups
            // to extract both addresses and keeps only the first one.
            std::regex reg_exp("([0-9a-f]+)-([0-9a-f]+)");
            std::smatch match;
            if (std::regex_search(curr_line, match, reg_exp) && match.size() > 1) {
                stack_high = std::stoul(match.str(2), nullptr, 16);
                break;
            }
        }
    }

    // Get the current stack pointer's address using a local variable
    char dummy_on_stack;
    long current_sp = reinterpret_cast<long>(&dummy_on_stack);

    return stack_high - current_sp;

#else

    // Windows Implementation

#endif
}

long ccl::metrics::get_heap_usage()
{
#ifndef _WIN32

    std::ifstream smaps("/proc/self/smaps");
    if (!smaps.is_open()) return 0;

    std::string curr_line;
    bool in_heap_section = false;
    long heap_kb = 0;

    while ( std::getline( smaps, curr_line ) )
    {
        // Detect the [heap] section between all the mappings
        if ( curr_line.find("[heap]") != std::string::npos )
        {
            in_heap_section = true;
            continue;
        }

        if ( in_heap_section )
        {
            if ( curr_line.empty() || curr_line[0] == ' ' ) break;
            
            // Look for Private_Clean and Private_Dirty lines
            if ( 
                   curr_line.rfind("Private_Clean:", 0) == 0 
                || curr_line.rfind("Private_Dirty:", 0) == 0 
            ) {
                // The current line is formatted as [Key] [Value] [Unit]
                std::istringstream iss( curr_line );
                std::string key, unit;
                long value;
                iss >> key >> value >> unit;
                heap_kb += value;
            }
        }
    }

    return heap_kb;

#else

    // Windows Implementation
    return 0;

#endif
}

std::string ccl::metrics::qualified_function_name(std::string_view f_name)
{
#ifndef _WIN32

    // 1. Remove the return types ( everything before first space )
    auto start = f_name.find( ' ' );
    if ( start == std::string_view::npos ) return std::string(f_name);
    f_name.remove_prefix( start + 1 );

    // 2. Remove the function signature ( input arguments )
    auto paren = f_name.find( '(' );
    if ( paren != std::string_view::npos )
    {
        f_name.remove_suffix( f_name.size() - paren );
    }

    return std::string(f_name);

#else

    // Windows Implementation
    return f_name;

#endif
}
