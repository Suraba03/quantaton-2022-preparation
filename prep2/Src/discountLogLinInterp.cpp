#include "prep2/prep2.hpp"
#include "header.hpp"
// DONE

std::function<double(double)>
vega::discountLogLinInterp(const std::vector<double> &rDiscountTimes,
                       const std::vector<double> &rDiscountFactors,
                       double dInitialTime)
{
    PRECONDITION(std::is_sorted(rDiscountTimes.begin(), rDiscountTimes.end(), std::less_equal<double>()));
    PRECONDITION(rDiscountTimes.size() == rDiscountFactors.size());
    PRECONDITION(rDiscountTimes.front() > dInitialTime);

    return [rDiscountTimes, rDiscountFactors, dInitialTime](double dT)
    {
        PRECONDITION(dT >= dInitialTime);
        PRECONDITION(dT <= rDiscountTimes.back());
        unsigned iI = std::lower_bound(rDiscountTimes.begin(), rDiscountTimes.end(), dT) - rDiscountTimes.begin(); // first element after dT - sth
        double dX0 = (iI > 0) ? rDiscountTimes[iI - 1] : dInitialTime;
        double dX1 = rDiscountTimes[iI];
        double dW = (dT - dX0) / (dX1 - dX0);
        double dY0 = (iI > 0) ? std::log(rDiscountFactors[iI - 1]) : std::log(1.);
        double dY1 = std::log(rDiscountFactors[iI]);

        return std::exp(dY0 + dW * (dY1 - dY0));
    };
}