#ifndef __vega_prep1_hpp__
#define __vega_prep1_hpp__

/**
 * @file prep1.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Data structures
 * @version 1.0
 * @date 2022-10-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <functional>
#include <vector>
#include <cassert>
#include <cmath>
#include <numeric>
#include <algorithm>

#define PRECONDITION assert
#define ASSERT assert
#define ASSERT assert

namespace vega
{
  const double EPS = 1E-10;
  double shape1(double dX);
  double shape2(double dX);
  /**
   * @defgroup vegaData Input data curves as std::functions.
   *
   * This module constructs input data curves for financial models.
   *
   * @{
   */

  /**
   * Computes the Nelson-Siegel discount curve:
   * \f[
   * d(t) = e^{-\gamma(t)(t-t_0)}, \quad t\geq t_0,
   * \f]
   * where the yield curve has the form:
   * \f[
   *  \gamma(t) = c_0 + c_1 \frac{1-e^{-\lambda(t-t_0)}}{\lambda (t-t_0)} +
   *  c_2 \left(\frac{1-e^{-\lambda(t-t_0)}}{\lambda (t-t_0)}  -
   *     e^{-\lambda (t-t_0)}\right), \quad t\geq t_0.
   * \f]
   *
   * @param dC0 \f$c_0\f$ The first constant
   * @param dC1 \f$c_1\f$ The second constant.
   * @param dC2 \f$c_2\f$ The third constant.
   * @param dLambda \f$\lambda>0\f$ The mean-reversion rate.
   * @param dInitialTime \f$t_0\f$ The initial time.
   *
   * @return The yield curve for the Nelson-Siegel model.
   */
  std::function<double(double)>
  discountNelsonSiegel(double dC0, double dC1, double dC2,
                       double dLambda, double dInitialTime);

  /**
   * Computes a discount curve by linear interpolation of market
   * yields.  The construction of the discount curve is accomplished
   * in 3 steps:
   * 1. We obtain the market yields from discount factors \p rDF.
   * 2. We get the market yield curve by linear interpolation.
   * 3. We compute the discount curve from the yield curve.
   *
   * @param rTimes The maturities of market discount factors.
   * @param rDF The market discount factors.
   * @param dR The initial short-term interest rate.
   * @param dInitialTime The initial time.
   *
   * @return The discount curve obtained from the market discount
   * factors by the linear interpolation of market yields.
   */
  std::function<double(double)>
  discountYieldLinInterp(const std::vector<double> &rTimes,
                         const std::vector<double> &rDF,
                         double dR, double dInitialTime);

  /**
   * Computes the curve of forward prices for a cash flow.  The buyer
   * pays forward price \f$F(t)\f$ at delivery time \f$t\f$ and then
   * receives payments \f$(P_i)\f$ at payments times \f$t_i>t\f$.
   *
   * @param rPayments \f$(P_i)\f$ The vector of payments.

   * @param rPaymentTimes \f$(t_i)\f$ The vector of payment times.
   * The first payment time has to be strictly greater than the
   * initial time.
   * @param rDiscount The discount curve.
   *
   * @return The forward prices \f$(F(t))\f$ for the cash
   * flow.
   */
  std::function<double(double)>
  forwardCashFlow(const std::vector<double> &rPayments,
                  const std::vector<double> &rPaymentTimes,
                  const std::function<double(double)> &rDiscount);

  /**
   * Computes the curve of forward prices ("clean" or "dirty") for a
   * coupon bond. The bond pays coupons \f$q\delta t\f$ at times
   * \f$(t_i)_{i=1,\dots,M}\f$ such that
   * \f[
   *     t_0 < t_1 \leq t_0+\delta t, \quad t_{i+1}-t_i= \delta t, \quad
   *     t_M = T.
   * \f]
   * The bond also pays notional \f$N=1\f$ at maturity \f$T\f$. The buyer of
   * the forward contract pays forward price \f$F(t)\f$ at delivery time
   * \f$t\f$ and then receives coupons \f$q\delta t\f$ at payments times
   * \f$t_i>t\f$ and notional \f$N=1\f$ at maturity \f$T\f$.

   *
   * @param dRate  \f$q\f$ The coupon rate.
   * @param dPeriod  \f$\delta t\f$ The time interval between payments.
   * @param dMaturity  \f$T\f$ The maturity.
   * @param rDiscount The discount curve.
   * @param bClean This parameter controls the output. If its value
   * is \p true, then we compute "clean" prices. Otherwise, we
   * compute "dirty" prices that include the accrued interest from
   * the next coupon.
   *
   * @return The forward price ("clean" or "dirty") curve for the coupon
   * bond.
   */
  std::function<double(double)>
  forwardCouponBond(double dRate, double dPeriod, double dMaturity,
                    const std::function<double(double)> &rDiscount,
                    bool bClean);

  /**
   * Computes foreign exchange rate from spot exchange rate and
   * domestic and foreign discount factors.
   * \f[
   * F(t) = S_0 \frac{d^{\text{for}}(t)}{d^{\text{dom}}(t)},
   * \quad t\geq t_0.
   * \f]
   *
   * @param dSpotFX (\f$ S_0 \f$) Spot exchange rate.
   *
   * @return Foreign exchange rate as a function of domestic
   * \f$d^{\text{dom}}\f$ (first arg) and foreign \f$d^{\text{for}}\f$
   * (second arg) discount factors.
   */
  std::function<double(double, double)>
  forwardFX(double dSpotFX);

  /**
   * Computes continuously compounded yield from discount factors:
   * \f[
   * r(t) = -\log d(t) / (t-t_0), \quad t\geq t_0.
   * \f]
   * The first argument is a maturity, the second is a discount factor.
   *
   * @param dInitialTime (\f$t_0\f$) The initial time.
   *
   * @return Continuously compounded yield as a function of
   * maturity \f$t\f$ (first arg) and discount factor
   * \f$d(t)\f$ (second arg).
   */
  std::function<double(double, double)>
  yield(double dInitialTime);

  /**
   * Computes the continuously compounded yield curve from a discount
   * curve. We recall that
   * \f[
   *  D(t) = e^{-\gamma(t)(t-t_0)}, \quad t\geq t_0.
   * \f]
   *
   * @param rDiscount \f$(D(t))_{t\geq t_0}\f$ The discount curve.
   * @param dInitialTime \f$t_0\f$ The initial time.
   *
   * @return \f$(\gamma(t))_{t\geq t_0}:\f$ the continuously compounded
   * yield curve.
   */
  std::function<double(double)>
  yield(const std::function<double(double)> &rDiscount, double dInitialTime);

  /**
   * Computes the Nelson-Siegel yield curve:
   * \f[
   *  \gamma(t) = c_0 + c_1 \frac{1-e^{-\lambda(t-t_0)}}{\lambda (t-t_0)} +
   *  c_2 \left(\frac{1-e^{-\lambda(t-t_0)}}{\lambda (t-t_0)}  -
   *     e^{-\lambda (t-t_0)}\right), \quad t\geq t_0.
   * \f]
   *
   * @param dC0 \f$c_0\f$ The first constant
   * @param dC1 \f$c_1\f$ The second constant.
   * @param dC2 \f$c_2\f$ The third constant.
   * @param dLambda \f$\lambda>0\f$ The mean-reversion rate.
   * @param dInitialTime \f$t_0\f$ The initial time.
   *
   * @return The yield curve for the Nelson-Siegel model.
   */
  std::function<double(double)>
  yieldNelsonSiegel(double dC0, double dC1, double dC2,
                    double dLambda, double dInitialTime);

  /**
   * Computes a part of yield curve for a number of interest rate
   * models (Hull-White, Nelson-Siegel, Svensson, Vasicek):
   * \f[
   * \Gamma(t) = \frac{1 - \exp(-\lambda (t-t_0))}{\lambda (t-t_0)},
   * \quad t\geq t_0.
   * \f]
   *
   * @param dLambda \f$\lambda\geq 0\f$ The mean-reversion rate.
   * @param dInitialTime \f$t_0\f$ The initial time.
   *
   * @return A part of yield curve for a number of interest rate
   * models.
   */
  std::function<double(double)>
  yieldShape1(double dLambda, double dInitialTime);

  /**
   * Computes a part of yield curve for a number of interest rate
   * models (Nelson-Siegel, Svensson):
   * \f[
   * \Gamma(t) = \frac{1 - \exp(-\lambda (t-t_0))}{\lambda (t-t_0)}
   * - \exp(-\lambda (t-t_0)), \quad t\geq t_0.
   * \f]
   *
   * @param dLambda \f$\lambda\geq 0\f$ The mean-reversion
   * parameter.
   * @param dInitialTime \f$t_0\f$ The initial time.
   *
   * @return A part of yield curve for a number of interest rate
   * models.
   */
  std::function<double(double)>
  yieldShape2(double dLambda, double dInitialTime);

  /** @} */
} // namespace vega

#endif // of __vega_prep1_hpp__
