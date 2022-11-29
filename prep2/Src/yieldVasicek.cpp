#include "prep2/prep2.hpp"
#include "header.hpp"
// DONE

std::function<double(double)>
vega::yieldVasicek(double dTheta, double dLambda, double dSigma,
                   double dR0, double dInitialTime)
{
    PRECONDITION(dLambda > 0);
    PRECONDITION(dSigma > 0);

    return [dTheta, dSigma, dLambda, dInitialTime, dR0](double dT)
    {
        PRECONDITION(dT >= dInitialTime);

        double dX = dLambda * (dT - dInitialTime);
        double dSigmato2over2 = std::pow(dSigma, 2) / 2;
        double dLambdato2 = std::pow(dLambda, 2);
        double dY = dR0 * shape1(dX) + (dTheta / dLambda) * (1 - shape1(dX)) -
                    (dSigmato2over2 / dLambdato2) * (1 - 2 * shape1(dX) + shape1(2 * dX));
        return dY;
    };
}