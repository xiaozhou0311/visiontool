#ifndef ALGOTEST_H
#define ALGOTEST_H
#include "mipp.h"

namespace algotest 
{
    template<typename T>
    void PrintRegister(const mipp::Reg<T>& r, const std::string& label) {
        T values[mipp::N<T>()];
        mipp::store<T>(values, r.r);

        std::cout << label << ": [";
        for (int i = 0; i < mipp::N<T>(); i++) {
            std::cout << (i > 0 ? ", " : "") << values[i];
        }
        std::cout << "]" << std::endl;
    }
}
#endif

