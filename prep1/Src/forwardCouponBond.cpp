#include "prep1/prep1.hpp"



std::function<double(double)>
vega::forwardCouponBond(double dRate, double dPeriod, double dMaturity,
                        const std::function<double(double)> &rDiscount,
                        bool bClean) // Forward price curve for a coupon bond
{
    return [dRate, dPeriod, dMaturity, rDiscount, bClean](double dT)
    {
        double dPayTime = dMaturity;
        double dSum = 0;
        while (dPayTime > dT)
        {
            dSum += rDiscount(dPayTime);
            dPayTime -= dPeriod;
        }
        double dPayment = dRate * dPeriod;
        dSum *= dPayment;
        dSum += rDiscount(dMaturity);
        double dF = dSum / rDiscount(dT);
        if (bClean)
        {
            dF -= dRate * (dT - dPayTime);
        }
        return dF;
    };
}