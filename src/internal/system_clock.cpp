#include "system_clock.hpp"

using namespace internal;

SystemClock::SystemClock() : m_start(Clock::now()) {}

double SystemClock::elapsed_second()
{
    std::chrono::duration<double> elapsed = Clock::now() - m_start;
    return elapsed.count();
}
