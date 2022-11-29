#include "prep1/prep1.hpp"

// DONE

std::function<double(double)>
vega::discountYieldLinInterp(const std::vector<double> &rTimes,
                             const std::vector<double> &rDF,
                             double dR, double dInitialTime) // Discount curve obtained by linear interpolation of yields
{
    PRECONDITION(rTimes.size() == rDF.size());
    PRECONDITION(rTimes.front() > dInitialTime);
    PRECONDITION(std::is_sorted(rTimes.begin(), rTimes.end(), std::less_equal<double>()));

    std::function<double(double, double)> uYield = yield(dInitialTime);

    return [rTimes, rDF, dR, dInitialTime, uYield](double dT)
    {
        PRECONDITION(dT >= dInitialTime);
        PRECONDITION(dT <= rTimes.back());

        unsigned iI = std::lower_bound(rTimes.begin(), rTimes.end(), dT) - rTimes.begin(); // first element after dT - sth
        double dX0 = (iI > 0) ? rTimes[iI - 1] : dInitialTime;
        double dX1 = rTimes[iI];
        double dW = (dT - dX0) / (dX1 - dX0);
        double dY0 = (iI > 0) ? uYield(dX0, rDF[iI - 1]) : dR;
        double dY1 = uYield(dX1, rDF[iI]);

        return exp(-(dY0 + dW * (dY1 - dY0)) * (dT - dInitialTime));
    };
}; // dY0 + dW * (dY1 - dY0)