#include "prep1/prep1.hpp"


std::function<double(double)>
vega::yieldShape2(double dLambda, double dInitialTime) // Yield shape curve 2
{
    return [dLambda, dInitialTime](double dT)
    {
        double dX = dLambda * (dT - dInitialTime);
        return shape2(dX);
    };
}