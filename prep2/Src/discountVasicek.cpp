#include "prep2/prep2.hpp"
#include "header.hpp"
// DONE

std::function<double(double)>
vega::discountVasicek(double dTheta, double dLambda, double dSigma,
                      double dR0, double dInitialTime)
{
    PRECONDITION(dLambda > 0);
    PRECONDITION(dSigma > 0);

    std::function<double(double)> uYieldVasicek =
        yieldVasicek(dTheta, dLambda, dSigma, dR0, dInitialTime);
    return [uYieldVasicek, dInitialTime](double dT)
    {
        PRECONDITION(dT >= dInitialTime);
        return std::exp(-uYieldVasicek(dT) * (dT - dInitialTime));
    };
}