#include "test/Main.hpp"
#include "setup/Output.hpp"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <numeric>

using namespace test;
using namespace std;

namespace NSetup
{
  void gsl()
  {
    print("Test of GSL and CBLAS");

    unsigned iN = 5;
    std::vector<double> uX(iN);
    std::vector<double> uY(iN);
    std::vector<double> uA(iN*iN);

    std::iota(uX.begin(), uX.end(), 0);
    std::iota(uY.begin(), uY.end(), uX.back()+1);
    std::iota(uA.begin(), uA.end(), 0.);

    gsl_vector_const_view uXw = gsl_vector_const_view_array(uX.data(), uX.size());
    gsl_vector_view uYw = gsl_vector_view_array(uY.data(), uY.size());
    gsl_matrix_const_view uAw = gsl_matrix_const_view_array(uA.data(), iN, iN);

    std::vector<std::vector<double> > uPrint(2+iN);
    std::vector<std::string> uNames(2+iN,"");
    uPrint.front() = uX;
    uPrint[1] = uY;
    uNames.front() = "X";
    uNames[1] = "Y";
    uNames[2+iN/2] = "A";

    for (unsigned i=0; i<iN; i++){
      uPrint[i+2]= std::vector<double>(uA.begin()+i*iN, uA.begin()+(i+1)*iN);
    }

    printTable(uPrint, uNames, "vectors and matrix", iN, 3, iN);

    print(uX.begin(), uX.end(), "vector X");
    print(uY.begin(), uY.end(), "vector Y");

    double dZ;
    gsl_blas_ddot(&uXw.vector, &uYw.vector, &dZ);
    print(dZ, "scalar product of X and Y", true);


    gsl_blas_dgemv(CblasNoTrans, 1., &uAw.matrix, &uXw.vector, 1., &uYw.vector);
    print(uY.begin(), uY.end(), "product of matrix A and vector X");    
  }
}

std::function<void()> test_setup()
{
  return []() {
    print("START OF SETUP");

    NSetup::gsl();

    print("END OF SETUP");
  };
}

int main()
{
  project(test_setup(), PROJECT_NAME, PROJECT_NAME,
          "setup");
}
