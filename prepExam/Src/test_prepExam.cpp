#include "test/Main.hpp"
#include "test/Data.hpp"
#include "test/Print.hpp"
#include "prepExam/Output.hpp"
#include "prepExam/prepExam.hpp"

using namespace test;
using namespace std;
using namespace test;

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

void costOfCarry()
{
  test::print("COST-OF-CARRY RATE");

  double dInitialTime = 0.75;
  double dMaturity = 1.25;
  double dSpot = 100;
  double dCostOfCarry = 0.07;
  double dForward = dSpot * exp(dCostOfCarry * (dMaturity - dInitialTime));

  dCostOfCarry = vega::costOfCarry(dSpot, dInitialTime)(dForward, dMaturity);

  print(dSpot, "spot");
  print(dInitialTime, "initial time");
  print(dMaturity, "maturity");
  print(dForward, "forward price", true);

  print(dCostOfCarry, "cost-of-carry", true);
}

void forwardFX()
{
  test::print("FORWARD PRICES FOR EXCHANGE RATES");

  double dSpotFX = 100;
  double dDom = 0.12;
  double dFor = 0.05;
  double dInitialTime = 1.;

  print(dInitialTime, "initial time");
  print(dSpotFX, "spot FX rate");
  print(dDom, "domestic interest rate");
  print(dFor, "foreign interest rate", true);

  std::function<double(double)> uDomestic = DF(dDom, dInitialTime);
  std::function<double(double)> uForeign = DF(dFor, dInitialTime);
  std::function<double(double)> uForwardFX = vega::forwardFX(dSpotFX, uDomestic, uForeign);
  double dInterval = 0.5;
  test::print(uForwardFX, dInitialTime, dInterval);
}

void yieldSvensson()
{
  test::print("SVENSSON YIELD CURVE");

  double dLambda1 = 0.05;
  double dLambda2 = 0.07;
  double dC0 = 0.02;
  double dC1 = 0.04;
  double dC2 = 0.06;
  double dC3 = 0.03;
  double dInitialTime = 1.5;

  print(dC0, "c0");
  print(dC1, "c1");
  print(dC2, "c2");
  print(dC3, "c3");
  print(dLambda1, "lambda 1");
  print(dLambda2, "lambda 2");
  print(dInitialTime, "initial time", true);

  std::function<double(double)> uYield =
      vega::yieldSvensson(dC0, dC1, dC2, dC3, dLambda1,
                          dLambda2, dInitialTime);
  double dInterval = 5;
  test::print(uYield, dInitialTime, dInterval);
}

void volatilityBlack()
{
  test::print("STATIONARY IMPLIED VOLATILITY IN BLACK MODEL");
  double dLambda = 0.05;
  double dSigma = 0.2;
  double dInitialTime = 0.75;

  print(dSigma, "sigma");
  print(dLambda, "lambda");
  print(dInitialTime, "initial time", true);

  std::function<double(double)> uVol = vega::volatilityBlack(dSigma, dLambda, dInitialTime);
  double dInterval = 1;
  test::print(uVol, dInitialTime, dInterval);
}

void forwardLibor()
{
  test::print("FORWARD LIBOR RATES");

  double dRate = 0.03;
  double dInitialTime = 1.5;
  double dLiborPeriod = 0.25;

  print(dLiborPeriod, "LIBOR period");
  print(dInitialTime, "initial time", true);

  auto uDiscount = DF(dRate, 4. * dRate, dInitialTime);

  std::function<double(double)> uForwardLibor =
      vega::forwardLibor(dLiborPeriod, uDiscount);

  double dInterval = 5;
  test::print(uForwardLibor, dInitialTime, dInterval);
}

void forwardCarryLinInterp()
{
  test::print("FORWARD PRICES  BY LINEAR INTERPOLATION OF COST-OF-CARRY RATES");

  double dSpot = 100;
  double dInitialTime = 1.;

  auto uF = test::getForward(dSpot, dInitialTime);

  auto uResult = vega::forwardCarryLinInterp(dSpot, uF.first,
                                             uF.second, dInitialTime);

  double dInterval = uF.first.back() - dInitialTime;
  test::print(uResult, dInitialTime, dInterval);
}

std::function<void()> test_prepExam()
{
  return []()
  {
    print("DATA CURVES FOR FINANCIAL MODELS");

    costOfCarry();
    forwardFX();
    yieldSvensson();
    volatilityBlack();
    forwardLibor();
    forwardCarryLinInterp();
  };
}

int main()
{
  project(test_prepExam(), PROJECT_NAME, PROJECT_NAME,
          "Exam for Vega-Prep");
}
