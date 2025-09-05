#pragma once

#include <string>
#include <fstream>
#include <sys/resource.h>
#include <regex>
#include <unistd.h>
#include <sys/syscall.h>
#include <string_view>

namespace ccl::metrics
{
    /**
     * @brief Collects the metric specified by the input Key for the current process
     * using the /proc/self/status file on Linux and Windows specific functions
     * on the other hand.
     * 
     * @param key The specific metric to collect
     * @return The value of the metric
     */
    long get_proc_value( const std::string& key );

    /**
     * @brief Returns the current user CPU time of the calling process
     */
    long get_cpu_time();

    /**
     * @brief Returns the current stack usage, using the special 'maps' file.
     * In particular, stack pointers for the current thread is given by the file
     * /proc/self/task/<TID>/maps.
     * 
     * @param tid The current TID (Thread ID) 
     */
    long get_stack_usage( int tid );

    /**
     * @brief Get the actual heap usage in KB. It uses the /proc/self/smaps special
     * file to read all memory mappings and find the one "[heap]"-labelled. The
     * returned value is the sum of the "Private_Clean" and "Private_Dirty" fields.
     * 
     * @return The physical RAM actually used by the HEAP 
     */
    long get_heap_usage();

    /**
     * @brief Extract only the namespace/class name of the input fully
     * qualified function name (or function signature). Therefore, 
     * returning the full path.
     * 
     * @param f_name The fully qualified function name
     * @return The full path of the function
     */
    std::string qualified_function_name( std::string_view f_name );
}

#define __FUNCNAME__() ccl::metrics::qualified_function_name( __PRETTY_FUNCTION__ )