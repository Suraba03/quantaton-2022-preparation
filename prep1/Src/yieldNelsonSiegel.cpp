#include "prep1/prep1.hpp"

// DONE

std::function<double(double)>
vega::yieldNelsonSiegel(double dC0, double dC1, double dC2,
                        double dLambda, double dInitialTime) // The Nelson-Siegel yield curve
{
  PRECONDITION(dLambda >= 0);

  return [dC0, dC1, dC2, dLambda, dInitialTime](double dT)
  {
    PRECONDITION(dT >= dInitialTime);

    double dX = dLambda * (dT - dInitialTime);
    double dY = dC0 + dC1 * shape1(dX) + dC2 * shape2(dX);
    return dY;
  };
}