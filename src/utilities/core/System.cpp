/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "System.hpp"

#include <boost/chrono/duration.hpp>
#include <boost/thread.hpp>
#include <boost/numeric/ublas/lu.hpp>

#include <cstdlib>  // std::getenv

namespace openstudio {

void System::msleep(int msecs) {
  boost::this_thread::sleep_for(boost::chrono::milliseconds(msecs));
}

unsigned System::numberOfProcessors() {
  unsigned numberOfProcessors = boost::thread::hardware_concurrency();
  if (numberOfProcessors < 1) {
    numberOfProcessors = 1;
  }
  return numberOfProcessors;
}

void System::testExceptions1() {
  try {
    std::cout << "testExceptions1: Test 1" << '\n';
    throw std::exception();
  } catch (const std::exception&) {
  }

  try {
    std::cout << "testExceptions1: Test 2" << '\n';
    throw std::exception();
  } catch (...) {
  }
}

void System::testExceptions2() {
  try {
    std::cout << "testExceptions2: Test 1" << '\n';
    throw std::runtime_error("test");
  } catch (const std::runtime_error&) {
  }

  try {
    std::cout << "testExceptions2: Test 2" << '\n';
    throw std::runtime_error("test");
  } catch (const std::exception&) {
  }

  try {
    std::cout << "testExceptions2: Test 3" << '\n';
    throw std::runtime_error("test");
  } catch (...) {
  }
}

void System::testExceptions5() {
  using namespace boost::numeric::ublas;
  using namespace std;

  struct BreakUBlas
  {
    static bool invertMatrix(matrix<double>& orig, matrix<double>& inverted) {
      using pmatrix = permutation_matrix<std::size_t>;
      matrix<double> A(orig);
      pmatrix pm(A.size1());

      // perform LU-factorization
      int res = lu_factorize(A, pm);
      if (res != 0) {
        return false;
      }

      inverted.assign(identity_matrix<double>(A.size1()));
      lu_substitute(A, pm, inverted);

      return true;
    };

    static void breakIt() {
      constexpr std::array<std::array<double, 6>, 6> data{{
        {15, 29700, 472042, 7.8021e+06, 1.32426e+08, 2.29091e+09},
        {29700, 1.32426e+08, 2.29091e+09, 4.01989e+10, 7.13142e+11, 1.27611e+13},
        {472042, 2.29091e+09, 4.01989e+10, 7.13142e+11, 1.27611e+13, 2.29941e+14},
        {7.8021e+06, 4.01989e+10, 7.13142e+11, 1.27611e+13, 2.29941e+14, 4.16694e+15},
        {1.32426e+08, 7.13142e+11, 1.27611e+13, 2.29941e+14, 4.16694e+15, 7.58705e+16},
        {2.29091e+09, 1.27611e+13, 2.29941e+14, 4.16694e+15, 7.58705e+16, 1.38694e+18},
      }};

      matrix<double> a(6, 6);
      for (unsigned i = 0; i < a.size1(); ++i) {
        for (unsigned j = 0; j < a.size2(); ++j) {
          a(i, j) = data[i][j];
        }
      }

      matrix<double> b(a);
      invertMatrix(a, b);
    }
  };

  try {
    std::cout << "testExceptions5: Test 1" << '\n';
    BreakUBlas::breakIt();
    assert(!"Exception Not Thrown");
  } catch (boost::numeric::ublas::internal_logic&) {
  }

  try {
    std::cout << "testExceptions5: Test 2" << '\n';
    BreakUBlas::breakIt();
    assert(!"Exception Not Thrown");
  } catch (std::logic_error&) {
  }

  try {
    std::cout << "testExceptions5: Test 3" << '\n';
    BreakUBlas::breakIt();
    assert(!"Exception Not Thrown");
  } catch (std::exception&) {
  }
}

void System::testExceptions3() {
  class MyException : public std::runtime_error
  {
   public:
    MyException() : std::runtime_error("exception") {}
    MyException(const MyException&) = default;
    MyException& operator=(const MyException&) = default;
    MyException(MyException&&) = default;
    MyException& operator=(MyException&&) = default;
    virtual ~MyException() noexcept override = default;
  };

  try {
    std::cout << "testExceptions3: Test 1" << '\n';
    throw MyException();
  } catch (const MyException&) {
  }

  try {
    std::cout << "testExceptions3: Test 2" << '\n';
    throw MyException();
  } catch (const std::runtime_error&) {
  }

  try {
    std::cout << "testExceptions3: Test 3" << '\n';
    throw MyException();
  } catch (const std::exception&) {
  }

  try {
    std::cout << "testExceptions3: Test 4" << '\n';
    throw MyException();
  } catch (...) {
  }
}

void System::testExceptions4() {
  class MyException : public std::runtime_error
  {
   public:
    MyException() : std::runtime_error("exception") {}
    MyException(const MyException&) = default;
    MyException& operator=(const MyException&) = default;
    MyException(MyException&&) = default;
    MyException& operator=(MyException&&) = default;
    virtual ~MyException() noexcept override = default;
  };

  class MyException2 : public MyException
  {
   public:
    MyException2() = default;
    MyException2(const MyException2&) = default;
    MyException2& operator=(const MyException2&) = default;
    MyException2(MyException2&&) = default;
    MyException2& operator=(MyException2&&) = default;
    virtual ~MyException2() noexcept override = default;
  };

  try {
    std::cout << "testExceptions4: Test 1" << '\n';
    throw MyException2();
  } catch (const MyException2&) {
  }

  try {
    std::cout << "testExceptions4: Test 2" << '\n';
    throw MyException2();
  } catch (const MyException&) {
  }

  try {
    std::cout << "testExceptions4: Test 3" << '\n';
    throw MyException2();
  } catch (const std::runtime_error&) {
  }

  try {
    std::cout << "testExceptions4: Test 4" << '\n';
    throw MyException2();
  } catch (const std::exception&) {
  }

  try {
    std::cout << "testExceptions4: Test 5" << '\n';
    throw MyException2();
  } catch (...) {
  }
}

}  // namespace openstudio
