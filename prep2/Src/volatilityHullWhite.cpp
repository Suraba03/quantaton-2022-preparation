#include "prep2/prep2.hpp"
#include "header.hpp"

// DONE

std::function<double(double, double)>
vega::volatilityHullWhite(double dSigma, double dLambda,
                          double dInitialTime)
{
    PRECONDITION(dLambda >= 0);
    PRECONDITION(dSigma > 0);
    return [dSigma, dLambda, dInitialTime](double dS, double dT)
    {
        PRECONDITION(dS >= dInitialTime && dS < dT);

        double dX = dLambda * (dS - dInitialTime);
        double dZ = dLambda * (dT - dS);
        double dTminusdS = dT - dS;
        double dY = (dLambda != 0) ? (dSigma * ((1 - std::exp(-dZ)) / dLambda) * std::sqrt(shape1(2 * dX))) : (dSigma * (dTminusdS - std::pow(dTminusdS, 2) * dLambda / 2 + std::pow(dTminusdS, 3) * std::pow(dLambda, 2) / 6 - std::pow(dTminusdS, 4) * std::pow(dLambda, 3) / 24) * std::sqrt(shape1(2 * dX)));
        return dY;
    };
}