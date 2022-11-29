#ifndef __test_all_Print_hpp__
#define __test_all_Print_hpp__

/**
 * @file Print.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Collections of testing functions.
 * @version 1.0
 * @date 2021-01-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <valarray>
#include "test/Output.hpp"
#include "test/Index.hpp"
#include "test/Data.hpp"

namespace test
{
  /**
   * @ingroup test_all
   *
   * @{
   */

  /**
   * Returns uniform partition of an interval.
   *
   * @param dL The left point of the interval.
   * @param dR The right point of the interval.
   * @param iN The number of points in the partition.
   * @return std::valarray<double> The uniform partition
   * of the interval \f$[L,U]\f$ on \p iN points.
   */
  std::valarray<double> getArg(double dL, double dR, unsigned iN);

  /**
   * Returns a testing vector of event times. The first element is greater than
   * \p dInitialTime.
   *
   * @param dInitialTime The initial time. It is not included in the result.
   * @param dMaturity The maturity.
   * @param iN The number of event times.
   * @return The testing vector of event times on
   * <code> [dInitialTime, dMaturity] </code> with \p iN elements.
   * \p dInitialTime is not included in the result.
   */
  std::vector<double> getTimes(double dInitialTime, double dMaturity, unsigned iN);

  /**
   * Returns uniformly distributed random points.
   *
   * @param dL The left point of the interval.
   * @param dR The right point of the interval.
   * @param iN The number of random points.
   * @return std::valarray<double> The partition of the interval
   * \f$[L,U]\f$ on \p iN points having the uniform distribution.
   */
  std::valarray<double> getRandArg(double dL, double dR, unsigned iN);

  /**
   * Compares numerical and exact results.
   *
   * @param rExact The exact slice.
   * @param rApprox The approximate slice.
   * @param rTitle The title of the table.
   * @param iColumn The size of every column.
   * @param iSpace The space between columns.
   * @param iMaxRows The maximal number of rows.
   */
  void compare(const std::valarray<double> &rExact,
               const std::valarray<double> &rApprox,
               const std::string &rTitle, unsigned iColumn = 12,
               unsigned iSpace = 3, unsigned iMaxRows = 9);

  /**
   * Prints displayed message.
   *
   * @param sMessage The message.
   */
  void print(const std::string &sMessage);

  /**
   * Prints the value of a parameter.
   *
   * @param dValue The value of the parameter.
   * @param rName The name of the parameter.
   * @param bExtraLine If \p true, then extra line is added at the end.
   */
  void print(double dValue, const std::string &rName, bool bExtraLine = false);

  /**
   * Prints the values of an array.
   *
   * @param start The iterator to the first element.
   * @param end The iterator to the last plus one element.
   * @param rName The name of the array.
   */
  template <class T>
  void print(T start, T end, const std::string &rName);

  /**
   * Prints the subset of the table of the results.
   *
   * @param rValues The vector of columns of the results.
   * @param rNames The vector of titles of the columns.
   * @param sMessage The title of the table.
   * @param iColumnSize The width of the column.
   * @param iSpaceSize The space between the columns.
   * @param iMaxRows The maximal number of rows in the middle of total table
   * to be printed.
   */
  void printTable(const std::vector<std::valarray<double>> &rValues,
                  const std::vector<std::string> &rNames,
                  const std::string &sMessage, unsigned iColumnSize,
                  unsigned iSpaceSize, unsigned iMaxRows);

  /**
   * Prints the subset of the table of the results.
   *
   * @param rValues The vector of columns of the results.
   * @param rNames The vector of titles of the columns.
   * @param sMessage The title of the table.
   * @param iColumnSize The width of the column.
   * @param iSpaceSize The space between the columns.
   * @param iMaxRows The maximal number of rows in the middle of total table
   * to be printed.
   */
  void printTable(const std::vector<std::vector<double>> &rValues,
                  const std::vector<std::string> &rNames,
                  const std::string &sMessage, unsigned iColumnSize = 10,
                  unsigned iSpaceSize = 6, unsigned iMaxRows = 20);

  class CashFlow;
  /**
   * Prints the parameters of a regular cash flow.
   *
   * @param rCashFlow The parameters of a regular cash flow.
   * @param rName The name of the cash flow.
   */
  void printCashFlow(const CashFlow &rCashFlow, const std::string &rName);

  /** @} */
} // namespace test

#include "test/Inline/iPrint.hpp"
#endif // of __test_all_Print_hpp__
