#include "test/Main.hpp"
#include "test/Data.hpp"
#include "test/Print.hpp"
#include "prep1/Output.hpp"
#include "prep1/prep1.hpp"

using namespace test;
using namespace std;

const double c_dYield = 0.07;

std::function<double(double)> DF(double dRate, double dInitialTime)
{
  return [dRate, dInitialTime](double dT)
  {
    return exp(-dRate * (dT - dInitialTime));
  };
}

std::function<double(double)> DF(double dRate1, double dRate2, double dInitialTime)
{
  return [dRate1, dRate2, dInitialTime](double dT)
  {
    return 0.5 * (exp(-dRate1 * (dT - dInitialTime)) + exp(-dRate2 * (dT - dInitialTime)));
  };
}

void discountNelsonSiegel()
{
  test::print("NELSON-SIEGEL DISCOUNT CURVE");

  double dLambda = 0.05;
  double dC0 = 0.02;
  double dC1 = 0.04;
  double dC2 = 0.06;
  double dInitialTime = 1.5;

  print(dC0, "c0");
  print(dC1, "c1");
  print(dC2, "c2");
  print(dLambda, "lambda");
  print(dInitialTime, "initial time", true);

  std::function<double(double)> uDiscount =
      vega::discountNelsonSiegel(dC0, dC1, dC2, dLambda, dInitialTime);
  double dInterval = 5;
  test::print(uDiscount, dInitialTime, dInterval);
}

void discountYieldLinInterp()
{
  test::print("DISCOUNT CURVE BY LINEAR INTERPOLATION OF YIELDS");

  double dInitialTime = 1.;

  auto uDF = test::getDiscount(dInitialTime);
  double dR = (1 / uDF.second.front() - 1.) / (uDF.first.front() - dInitialTime);
  test::print(dR, "initial short-term rate", true);

  auto uDiscount =
      vega::discountYieldLinInterp(uDF.first, uDF.second, dR, dInitialTime);

  double dInterval = uDF.first.back() - dInitialTime;
  test::print(uDiscount, dInitialTime, dInterval);
}

void forwardCashFlow()
{
  test::print("FORWARD PRICES FOR A CASH FLOW");

  double dRate = c_dYield;
  double dInitialTime = 1.;
  unsigned iPayments = 6;
  std::vector<double> uPayments(iPayments);
  uPayments.front() = 100.;
  std::transform(uPayments.begin(), uPayments.end() - 1, uPayments.begin() + 1,
                 [](double dX)
                 {
                   return dX * (1. + 0.5 * std::cos(dX));
                 });
  std::vector<double> uTimes =
      getTimes(dInitialTime + 0.5, dInitialTime + 3., iPayments);
  std::function<double(double)> uDiscount = DF(dRate, dInitialTime);
  test::print(dRate, "interest rate");
  test::print(dInitialTime, "initial time", true);
  test::print("cash flow:", uTimes, uPayments);
  std::function<double(double)> uForwardCashFlow =
      vega::forwardCashFlow(uPayments, uTimes, uDiscount);
  double dInterval = (uTimes.back() - dInitialTime) / 1.01;
  test::print(uForwardCashFlow, dInitialTime, dInterval);
}

void forwardCouponBond()
{
  test::print("FORWARD PRICES FOR A COUPON BOND");

  CashFlow uBond = swapParameters();
  uBond.notional = 1.;
  double dRate = uBond.rate;
  double dInitialTime = 1.;
  std::function<double(double)> uDiscount = [dRate, dInitialTime](double dT)
  {
    return exp(-dRate * (dT - dInitialTime));
  };
  test::print(dRate, "interest rate");
  test::print(dInitialTime, "initial time", true);

  test::printCashFlow(uBond, "bond parameters");

  for (int iI = 0; iI < 2; iI++)
  {
    bool bClean = (iI == 0) ? true : false;
    if (bClean)
    {
      print("clean prices:");
    }
    else
    {
      print("dirty prices:");
    }
    double dRate = uBond.rate;
    double dPeriod = uBond.period;
    double dMaturity = dInitialTime + dPeriod * uBond.numberOfPayments;
    std::function<double(double)> uForwardCouponBond =
        vega::forwardCouponBond(dRate, dPeriod, dMaturity, uDiscount,
                                bClean);
    double dInterval =
        uBond.period * uBond.numberOfPayments / 1.1;
    test::print(uForwardCouponBond, dInitialTime, dInterval);
  }
}

void forwardFXSimple()
{
  test::print("SIMPLE FORWARD FX CALCULATOR");

  double dSpotFX = 100;
  double dDomDF = 0.95;
  double dForDF = 0.92;

  print(dSpotFX, "spot FX rate");
  print(dDomDF, "domestic discount factor");
  print(dForDF, "foreign discount factor", true);

  auto uFX = vega::forwardFX(dSpotFX);
  double dFX = uFX(dDomDF, dForDF);
  print(dFX, "forward FX rate", true);
}

void yieldSimple()
{
  test::print("SIMPLE YIELD CALCULATOR");

  double dYield = 0.07;
  double dInitialTime = 2.;
  double dMaturity = dInitialTime + 1.5;
  double dDF = exp(-dYield * (dMaturity - dInitialTime));

  auto uYield = vega::yield(dInitialTime);
  dYield = uYield(dMaturity, dDF);

  print(dInitialTime, "initial time");
  print(dMaturity, "maturity");
  print(dDF, "discount factor", true);
  print(dYield, "yield", true);
}

void yield()
{
  test::print("CONSTRUCTION OF YIELD CURVE FROM DISCOUNT CURVE");

  double dYield = 0.07;
  double dInitialTime = 2.;

  print(dInitialTime, "initial time");
  print(dYield, "interest rate", true);

  std::function<double(double)> uDiscount = DF(dYield, dInitialTime);
  std::function<double(double)> uYield = vega::yield(uDiscount, dInitialTime);
  double dInterval = 4.75;
  test::print(uYield, dInitialTime + 0.001, dInterval);
}

void yieldNelsonSiegel()
{
  test::print("NELSON-SIEGEL YIELD CURVE");

  double dLambda = 0.05;
  double dC0 = 0.02;
  double dC1 = 0.04;
  double dC2 = 0.06;
  double dInitialTime = 1.5;

  print(dC0, "c0");
  print(dC1, "c1");
  print(dC2, "c2");
  print(dLambda, "lambda");
  print(dInitialTime, "initial time", true);

  std::function<double(double)> uYield =
      vega::yieldNelsonSiegel(dC0, dC1, dC2, dLambda, dInitialTime);
  double dInterval = 5;
  test::print(uYield, dInitialTime, dInterval);
}

void yieldShape1()
{
  test::print("YIELD SHAPE 1");

  double dLambda = 0.05;
  double dInitialTime = 2.;

  print(dLambda, "lambda");
  print(dInitialTime, "initial time", true);
  std::function<double(double)> uYield = vega::yieldShape1(dLambda, dInitialTime);
  double dInterval = 4.75;
  test::print(uYield, dInitialTime + 0.001, dInterval);
}

void yieldShape2()
{
  test::print("YIELD SHAPE 2");

  double dLambda = 0.05;
  double dInitialTime = 2.;

  print(dLambda, "lambda");
  print(dInitialTime, "initial time", true);
  std::function<double(double)> uYield = vega::yieldShape2(dLambda, dInitialTime);
  double dInterval = 4.75;
  test::print(uYield, dInitialTime + 0.001, dInterval);
}

std::function<void()> test_prep1()
{
  return []()
  {
    print("DATA CURVES FOR FINANCIAL MODELS");

    yieldSimple();
    yield();
    yieldShape1();
    yieldShape2();
    yieldNelsonSiegel();
    discountNelsonSiegel();
    discountYieldLinInterp();
    forwardFXSimple();
    forwardCashFlow();
    forwardCouponBond();
  };
}

int main()
{
  project(test_prep1(), PROJECT_NAME, PROJECT_NAME,
          "Set 1");
}
