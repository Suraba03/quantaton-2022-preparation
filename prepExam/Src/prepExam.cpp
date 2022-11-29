#include "prepExam/prepExam.hpp"

#include <cassert>
#include <cmath>
#include <numeric>
#include <algorithm>

#define PRECONDITION assert
#define ASSERT assert
#define ASSERT assert

const double EPS = 1E-10;

double shape1(double dX)
{
    PRECONDITION(dX >= 0);
    double dY = (dX > EPS) ? (1 - std::exp(-dX)) / dX : (1. - dX / 2. + dX * dX / 6.);
    return dY;
}

double shape2(double dX)
{
    PRECONDITION(dX >= 0);
    double dY = (dX > EPS) ? (1 - std::exp(-dX) * (1. + dX)) / dX : (dX / 2. - dX * dX / 3.);
    return dY;
}

std::function<double(double, double)>
vega::costOfCarry(double dSpot, double dInitialTime)
{
    return [dSpot, dInitialTime](double dFofT, double dT)
    {
        PRECONDITION(dT >= dInitialTime);
        double dY;
        if (dT - dInitialTime < EPS)
        {
            dY = std::log(dFofT / dSpot) / EPS;
        }
        else
        {
            dY = std::log(dFofT / dSpot) / (dT - dInitialTime);
        }
        return dY;
    };
}

std::function<double(double)>
vega::forwardFX(double dSpotFX, const std::function<double(double)> &rDomesticDiscount,
                const std::function<double(double)> &rForeignDiscount)
{

    return [dSpotFX, rDomesticDiscount, rForeignDiscount](double dT)
    {
        double dY;
        if (rDomesticDiscount(dT) < EPS)
        {
            dY = (dSpotFX * rForeignDiscount(dT)) / EPS;
        }
        else
        {
            dY = (dSpotFX * rForeignDiscount(dT)) / rDomesticDiscount(dT);
        }
        return dY;
    };
}

std::function<double(double)>
vega::yieldSvensson(double dC0, double dC1, double dC2, double dC3,
                    double dLambda1, double dLambda2, double dInitialTime)
{
    PRECONDITION(dLambda1 != dLambda2);

    return [dC0, dC1, dC2, dC3, dLambda1, dLambda2, dInitialTime](double dT)
    {
        PRECONDITION(dT >= dInitialTime);
        double dX1 = dLambda1 * (dT - dInitialTime);
        double dX2 = dLambda2 * (dT - dInitialTime);
        double dY = dC0 + dC1 * shape1(dX1) + dC2 * shape2(dX1) + dC3 * shape2(dX2);
        return dY;
    };
}

std::function<double(double)>
vega::volatilityBlack(double dSigma, double dLambda, double dInitialTime)
{
    PRECONDITION(dLambda >= 0);
    PRECONDITION(dSigma > 0);
    return [dSigma, dLambda, dInitialTime](double dT)
    {
        PRECONDITION(dT >= dInitialTime);
        double dX = dLambda * (dT - dInitialTime);
        double dY = dSigma * std::sqrt(shape1(2 * dX));
        return dY;
    };
}

std::function<double(double)>
vega::forwardLibor(double dLiborPeriod,
                   const std::function<double(double)> &rDiscount)
{
    return [dLiborPeriod, rDiscount](double dT)
    {
        double dDicountsRatio;
        if (rDiscount(dT + dLiborPeriod) < EPS)
        {
            dDicountsRatio = rDiscount(dT) / EPS;
        }
        else
        {
            dDicountsRatio = rDiscount(dT) / rDiscount(dT + dLiborPeriod);
        }
        double dY;
        if (dLiborPeriod > EPS)
        {
            dY = (dDicountsRatio - 1) / dLiborPeriod;
        }
        else
        {
            dY = (dDicountsRatio - 1) / EPS;
        }
        return dY;
    };
}

std::function<double(double)>
vega::forwardCarryLinInterp(double dSpot,
                            const std::vector<double> &rDeliveryTimes,
                            const std::vector<double> &rForwardPrices,
                            double dInitialTime)
{
    PRECONDITION(rDeliveryTimes.size() == rForwardPrices.size());
    PRECONDITION(rDeliveryTimes.front() > dInitialTime);
    PRECONDITION(std::is_sorted(rDeliveryTimes.begin(), rDeliveryTimes.end(), std::less_equal<double>()));

    std::function<double(double, double)> uCostOfCarry = costOfCarry(dSpot, dInitialTime);

    return [dSpot, rDeliveryTimes, rForwardPrices, dInitialTime, uCostOfCarry](double dT)
    {
        PRECONDITION(dT >= dInitialTime);
        PRECONDITION(dT <= rDeliveryTimes.back());

        if (dT <= rDeliveryTimes[0] && dT >= dInitialTime)
        {
            return dSpot * std::exp(uCostOfCarry(rForwardPrices[0], dT) * (dT - dInitialTime));
        }
        else
        {
            unsigned iI = std::lower_bound(rDeliveryTimes.begin(), rDeliveryTimes.end(), dT) - rDeliveryTimes.begin(); // first element after dT - sth
            double dX0 = (iI > 0) ? rDeliveryTimes[iI - 1] : dInitialTime;
            double dX1 = rDeliveryTimes[iI];
            double dW = (dT - dX0) / (dX1 - dX0);
            double dY0 = uCostOfCarry(rForwardPrices[iI - 1], dX0);
            double dY1 = uCostOfCarry(rForwardPrices[iI], dX1);

            return dSpot * std::exp((dY0 + dW * (dY1 - dY0)) * (dT - dInitialTime));
        }
    };
}