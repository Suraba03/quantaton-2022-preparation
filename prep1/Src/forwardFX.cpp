#include "prep1/prep1.hpp"

std::function<double(double, double)>
vega::forwardFX(double dSpotFX) // Forward exchange rate from domestic and foreign discount factors
{
  return [dSpotFX](double dDom, double dFor)
  {
    PRECONDITION(dDom > vega::EPS);
    return dSpotFX * dFor / dDom;
  };
}