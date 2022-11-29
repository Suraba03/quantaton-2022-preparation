#include "test/Data.hpp"
#include "test/Output.hpp"
#include <cassert>

using namespace std;
using namespace test;

void test::print(const std::function<double(double)> &rData, double dStartTime,
                 double dInterval, unsigned iPoints)
{
  test::print("VALUES VERSUS TIME:");

  int iI, iSize;
  if (dInterval == 0.)
  {
    iSize = 1;
  }
  else
  {
    iSize = iPoints;
  }

  unsigned iTime = 8;
  unsigned iSpace = 6;
  unsigned iValue = 10;

  double dPeriod = dInterval / (iSize + 0.25);
  std::cout << std::setw(iTime) << "time"
            << std::setw(iSpace) << ""
            << std::setw(iValue) << "value" << endl;

  for (iI = 0; iI < iSize + 1; iI++)
  {
    double dTime = dStartTime + iI * dPeriod;
    std::cout << std::setw(iTime) << dTime
              << std::setw(iSpace) << ""
              << std::setw(iValue) << rData(dTime) << endl;
  }
  cout << endl;
}

void test::print(const std::string &sTitle,
                 const std::vector<double> &rTimes,
                 const std::vector<double> &rValues,
                 const std::string &sTimes,
                 const std::string &sValues)
{
  std::vector<std::vector<double>> uValues = {rTimes, rValues};
  std::vector<std::string> uNames = {sTimes, sValues};
  unsigned iColumn = 10;
  unsigned iSpace = 6;
  unsigned iMaxRows = 60;

  printTable(uValues, uNames, sTitle, iColumn, iSpace, iMaxRows);
}

const std::string c_sInDF("Input discount factors:");
const std::string c_sInForward("Input forward prices:");

namespace testData
{
  const double EPS = 1E-10;

  std::function<double(double)> DF(double dRate, double dInitialTime)
  {
    return [dRate, dInitialTime](double dT)
    {
      return exp(-dRate * (dT - dInitialTime));
    };
  }

  std::function<double(double)>
  DF(std::function<double(double)> &rYield, double dInitialTime)
  {
    return [rYield, dInitialTime](double dT)
    {
      return exp(-rYield(dT) * (dT - dInitialTime));
    };
  }

  std::pair<std::vector<double>, std::vector<double>>
  getDiscount(double dInitialTime, double dRate = 0.07, double dLambda = 0.22, bool bPrint = true)
  {
    unsigned iTimes = 12;
    std::vector<double> uTimes(iTimes);
    double dPeriod = 0.5;
    uTimes.front() = dInitialTime + dPeriod;
    std::transform(uTimes.begin(), uTimes.end() - 1, uTimes.begin() + 1,
                   [dPeriod](double dX)
                   { return dX + dPeriod; });
    std::vector<double> uDiscountFactors(iTimes);
    std::function<double(double)> uYield = [dRate, dLambda, dInitialTime](double dT)
    {
      double dX = std::max(dLambda * (dT - dInitialTime), EPS);
      return dRate * (1 - std::exp(-dX)) / dX; };

    std::transform(uTimes.begin(), uTimes.end(), uDiscountFactors.begin(),
                   DF(uYield, dInitialTime));

    if (bPrint)
    {
      test::print(dInitialTime, "initial time", true);
      test::print(c_sInDF, uTimes, uDiscountFactors);
    };

    return {uTimes, uDiscountFactors};
  }

  std::pair<std::vector<double>, std::vector<double>>
  getForward(double dSpot, double dInitialTime, double dLambda = 0.22)
  {
    unsigned iTimes = 10;
    std::vector<double> uTimes(iTimes);
    double dPeriod = 0.5;
    uTimes.front() = dInitialTime + dPeriod;
    std::transform(uTimes.begin(), uTimes.end() - 1, uTimes.begin() + 1,
                   [dPeriod](double dX)
                   { return dX + dPeriod; });
    std::vector<double> uForward(iTimes);
    double dRate = 0.07;
    std::function<double(double)> uCarry = [dRate, dLambda, dInitialTime](double dT)
    {
      double dX = std::max(dLambda * (dT - dInitialTime), EPS);
      return dRate * (1 - std::exp(-dX)) / dX; };

    std::transform(uTimes.begin(), uTimes.end(), uForward.begin(),
                   [dSpot, uCarry, dInitialTime](double dT)
                   {
                     return dSpot * exp(uCarry(dT) * (dT - dInitialTime));
                   });

    test::print(dSpot, "spot");
    test::print(dInitialTime, "initial time", true);
    test::print(c_sInForward, uTimes, uForward);

    return {uTimes, uForward};
  }
} // namespace testData

std::pair<std::vector<double>, std::vector<double>>
test::getDiscount(double dInitialTime)
{
  return testData::getDiscount(dInitialTime);
}

std::pair<std::vector<double>, std::vector<double>>
test::getForward(double dSpot, double dInitialTime)
{
  return testData::getForward(dSpot, dInitialTime);
}

const std::string c_sInVol("Input volatilities:");

using namespace testData;

std::pair<std::vector<double>, std::vector<double>>
test::getVol(double dInitialTime)
{
  double dSigma = 0.035;
  double dLambda = 0.25;
  unsigned iTimes = 10;
  std::vector<double> uTimes(iTimes);
  double dPeriod = 0.5;
  uTimes.front() = dInitialTime + dPeriod;
  std::transform(uTimes.begin(), uTimes.end() - 1, uTimes.begin() + 1,
                 [dPeriod](double dX)
                 { return dX + dPeriod; });
  std::vector<double> uVols(iTimes);
  std::transform(uTimes.begin(), uTimes.end(), uVols.begin(),
                 [dSigma, dLambda, dInitialTime](double dT)
                 {
                   double dX = 2. * dLambda * (dT - dInitialTime);
                   dX = (std::abs(dX) < EPS) ? 1. + 0.5 * dX : (std::exp(dX) - 1.) / dX;

                   return dSigma * std::sqrt(dX);
                 });

  test::print(dInitialTime, "initial time", true);
  test::print(c_sInVol, uTimes, uVols, "time", "volatility");

  return {uTimes, uVols};
}

const double c_dNotional = 1000.;
const double c_dYield = 0.07;
const double c_dPeriod = 0.25;
const unsigned c_iNumberOfPeriods = 6;

test::Swap test::swapParameters()
{
  Swap uSwap;
  uSwap.notional = c_dNotional;
  uSwap.rate = c_dYield;
  uSwap.period = c_dPeriod;
  uSwap.numberOfPayments = c_iNumberOfPeriods;
  uSwap.payFloat = true;
  return uSwap;
}
