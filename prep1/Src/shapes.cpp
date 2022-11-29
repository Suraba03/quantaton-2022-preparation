#include "prep1/prep1.hpp"

double vega::shape1(double dX)
{
  PRECONDITION(dX >= 0);
  double dY = (dX > vega::EPS) ? (1 - std::exp(-dX)) / dX : (1. - dX / 2. + dX * dX / 6.);
  return dY;
}

double vega::shape2(double dX)
{
  PRECONDITION(dX >= 0);
  double dY = (dX > vega::EPS) ? (1 - std::exp(-dX) * (1. + dX)) / dX : (dX / 2. - dX * dX / 3.);
  return dY;
}
