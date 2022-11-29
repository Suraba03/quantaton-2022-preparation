#include "prep2/prep2.hpp"
#include "header.hpp"

double shape1(double dX)
{
  PRECONDITION(dX >= 0);
  double dY = (dX > EPS) ? (1 - std::exp(-dX)) / dX : (1. - dX / 2. + dX * dX / 6.);
  return dY;
}

double shape2(double dX, double dTminusdS)
{
  PRECONDITION(dX >= 0);
  double dY = (dX > EPS) ? (1 - std::exp(-dX)) / (dX / dTminusdS) : (1. - dX / 2. + dX * dX / 6.);
  return dY;
}