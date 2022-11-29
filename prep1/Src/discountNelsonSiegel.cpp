#include "prep1/prep1.hpp"

// DONE

std::function<double(double)>
vega::discountNelsonSiegel(double dC0, double dC1, double dC2,
                           double dLambda, double dInitialTime) // The Nelson-Siegel discount curve
{
    PRECONDITION(dLambda >= 0);

    std::function<double(double)> uYield =
        yieldNelsonSiegel(dC0, dC1, dC2, dLambda, dInitialTime);
    return [uYield, dInitialTime](double dT)
    {
        PRECONDITION(dT >= dInitialTime);
        return std::exp(-uYield(dT) * (dT - dInitialTime));
    };
}