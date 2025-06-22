#pragma once

#include <chrono>

namespace ccl::sys::time
{
    void getNtpTime( double& sec, double& usec );
}