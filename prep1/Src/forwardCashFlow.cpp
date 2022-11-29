#include "prep1/prep1.hpp"

// DONE 

std::function<double(double)>
vega::forwardCashFlow(const std::vector<double> &rPayments,
                      const std::vector<double> &rPaymentTimes,
                      const std::function<double(double)> &rDiscount) // Forward price curve for a cash flow
{
    PRECONDITION(is_sorted(rPaymentTimes.begin(), rPaymentTimes.end(), std::less_equal<double>()));
    PRECONDITION(rPayments.size() == rPaymentTimes.size());

    return [rPayments, rPaymentTimes, rDiscount](double dT)
    {
        PRECONDITION(dT <= rPaymentTimes.back());

        unsigned iTime = std::upper_bound(rPaymentTimes.begin(), rPaymentTimes.end(), dT) - rPaymentTimes.begin();
        double dSum = 0.;
        dSum = std::inner_product(rPayments.begin() + iTime, rPayments.end(),
                                  rPaymentTimes.begin() + iTime,
                                  dSum, std::plus<double>(),
                                  [rDiscount](double dP, double dT)
                                  {
                                      return dP * rDiscount(dT);
                                  });
        double dF = dSum / rDiscount(dT);
        return dF;
    };
}