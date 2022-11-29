#include "prep1/prep1.hpp"

// DONE

std::function<double(double, double)>
vega::yield(double dInitialTime) // Yield from maturity and discount factor
{
    return [dInitialTime](double T, double dT)
    {
        PRECONDITION(T > dInitialTime + vega::EPS);
        return -std::log(dT) / (T - dInitialTime);
    };
}