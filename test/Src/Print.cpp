#include <random>
#include <cassert>
#include "test/Output.hpp"
#include "test/Data.hpp"
#include "test/Print.hpp"

using namespace std;
using namespace test;

// accessor functions

std::valarray<double> test::getArg(double dL, double dR, unsigned iN)
{
  assert(iN > 0);
  
  std::valarray<double> uResult(iN);
  double dH = (dR - dL) / (iN - 1);
  double dX = dL;
  for (unsigned iI = 0; iI < iN; iI++)
  {
    uResult[iI] = dX;
    dX += dH;
  }
  uResult[uResult.size() - 1] = dR;
  return uResult;
}

std::vector<double> test::getTimes(double dInitialTime, double dMaturity, unsigned iN)
{
  std::valarray<double> uArg = getArg(dInitialTime, dMaturity, iN + 1);
  std::vector<double> uTimes(std::begin(uArg) + 1, std::end(uArg));
  return uTimes;
}

std::valarray<double> test::getRandArg(double dL, double dR, unsigned iN)
{
  assert(iN > 0);
  
  std::valarray<double> uResult(iN);
  std::minstd_rand uGen(1);
  std::uniform_real_distribution<double> uRand(dL, dR);
  for (unsigned iI = 0; iI < iN; iI++)
  {
    uResult[iI] = uRand(uGen);
  }
  std::sort(begin(uResult), end(uResult));
  assert((dL < uResult[0]) && (uResult[uResult.size() - 1] < dR));
  return uResult;
}

// print functions

void test::compare(const std::valarray<double> &rExact,
                   const std::valarray<double> &rNum,
                   const std::string &rTitle, unsigned iColumn,
                   unsigned iSpace, unsigned iMaxRows)
{
  assert(rExact.size() == rNum.size());
  
  std::vector<std::valarray<double>>
    uResults = {rExact, rNum, std::abs(rExact-rNum)};
  std::vector<std::string> uHeads = {"exact", "numeric", "error"};
  printTable(uResults, uHeads, rTitle, iColumn, iSpace, iMaxRows);
}

void test::print(double dValue, const std::string &sMessage, bool bExtraLine)
{
  std::string sM(sMessage);
  sM += std::string(" = ");
  std::cout << sM.c_str() << dValue << endl;
  if (bExtraLine)
  {
    std::cout << endl;
  }
}

void test::print(const std::string &sMessage)
{
  std::cout << sMessage.c_str() << endl
            << endl;
}

void test::printTable(const std::vector<std::valarray<double>> &rValues,
                      const std::vector<std::string> &rNames,
                      const std::string &sMessage, unsigned iColumn,
                      unsigned iSpace, unsigned iMaxRows)
{
  assert(rValues.size() == rNames.size());
  
  print(sMessage);
  for (unsigned i = 0; i < rValues.size(); i++)
  {
    std::cout << std::setw(iColumn) << rNames[i].c_str() << std::setw(iSpace) << "";
  }
  std::cout << endl;

  unsigned iSize = rValues.front().size();
  unsigned iRows = std::min(iSize, iMaxRows);
  unsigned iStart = (iSize - iRows) / 2;
  unsigned iEnd = (iSize + iRows) / 2;
  iEnd = min(iEnd, iSize);

  for (unsigned j = iStart; j < iEnd; j++)
  {
    for (unsigned i = 0; i < rValues.size(); i++)
    {
      assert(rValues[i].size() == iSize);
      std::cout << std::setw(iColumn) << rValues[i][j] << std::setw(iSpace) << "";
    }
    std::cout << endl;
  }
  std::cout << std::endl;
}

void test::printTable(const std::vector<std::vector<double>> &rValues,
                      const std::vector<std::string> &rNames,
                      const std::string &sMessage, unsigned iColumn,
                      unsigned iSpace, unsigned iMaxRows)
{
  unsigned iSize = rValues.front().size();
  std::vector<std::valarray<double>> uV(rValues.size(), std::valarray<double>(iSize));
  for (unsigned i = 0; i < rValues.size(); i++)
  {
    std::copy(rValues[i].begin(), rValues[i].end(), begin(uV[i]));
  }
  printTable(uV, rNames, sMessage, iColumn, iSpace, iMaxRows);
}

namespace testPrint
{
  void print(const CashFlow &rCashFlow, const std::string &rName)
  {
    std::string sM(rName);
    sM += std::string(":");
    test::print(sM); 
    test::print(rCashFlow.notional, "notional"); 
    test::print(rCashFlow.period, "period between payments");
    test::print(rCashFlow.numberOfPayments, "number of payments");
    test::print(rCashFlow.rate, "rate");
  }
} // namespace testPrint

void test::printCashFlow(const CashFlow &rCashFlow, const std::string &rName)
{
  testPrint::print(rCashFlow, rName);
  cout << endl;
}
