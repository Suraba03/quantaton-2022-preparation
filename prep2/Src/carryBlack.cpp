#include "prep2/prep2.hpp"
#include "header.hpp"
// DONE

std::function<double(double)>
vega::carryBlack(double dTheta, double dLambda, double dSigma,
                 double dInitialTime) // Cost-of-carry rate curve for the Black model
{
  PRECONDITION(dLambda >= 0);
  PRECONDITION(dSigma >= 0);

  return [dTheta, dSigma, dLambda, dInitialTime](double dT)
  {
    PRECONDITION(dT >= dInitialTime);

    double dX = dLambda * (dT - dInitialTime);
    double dSigmato2over2 = std::pow(dSigma, 2) / 2;
    double dY = dTheta * shape1(dX) + dSigmato2over2 * shape1(2 * dX);
    return dY;
  };
}