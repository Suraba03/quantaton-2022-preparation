#include "prep2/prep2.hpp"
#include "header.hpp"
// DONE

std::function<double(double)>
vega::volatilityVarLinInterp(const std::vector<double> &rTimes,
                         const std::vector<double> &rVols,
                         double dInitialTime)
{
    PRECONDITION(rTimes.size() == rVols.size());
    PRECONDITION(rTimes.front() > dInitialTime);
    PRECONDITION(std::is_sorted(rTimes.begin(), rTimes.end(), std::less_equal<double>()));

    return [rTimes, rVols, dInitialTime](double dT)
    {
        PRECONDITION(dT >= dInitialTime);
        PRECONDITION(dT <= rTimes.back());
        if (dT <= rTimes[1] && dT >= dInitialTime)
        {
            return rVols[1];
        }
        else {
            unsigned iI = std::lower_bound(rTimes.begin(), rTimes.end(), dT) - rTimes.begin();
            double dX0 = (iI > 0) ? rTimes[iI - 1] : dInitialTime;
            double dX1 = rTimes[iI];
            double dW = (dT - dX0) / (dX1 - dX0);
            double dY0 = rVols[iI-1];
            double dY1 = rVols[iI];

            return dY0 + dW * (dY1 - dY0);
        }
    };
}