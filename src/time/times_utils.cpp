#include "times_utils.hpp"

void ccl::sys::time::getNtpTime(double &sec, double &usec)
{
    using namespace std::chrono;
    
    auto t_now  = high_resolution_clock::now().time_since_epoch();
    auto t_sec  = duration_cast<seconds>(t_now).count();
    auto t_usec = duration_cast<microseconds>(t_now).count() % (int)1e6;

    sec = static_cast<double>( t_sec );
    usec = static_cast<double>( t_usec );
}