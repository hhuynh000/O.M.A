#pragma once

#include <chrono>

namespace internal
{
    class SystemClock
    {
    public:
        SystemClock();
        double elapsed_second();

    private:
        using Clock = std::chrono::high_resolution_clock;
        using second = std::chrono::seconds;
        std::chrono::time_point<Clock> m_start;
    };
} // namespace internal
