#include "prep2/prep2.hpp"
#include "header.hpp"
// DONE

std::function<double(double)>
vega::forwardAnnuity(double dRate, double dPeriod, double dMaturity,
                 const std::function<double(double)> &rDiscount,
                 bool bClean)
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
        double dF = dSum / rDiscount(dT);
        if (bClean)
        {
            dF -= dRate * (dT - dPayTime);
        }
        return dF;
    };
}