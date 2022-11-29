#include "prep2/prep2.hpp"
#include "header.hpp"
// DONE
std::function<double(double)>
vega::forwardSwapRate(double dPeriod, unsigned iNumberOfPayments,
                  const std::function<double(double)> &rDiscount)
{
    return [dPeriod, iNumberOfPayments, rDiscount](double dT)
    {
        double dSum = 0.;
        for (unsigned i = 0; i < iNumberOfPayments; ++i)
        {
            dSum += rDiscount(dT + (i+1)*dPeriod);
        }

        return (rDiscount(dT) - rDiscount(dT + iNumberOfPayments*dPeriod)) / (dSum * dPeriod);
    };
}