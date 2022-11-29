#include "test/Main.hpp"
#include "test/Data.hpp"
#include "test/Print.hpp"
#include "prep2/Output.hpp"
#include "prep2/prep2.hpp"

using namespace test;
using namespace std;

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

void carryBlack()
{
  test::print("COST-OF-CARRY RATE IN BLACK MODEL");

  double dTheta = 0.03;
  double dLambda = 0.05;
  double dSigma = 0.2;
  double dInitialTime = 0.75;

  print(dTheta, "theta");
  print(dLambda, "lambda");
  print(dSigma, "sigma");
  print(dInitialTime, "initial time", true);

  std::function<double(double)>
      uCostOfCarry = vega::carryBlack(dTheta, dLambda, dSigma, dInitialTime);
  double dInterval = 1;
  test::print(uCostOfCarry, dInitialTime, dInterval);
}

void discountLogLinInterp()
{
  test::print("LOG LINEAR INTERPOLATION OF DISCOUNT CURVE");

  double dInitialTime = 1.;

  auto uDF = test::getDiscount(dInitialTime);

  auto uDiscount =
      vega::discountLogLinInterp(uDF.first, uDF.second, dInitialTime);

  double dInterval = uDF.first.back() - dInitialTime;
  test::print(uDiscount, dInitialTime, dInterval);
}

void discountVasicek()
{
  test::print("DISCOUNT CURVE IN VASICEK MODEL");

  double dLambda = 0.05;
  double dTheta = 0.02;
  double dR0 = 0.04;
  double dSigma = 0.01;
  double dInitialTime = 1.5;

  print(dTheta, "theta");
  print(dLambda, "lambda");
  print(dSigma, "sigma");
  print(dR0, "r_0");
  print(dInitialTime, "initial time", true);

  std::function<double(double)> uYield =
      vega::discountVasicek(dTheta, dLambda, dSigma, dR0, dInitialTime);
  double dInterval = 5;
  test::print(uYield, dInitialTime, dInterval);
}

void forwardAnnuity()
{
  test::print("FORWARD PRICES FOR AN ANNUITY");

  test::CashFlow uAnnuity = test::swapParameters();
  uAnnuity.notional = 1;
  double dRate = uAnnuity.rate;
  double dInitialTime = 1.;
  std::function<double(double)> uDiscount = DF(dRate, dInitialTime);

  print(dRate, "interest rate");
  print(dInitialTime, "initial time", true);

  test::printCashFlow(uAnnuity, "annuity parameters");

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
    double dRate = uAnnuity.rate;
    double dPeriod = uAnnuity.period;
    double dMaturity = dInitialTime + dPeriod * uAnnuity.numberOfPayments;
    std::function<double(double)> uForwardAnnuity =
        vega::forwardAnnuity(dRate, dPeriod, dMaturity, uDiscount,
                             bClean);
    double dInterval =
        uAnnuity.period * uAnnuity.numberOfPayments / 1.1;
    test::print(uForwardAnnuity, dInitialTime, dInterval);
  }
}

void forwardStockDividends()
{
  test::print("FORWARD PRICES FOR A STOCK WITH DIVIDENDS");

  double dSpot = 100;
  double dRate = 0.12;
  double dInitialTime = 1.;
  std::function<double(double)> uDiscount = DF(dRate, dInitialTime);
  unsigned iTimes = 10;
  std::vector<double> uTimes(iTimes);
  double dPeriod = 0.5;
  uTimes.front() = dInitialTime + dPeriod;
  std::transform(uTimes.begin(), uTimes.end() - 1, uTimes.begin() + 1,
                 [dPeriod](double dX)
                 { return dX + dPeriod; });
  double dDividend = 5.;
  double dStep = 0.5;
  std::vector<double> uDividends(iTimes, dDividend);
  std::transform(uDividends.begin(), uDividends.end() - 1, uDividends.begin() + 1, [dStep](double dD)
                 { return dD + dStep; });

  print(dInitialTime, "initial time");
  print(dRate, "interest rate");
  print(dSpot, "spot", true);
  test::print("Stock dividends:", uTimes, uDividends);

  std::function<double(double)> uForwardStockDividends =
      vega::forwardStockDividends(dSpot, uTimes, uDividends,
                                  uDiscount);
  double dInterval = iTimes * dPeriod / 1.01;
  test::print(uForwardStockDividends, dInitialTime, dInterval);
}

void forwardSwapRate()
{
  test::print("FORWARD SWAP RATES");

  double dRate = 0.03;
  double dInitialTime = 1.5;
  double dPeriod = 0.25;
  unsigned iNumberOfPayments = 4;

  print(dPeriod, "swap period");
  print(iNumberOfPayments, "number of payments");
  print(dInitialTime, "initial time", true);

  auto uDiscount = DF(dRate, 4. * dRate, dInitialTime);
  std::function<double(double)> uForwardSwapRates =
      vega::forwardSwapRate(dPeriod, iNumberOfPayments, uDiscount);

  double dInterval = 5;
  // test::print(uDiscount, dInitialTime, dInterval);
  test::print(uForwardSwapRates, dInitialTime, dInterval);
}

void volatilityHullWhite()
{
  test::print("STATIONARY IMPLIED VOLATILITY IN HULL-WHITE MODEL");

  double dLambda = 0.05;
  double dSigma = 0.2;
  double dInitialTime = 0.75;

  print(dLambda, "lambda");
  print(dSigma, "sigma");
  print(dInitialTime, "initial time", true);

  double dPeriod = 0.5;
  print(dPeriod, "bond maturity - option maturity", true);

  std::function<double(double, double)> uVol =
      vega::volatilityHullWhite(dLambda, dSigma, dInitialTime);
  std::function<double(double)> uVolatility = [uVol, dPeriod](double dS)
  {
    return uVol(dS, dS + dPeriod);
  };

  double dInterval = 1;
  test::print(uVolatility, dInitialTime, dInterval);
}

void volatilityVar()
{
  test::print("VOLATILITY CURVE FROM VARIANCE CURVE");

  double dInitialTime = 0.75;
  print(dInitialTime, "initial time", true);

  double dSigma = 0.25;

  std::function<double(double)> uVol = [dSigma, dInitialTime](double dT)
  {
    return dSigma * (1. + (dT - dInitialTime));
  };

  std::function<double(double)> uVar = [dInitialTime, uVol](double dT)
  {
    double dSigma = uVol(dT);
    return dSigma * dSigma * (dT - dInitialTime); };

  std::function<double(double)> uV = vega::volatilityVar(uVar, dInitialTime);

  double dInterval = 1.;
  test::print(uV, dInitialTime, dInterval);
}

void volatilityVarLinInterp()
{
  test::print("VOLATILITY CURVE BY LINEAR INTERPOLATION OF VARIANCE CURVE");

  double dInitialTime = 1.;
  auto uV = test::getVol(dInitialTime);

  auto uResult =
      vega::volatilityVarLinInterp(uV.first, uV.second, dInitialTime);
  double dInterval = uV.first.back() - dInitialTime;
  test::print(uResult, dInitialTime, dInterval);
}

void yieldVasicek()
{
  test::print("YIELD CURVE IN VASICEK MODEL");

  double dLambda = 0.05;
  double dTheta = 0.02;
  double dR0 = 0.04;
  double dSigma = 0.01;
  double dInitialTime = 1.5;

  print(dTheta, "theta");
  print(dLambda, "lambda");
  print(dSigma, "sigma");
  print(dR0, "r_0");
  print(dInitialTime, "initial time", true);

  std::function<double(double)> uYield =
      vega::yieldVasicek(dTheta, dLambda, dSigma, dR0, dInitialTime);
  double dInterval = 5;
  test::print(uYield, dInitialTime, dInterval);
}

std::function<void()> test_prep2()
{
  return []()
  {
    print("DATA CURVES FOR FINANCIAL MODELS");

    carryBlack();
    yieldVasicek();
    discountVasicek();
    discountLogLinInterp();
    forwardAnnuity();
    forwardStockDividends();
    forwardSwapRate();
    volatilityVar();
    volatilityVarLinInterp();
    volatilityHullWhite();
  };
}

int main()
{
  project(test_prep2(), PROJECT_NAME, PROJECT_NAME,
          "Set 2");
}
