#ifndef __vega_prepExam_hpp__
#define __vega_prepExam_hpp__

/**
 * @file prepExam.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Exam for Vega-Prep
 * @version 1.0
 * @date 2022-10-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <functional>
#include <vector>

namespace vega
{
  /**
   * @defgroup vegaData Input data curves as std::functions.
   *
   * This module constructs input data curves for financial models.
   *
   * @{
   */

  /**
   * Computes continuously compounded cost-of-carry rate from forward price:
   * \f[
   * c(F(t),t) = \log(F(t)/S_0)/(t-t_0), \quad t\geq t_0.
   * \f]
   *
   * @param dSpot (\f$S_0\f$) Spot price of the asset.
   * @param dInitialTime (\f$t_0\f$) The initial time.
   *
   * @return Cost of carry rate curve as a function of forward price \f$F(t)\f$
   * (first arg) and maturity \f$t\f$ (second arg).
   */
  std::function<double(double, double)>
  costOfCarry(double dSpot, double dInitialTime);

  /**
   * Computes the forward exchange rate curve. The exchange rate is
   * the number of units of domestic currency per one unit of foreign
   * currency.
   *
   * @param dSpotFX The spot exchange rate.
   * @param rDomesticDiscount The domestic discount curve.
   * @param rForeignDiscount The foreign discount curve.
   *
   * @return The forward exchange rate curve.
   */
  std::function<double(double)>
  forwardFX(double dSpotFX, const std::function<double(double)> &rDomesticDiscount,
            const std::function<double(double)> &rForeignDiscount);

  /**
   * Computes yield curve \f$\gamma=\gamma(t)\f$ for the Svensson
   * model:
   * \f[
   *  \gamma(t) = c_0 + c_1 \frac{1-e^{-\lambda_1(t-t_0)}}{\lambda_1
   *  (t-t_0)} + c_2 \left(\frac{1-e^{-\lambda_1(t-t_0)}}{\lambda_1
   *  (t-t_0)} - e^{-\lambda_1 (t-t_0)}\right) + c_3
   *  \left(\frac{1-e^{-\lambda_2(t-t_0)}}{\lambda_2 (t-t_0)} -
   *  e^{-\lambda_2 (t-t_0)}\right) , \quad t\geq t_0.
   * \f]
   *
   * @param dC0 \f$c_0\f$ The first constant
   * @param dC1 \f$c_1\f$ The second constant.
   * @param dC2 \f$c_2\f$ The third constant.
   * @param dC3 \f$c_3\f$ The fourth constant.
   * @param dLambda1 \f$\lambda_1>0\f$ The first mean-reversion rate.
   * @param dLambda2 \f$\lambda_2>0\f$ The second mean-reversion
   * rate, \f$\lambda_1\not=\lambda_2\f$.
   * @param dInitialTime \f$t_0\f$ The initial time.
   *
   * @return The yield curve for the Svensson model.
   */
  std::function<double(double)>
  yieldSvensson(double dC0, double dC1, double dC2, double dC3,
                double dLambda1, double dLambda2, double dInitialTime);

  /**
   * Returns the stationary implied volatility curve for the Black
   * model:
   * \f[
   * \Sigma(t) = \sigma \sqrt{\frac{1 -\exp(-2\lambda
   *   (t-t_0))}{2\lambda (t-t_0)}}, \quad t\geq t_0.
   * \f]
   *
   * @param dSigma \f$\sigma\geq 0\f$ The short-term volatility.
   * @param dLambda \f$\lambda\geq 0\f$ The mean-reversion rate.
   * @param dInitialTime \f$t_0\f$ The initial time.
   *
   * @return The stationary implied volatility curve in Black model.
   */
  std::function<double(double)>
  volatilityBlack(double dSigma, double dLambda, double dInitialTime);

  /**
   * Computes the curve of forward LIBORs. It costs nothing to enter
   * the forward rate agreement (FRA) with maturity \f$t\f$ and period
   * \f$\delta t\f$ as either a borrower or a lender. Forward LIBOR
   * \f$L^f(t,t+\delta t)\f$ is set at issue time \f$t_0\f$. In the
   * contract,
   * 1. At maturity \f$t\f$, the borrower receives notional \f$N\f$.
   * 2. At time \f$t+\delta t\f$, the borrower pays the notional plus
   * the interest computed at the forward LIBOR:
   * \f[
   *    N(1+ L^f(t,t+\delta t)\delta t).
   * \f]
   *
   * @param dLiborPeriod \f$\delta t\f$  The LIBOR period.
   * @param rDiscount The discount curve.
   *
   * @return  \f$(L^f(t,t+\delta t))_{t\geq t_0}\f$ The
   * curve of forward LIBORs.
   */
  std::function<double(double)>
  forwardLibor(double dLiborPeriod,
               const std::function<double(double)> &rDiscount);

  /**
   * Computes forward curve
   * \f[
   * F(t) = S_0 e^{q(t)(t-t_0)}, \quad t\in [t_0,t_M],
   * \f]
   * where cost-of-carry rate \f$q=q(t)\f$ is obtained by the linear
   * interpolation of the market cost-of-carry rates. On the first
   * interval \f$[t_0,t_1]\f$, the cost-of-carry rate is constant:
   * \f[
   * q(t) = q(t_1), \quad t\in [t_0,t_1].
   * \f]
   *
   * @param dSpot \f$S_0\f$ The spot price of the stock.
   * @param rDeliveryTimes \f$(t_i)_{i=1,\dots,M}\f$ The maturities
   * of the market forward contracts, \f$t_0<t_1\f$.
   * @param rForwardPrices \f$(F(t_i))_{i=1,\dots,M}\f$ The market
   * forward prices.
   * @param dInitialTime \f$t_0\f$ The initial time.
   *
   * @return The forward curve obtained by the linear interpolation of
   * the market cost-of-carry rates.
   */
  std::function<double(double)>
  forwardCarryLinInterp(double dSpot,
                        const std::vector<double> &rDeliveryTimes,
                        const std::vector<double> &rForwardPrices,
                        double dInitialTime);
  /** @} */
} // namespace vega

#endif // of __vega_prepExam_hpp__
