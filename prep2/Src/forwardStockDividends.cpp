#include "prep2/prep2.hpp"
#include "header.hpp"
// DONE

std::function<double(double)>
vega::forwardStockDividends(double dSpot,
                            const std::vector<double> &rDividendsTimes,
                            const std::vector<double> &rDividends,
                            const std::function<double(double)> &rDiscount)
{
    PRECONDITION(is_sorted(rDividendsTimes.begin(), rDividendsTimes.end(), std::less_equal<double>()));
    PRECONDITION(rDividends.size() == rDividendsTimes.size());

    return [dSpot, rDividendsTimes, rDividends, rDiscount](double dT)
    {
        PRECONDITION(dT <= rDividendsTimes.back());
        unsigned iTime = std::upper_bound(rDividendsTimes.begin(), rDividendsTimes.end(), dT) - rDividendsTimes.begin(); // 1st elem before dT
        double dSum = 0.;
        dSum = std::inner_product(rDividends.begin(), rDividends.begin() + iTime,
                                  rDividendsTimes.begin(),
                                  dSum, std::plus<double>(),
                                  [rDiscount, dT](double dDividends, double dTimes)
                                  {
                                      return dDividends * rDiscount(dTimes) / rDiscount(dT);
                                  });
        double dF = dSpot / rDiscount(dT) - dSum;
        return dF;
    };
}