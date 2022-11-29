#ifndef __vega_prep2_hpp__
#define __vega_prep2_hpp__

/**
 * @file prep2.hpp
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
   * Computes cost-of-carry rate curve \f$c=(c(t))_{t\geq t_0}\f$
   * for the Black model. In this model, the log of spot
   * price evolves as
   * \f[
   * \log S_t = \log S(t_0) + X_t, \quad t\geq t_0,
   * \f]
   * where \f$X=(X_t)\f$ is an OU (Ornstein-Uhlenbeck) process
   * driven by Brownian motion \f$B=(B_t)\f$:
   * \f[
   *    dX_t =  (\theta - \lambda X_t) dt + \sigma dB_t,
   *    \quad X(t_0) = 0.
   * \f]
   * We recall that the forward price curve has the form:
   * \f[
   * F(t) = S(t_0) \exp(c(t)(t-t_0)) = \mathbb{E}(S(t)), \quad t\geq t_0.
   * \f]
   * The computations show that
   * \f[
   * q(t) = \theta \frac{1-e^{-\lambda(t-t_0)}}{\lambda(t-t_0)} +
   * \frac{\sigma^2}2 \frac{1-e^{-2\lambda(t-t_0)}}{2\lambda(t-t_0)},
   * \quad t\geq t_0.
   * \f]
   *
   * @param dTheta \f$\theta\f$ The drift term.
   * @param dLambda \f$\lambda\geq 0\f$ The mean reversion level.
   * @param dSigma  \f$\sigma\geq 0\f$ The volatility.
   * @param dInitialTime \f$t_0\f$ The initial time.
   *
   * @return The cost-of-carry rate curve in Black model.
   */
  std::function<double(double)>
  carryBlack(double dTheta, double dLambda, double dSigma,
             double dInitialTime);
  
  /**
   * Computes the discount curve by the log-linear interpolation of a
   * given collection of market discount factors.
   *
   * @param rDiscountTimes The maturities of the market discount
   * factors.
   * @param rDiscountFactors The market discount factors.
   * @param dInitialTime The initial time.
   *
   * @return The discount curve obtained from the market discount
   * factors by the log-linear interpolation.
   */
  std::function<double(double)>
  discountLogLinInterp(const std::vector<double> &rDiscountTimes,
                       const std::vector<double> &rDiscountFactors,
                       double dInitialTime);

  /**
   * We recall that the discount curve has the form:
   * \f[
   * D(t) = e^{-\gamma(t)(t-t_0)}, \quad t\geq t_0.
   * \f]
   * In the Vasicek model, yield curve \f$\gamma = \gamma(t)\f$ is given by
   * \f[
   * \gamma(t) = r(t_0) A(t) + \frac{\theta}{\lambda}(1-A(t)) -
   * \frac{\sigma^2}{2\lambda^2} (1-2A(t)+B(t)), \quad
   * t\geq t_0,
   * \f]
   * where
   * \f[
   * A(t) = \frac{1-e^{-\lambda(t-t_0)}}{\lambda(t-t_0)}, \quad B(t)
   *  = \frac{1-e^{-2\lambda(t-t_0)}}{2\lambda(t-t_0)}.
   * \f]
   *
   * @param dTheta \f$\theta\f$ The drift.
   * @param dLambda \f$\lambda\geq 0\f$ The mean-reversion rate.
   * @param dSigma \f$\sigma\geq 0\f$ The volatility.
   * @param dR0 \f$r(t_0)\f$ The initial short-term interest rate.
   * @param dInitialTime \f$t_0\f$ The initial time.
   *
   * @return The discount curve for the Vasicek model of interest
   * rates.
   */
  std::function<double(double)>
  discountVasicek(double dTheta, double dLambda, double dSigma,
                  double dR0, double dInitialTime);

  /**
   * Computes the curve of forward prices ("clean" or "dirty") for an
   * annuity. The annuity pays coupons \f$q \delta t\f$ at times
   * \f$(t_i)_{i=1,\dots,M}\f$ such that
   * \f[
   *     t_0 < t_1 \leq t_0+\delta t, \quad t_{i+1}-t_i= \delta t, \quad
   *     t_M = T.
   * \f]
   * The buyer pays forward price \f$F(t)\f$ at delivery time \f$t\f$
   * and then receives coupons \f$q\delta t\f$ at payments times
   * \f$t_i>t\f$.
   *
   * @param dRate \f$q\f$ The annuity rate.
   * @param dPeriod \f$\delta t\f$ The time interval between payments.
   * @param dMaturity \f$T\f$ The maturity.
   * @param rDiscount The discount curve.
   * @param bClean This parameter controls the output. If its value
   * is \p true, then we compute "clean" prices. Otherwise, we
   * compute "dirty" prices that include the accrued interest from
   * the next coupon.
   *
   * @return The forward prices ("clean" or "dirty") for an annuity.
   */
  std::function<double(double)>
  forwardAnnuity(double dRate, double dPeriod, double dMaturity,
                 const std::function<double(double)> &rDiscount,
                 bool bClean);
  
  /**
   * Computes the curve of forward prices for a dividend paying
   * stock. The buyer pays forward price \f$F(t)\f$ at delivery time
   * \f$t\f$ and then receives the stock. If \f$t\f$ is a dividend
   * time, then the buyer gets the dividend paid at \f$t\f$. It costs
   * nothing to enter a forward contract at its issue time \f$t_0\f$.
   *
   * @param dSpot The spot price of the stock.
   * @param rDividendsTimes The dividend times.
   * @param rDividends The dividend payments.
   * @param rDiscount The discount curve.
   *
   * @return The forward price curve for the stock.
   */
  std::function<double(double)>
  forwardStockDividends(double dSpot,
                        const std::vector<double> &rDividendsTimes,
                        const std::vector<double> &rDividends,
                        const std::function<double(double)> &rDiscount);

  /**
   * Computes the curve of forward swap rates.
   *
   * @param dPeriod The time interval between payments in the swap.
   * @param iNumberOfPayments The number of payments in the swap.
   * @param rDiscount The discount curve.
   *
   * @return The curve of forward swap rates.
   */
  std::function<double(double)>
  forwardSwapRate(double dPeriod, unsigned iNumberOfPayments,
                  const std::function<double(double)> &rDiscount);
  
  /**
   * Returns the stationary implied volatility curve for
   * the Hull and White (Vasicek) model of interest rates:
   * \f[
   * \Sigma(s,t) = \sigma
   * \frac{1-\exp(-\lambda(t-s))}{\lambda}
   * \sqrt{\frac{1-\exp(-2\lambda(s-t_0))}{2\lambda(s-t_0)}}, \quad
   * t_0\leq s\leq t,
   * \f]
   * where \f$s\f$ is the maturity of the option written on the
   * zero-coupon bond with maturity \f$t\f$.
   *
   * @param dSigma \f$\sigma\geq 0\f$ The short-term volatility.
   * @param dLambda \f$\lambda\geq 0\f$ The mean-reversion rate.
   * @param dInitialTime \f$t_0\f$ The initial time.
   *
   * @return The stationary implied volatility curve in the Hull-White
   * model as the function of option maturity (first arg) and bond
   * maturity (second arg).
   */
  std::function<double(double, double)>
  volatilityHullWhite(double dSigma, double dLambda,
                      double dInitialTime);

  /**
   * Computes volatility curve \f$\Sigma = \Sigma(t)\f$ from
   * variance curve \f$V = V(t)\f$:
   * \f[
   * V(t) = \Sigma^2(t)(t-t_0), \quad t\geq t_0.
   * \f]
   *
   * @param rVar \f$V(t)\f$ The variance function.
   * @param dInitialTime  \f$t_0\f$ The initial time.
   * @return The volatility curve.
   */
  std::function<double(double)>
  volatilityVar(const std::function<double(double)> &rVar, double dInitialTime);

  /**
   * Computes implied volatility curve \f$\Sigma = \Sigma(t)\f$ by
   * applying linear interpolation to variance curve
   * \f[
   *      V(t) = \Sigma^2(t)(t - t_0), \quad t\geq t_0.
   * \f]
   * The resulting volatility curve is constant on \f$[t_0,t_1]\f$.
   *
   * @param rTimes \f$(t_i)_{i=1,\dots,M}\f$ The maturities of the
   * market implied volatilities, \f$t_1>t_0\f$.
   * @param rVols The vector of market volatilities.
   * @param dInitialTime (\f$t_0\f$) The initial time.
   *
   * @return The volatility curve obtained by linear interpolation
   * of market variances.
   */
  std::function<double(double)>
  volatilityVarLinInterp(const std::vector<double> &rTimes,
                         const std::vector<double> &rVols,
                         double dInitialTime);
  
  /**
   * Computes yield curve \f$\gamma=(\gamma(t))_{t\geq t_0}\f$ for the Vasicek
   * model, where short-term interest rate \f$r=(r_t)\f$ is an OU
   * (Ornstein-Uhlenbeck) process driven by Brownian motion \f$B=(B_t)\f$:
   * \f[
   *   dr_t=  (\theta - \lambda r_t) dt + \sigma dB_t,
   *  \quad t\geq t_0.
   * \f]
   * We recall that the discount curve has the form:
   * \f[
   * D(t) = e^{-\gamma(t)(t-t_0)} =
   * \mathbb{E}(e^{-\int_{t_0}^t r_s ds}), \quad t\geq t_0.
   * \f]
   * Computations show (please, check) that
   * \f[
   * \gamma(t) = r(t_0) A(t) + \frac{\theta}{\lambda}(1-A(t)) -
   * \frac{\sigma^2}{2\lambda^2} (1-2A(t)+B(t)),
   * \quad t\geq t_0.
   * \f]
   * where
   * \f[
   * A(t) = \frac{1-e^{-\lambda(t-t_0)}}{\lambda(t-t_0)}, \quad B(t)
   * = \frac{1-e^{-2\lambda(t-t_0)}}{2\lambda(t-t_0)}.
   * \f]
   *
   * @param dTheta \f$\theta\f$ The drift.
   * @param dLambda \f$\lambda\geq 0\f$ The mean-reversion rate.
   * @param dSigma \f$\sigma\geq 0\f$ The volatility.
   * @param dR0 \f$r(t_0)\f$ The initial short-term interest rate.
   * @param dInitialTime \f$t_0\f$ The initial time.
   *
   * @return The yield curve for the Vasicek model of interest rates.
   */
  std::function<double(double)>
  yieldVasicek(double dTheta, double dLambda, double dSigma,
               double dR0, double dInitialTime);

  /** @} */
} // namespace vega

#endif // of __vega_prep2_hpp__
