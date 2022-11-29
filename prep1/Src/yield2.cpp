#include "prep1/prep1.hpp"

std::function<double(double)>
vega::yield(const std::function<double(double)> &rDiscount,
            double dInitialTime) // Yield curve computed from discount curve
{
  return [rDiscount, dInitialTime](double dT)
  {
    PRECONDITION(dT >= dInitialTime);
    if (dT < dInitialTime + vega::EPS)
    {
      return (1. - rDiscount(dInitialTime + vega::EPS)) / vega::EPS;
    }
    else
    {
      return -std::log(rDiscount(dT)) / (dT - dInitialTime);
    }
  };
}