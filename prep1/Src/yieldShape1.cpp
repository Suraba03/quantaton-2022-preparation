#include "prep1/prep1.hpp"
std::function<double(double)>
vega::yieldShape1(double dLambda, double dInitialTime) // Yield shape curve 1
{
    return [dLambda, dInitialTime](double dT)
    {
        PRECONDITION(dT >= dInitialTime);

        double dX = dLambda * (dT - dInitialTime);
        return shape1(dX);
    };
}