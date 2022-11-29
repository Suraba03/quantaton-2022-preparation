#pragma once

#include <cassert>
#include <cmath>
#include <numeric>
#include <algorithm>

#define PRECONDITION assert
#define ASSERT assert
#define ASSERT assert
  
const double EPS = 1E-10;
double shape1(double dX);
double shape2(double dX, double dTminusdS);