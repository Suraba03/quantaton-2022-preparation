#include "prep2/prep2.hpp"
#include "header.hpp"
// DONE

std::function<double(double)>
vega::volatilityVar(const std::function<double(double)> &rVar, double dInitialTime)

{
    return [rVar, dInitialTime](double dT)
    {
        PRECONDITION(dT >= dInitialTime);
        double dY;
        if (dT != dInitialTime)
            dY = std::sqrt(rVar(dT) / (dT - dInitialTime)); 
        else {
            double dSigma = sqrt(rVar(dInitialTime + EPS) / EPS);
            dY = dSigma;
        }
        return dY;
    };
}