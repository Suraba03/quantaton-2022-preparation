#ifndef __test_all_Data_hpp__
#define __test_all_Data_hpp__

/**
 * @file Data.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Testing of data functions.
 * @version 1.0
 * @date 2021-01-18
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "test/Main.hpp"
#include <string>

namespace test
{
  /**
   *
   * @defgroup test_all_Data Testing of data curves.
   *
   * This module contains testing functions for the
   * construction of input data curves for financial models.
   *
   * @{
   */

  /**
   * Returns a pair of vectors of times and discount factors.
   *
   * @param dInitialTime The initial time
   * @return std::pair<std::vector<double>, std::vector<double> >
   */
  std::pair<std::vector<double>, std::vector<double>>
  getDiscount(double dInitialTime);

  /**
   * Returns a pair of vectors of times and forward prices.
   *
   * @param dSpot The spot price.
   * @param dInitialTime The initial time.
   * @return std::pair<std::vector<double>, std::vector<double> >
   */
  std::pair<std::vector<double>, std::vector<double>>
  getForward(double dSpot, double dInitialTime);

  /**
   * Returns vectors of maturities and implied volatilities.
   *
   * @param dInitialTime The initial time.
   * @return std::pair<std::vector<double>, std::vector<double>>
   */
  std::pair<std::vector<double>, std::vector<double>>
  getVol(double dInitialTime);

  /**
   * Prints data curve in the specified interval.
   *
   * @param rData The data curve.
   * @param dStartTime The initial time.
   * @param dInterval The length of the interval.
   * @param iPoints The number of points in the output.
   */
  void print(const std::function<double(double)> &rData, double dStartTime,
             double dInterval, unsigned iPoints = 10);

  /**
   * Prints table with input data.
   *
   * @param sTitle The title of the table.
   * @param rTimes The vector of arguments (usually, times).
   * @param rValues The vector of values.
   * @param sTimes The name of argument.
   * @param sValues The name of values.
   */
  void print(const std::string &sTitle,
             const std::vector<double> &rTimes,
             const std::vector<double> &rValues,
             const std::string &sTimes = "time",
             const std::string &sValues = "value");

  /**
   * @brief  Cash flow at fixed rate over regular time intervals.
   *
   * This class describes the cash flow which takes place
   * at a given fixed interest rate and at regular time intervals.
   * \code coupon = notional * rate * period \endcode
   *
   * @see Swap
   */
  class CashFlow
  {
  public:
    /**
     * The notional amount.
     */
    double notional;

    /**
     * The fixed interest rate.
     */
    double rate;

    /**
     * The interval between two payments as year fraction.
     */
    double period;

    /**
     * The total number of payments.
     */
    unsigned numberOfPayments;
  };

  /**
   * @brief  Interest rate swap.
   *
   * This class describes the parameters of interest rate
   * swap. One side makes fixed payments according to
   * simple fixed CashFlow and another side makes float payments
   * according to the market (LIBOR) interest rate.
   *
   * @see CashFlow
   */
  class Swap : public CashFlow
  {
  public:
    /**
     * Default constructor.
     */
    Swap(){};

    /**
     * Constructs the interest rate swap from
     * the class CashFlow and determines the side of the
     * contract by \p bPayFloat.
     *
     * @param rCashFlow A constant reference to CashFlow
     * @param bPayFloat A side of the contract. If \p true
     * then we pay float and receive fixed, if  \p false
     * then otherwise.
     */
    Swap(const CashFlow &rCashFlow, bool bPayFloat = true);

    /**
     * The side of the contract. If <code>payFloat = true</code>, then
     * we pay float and receive fixed. If <code>payFloat = false</code>, then
     * we pay fixed and receive float.
     */
    bool payFloat;
  };

  /**
   * Returns some default parameters for an interest rate swap.
   *
   * @return Default parameters for an interest rate swap.
   */
  Swap swapParameters();
  /** @} */
} // namespace test

#endif // of __test_all_Data_hpp__
