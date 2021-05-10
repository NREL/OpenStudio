/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "Matrix.hpp"

#include "../math/FloatCompare.hpp"

#include <random>

// this should all be moved to a utilities/core/Random.h
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

namespace openstudio {

bool operator==(const Matrix& lhs, const Matrix& rhs) {
  bool result = false;
  if (lhs.size1() == rhs.size1()) {
    if (lhs.size2() == rhs.size2()) {
      result = true;
      for (unsigned i = 0; i < lhs.size1(); ++i) {
        for (unsigned j = 0; j < lhs.size2(); ++j) {
          if (lhs(i, j) != rhs(i, j)) {
            return false;
          }
        }
      }
    }
  }
  return result;
}

bool operator!=(const Matrix& lhs, const Matrix& rhs) {
  return !(lhs == rhs);
}

/// linear interpolation of the function v = f(x, y) at point xi, yi
/// assumes that x and y are strictly increasing
double interp(const Vector& x, const Vector& y, const Matrix& v, double xi, double yi, InterpMethod interpMethod, ExtrapMethod extrapMethod) {
  double result = 0.0;

  size_t M = x.size();
  size_t N = y.size();

  if ((M != v.size1()) || (N != v.size2())) {
    return result;
  }

  InterpInfo xInfo = interpInfo(x, xi);

  if (xInfo.extrapolated) {
    switch (extrapMethod) {
      case NoneExtrap:
        // set all weights to zero
        xInfo.wa = 0.0;
        xInfo.wb = 0.0;
        break;
      case NearestExtrap:
        // pick closest point
        // no-op
        break;
    }
  } else {
    switch (interpMethod) {
      case LinearInterp:
        // linear interpolation
        // no-op
        break;
      case NearestInterp:
        // pick closest point
        if (xInfo.wa > xInfo.wb) {
          xInfo.wa = 1.0;
          xInfo.wb = 0.0;
        } else {
          xInfo.wa = 0.0;
          xInfo.wb = 1.0;
        }
        break;
      case HoldLastInterp:
        // set to previous value
        xInfo.wa = 1.0;
        xInfo.wb = 0.0;
        break;
      case HoldNextInterp:
        // set to next value
        xInfo.wa = 0.0;
        xInfo.wb = 1.0;
        break;
    }
  }

  InterpInfo yInfo = interpInfo(y, yi);

  if (yInfo.extrapolated) {
    switch (extrapMethod) {
      case NoneExtrap:
        // set all weights to zero
        yInfo.wa = 0.0;
        yInfo.wb = 0.0;
        break;
      case NearestExtrap:
        // pick closest point
        // no-op
        break;
    }
  } else {
    switch (interpMethod) {
      case LinearInterp:
        // linear interpolation
        // no-op
        break;
      case NearestInterp:
        // pick closest point
        if (yInfo.wa > yInfo.wb) {
          yInfo.wa = 1.0;
          yInfo.wb = 0.0;
        } else {
          yInfo.wa = 0.0;
          yInfo.wb = 1.0;
        }
        break;
      case HoldLastInterp:
        // set to previous value
        yInfo.wa = 1.0;
        yInfo.wb = 0.0;
        break;
      case HoldNextInterp:
        // set to next value
        yInfo.wa = 0.0;
        yInfo.wb = 1.0;
        break;
    }
  }

  // we have set weights appropriately so that here we can compute in the same way all the time
  result = xInfo.wa * yInfo.wa * v(xInfo.ia, yInfo.ia) + xInfo.wa * yInfo.wb * v(xInfo.ia, yInfo.ib) + xInfo.wb * yInfo.wa * v(xInfo.ib, yInfo.ia)
           + xInfo.wb * yInfo.wb * v(xInfo.ib, yInfo.ib);

  return result;
}

/// linear interpolation of the function v = f(x, y) at points xi, yi
/// assumes that x and y are strictly increasing
Vector interp(const Vector& x, const Vector& y, const Matrix& v, const Vector& xi, double yi, InterpMethod interpMethod, ExtrapMethod extrapMethod) {
  size_t M = x.size();

  Vector result(M);

  if (M != v.size1()) {
    return result;
  }

  for (unsigned i = 0; i < M; ++i) {
    result(i) = interp(x, y, v, xi(i), yi, interpMethod, extrapMethod);
  }

  return result;
}

/// linear interpolation of the function v = f(x, y) at points xi, yi
/// assumes that x and y are strictly increasing
Vector interp(const Vector& x, const Vector& y, const Matrix& v, double xi, const Vector& yi, InterpMethod interpMethod, ExtrapMethod extrapMethod) {
  size_t N = y.size();

  Vector result(N);

  if (N != v.size2()) {
    return result;
  }

  for (unsigned j = 0; j < N; ++j) {
    result(j) = interp(x, y, v, xi, yi(j), interpMethod, extrapMethod);
  }

  return result;
}

/// linear interpolation of the function v = f(x, y) at points xi, yi
/// assumes that x and y are strictly increasing
Matrix interp(const Vector& x, const Vector& y, const Matrix& v, const Vector& xi, const Vector& yi, InterpMethod interpMethod,
              ExtrapMethod extrapMethod) {
  size_t M = x.size();
  size_t N = y.size();

  Matrix result(M, N);

  if ((M != v.size1()) || (N != v.size2())) {
    return result;
  }

  for (unsigned i = 0; i < M; ++i) {
    for (unsigned j = 0; j < N; ++j) {
      result(i, j) = interp(x, y, v, xi(i), yi(j), interpMethod, extrapMethod);
    }
  }

  return result;
}

/// matrix product
Matrix prod(const Matrix& lop, const Matrix& rop) {
  return boost::numeric::ublas::prod(lop, rop);
}

/// vector product
Vector prod(const Matrix& m, const Vector& v) {
  return boost::numeric::ublas::prod(m, v);
}

/// outer product
Matrix outerProd(const Vector& lhs, const Vector& rhs) {
  return boost::numeric::ublas::outer_prod(lhs, rhs);
}

/// take the natural logarithm of a Matrix
Matrix log(const Matrix& v) {
  size_t M = v.size1();
  size_t N = v.size2();
  Matrix result(M, N);
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < N; ++j) {
      result(i, j) = std::log(v(i, j));
    }
  }
  return result;
}

/// take the logarithm of a Matrix with certain base
Matrix log(const Matrix& v, double base) {
  double logBase = std::log(base);
  size_t M = v.size1();
  size_t N = v.size2();
  Matrix result(M, N);
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < N; ++j) {
      result(i, j) = std::log(v(i, j)) / logBase;
    }
  }
  return result;
}

/// generates a Matrix of M*N points randomly drawn between and including a and b.
Matrix randMatrix(double a, double b, unsigned M, unsigned N) {
  Matrix result(M, N);

  // handle degenerate case
  if (equal(a, b)) {
    for (unsigned i = 0; i < M; ++i) {
      for (unsigned j = 0; j < N; ++j) {
        result(i, j) = a;
      }
    }
    return result;
  }

  // ETH@20100120. What library does this come from? The user should be able to seed the
  // generator independently of this function.
  // seed random number generator
  thread_local std::minstd_rand generator(42u);

  // define distribution
  boost::uniform_real<> dist(a, b);

  // create a generator
  boost::variate_generator<std::minstd_rand&, boost::uniform_real<>> uniformGenerator(generator, dist);

  for (unsigned i = 0; i < M; ++i) {
    for (unsigned j = 0; j < N; ++j) {
      result(i, j) = uniformGenerator();
    }
  }
  return result;
}

/// sum
double sum(const Matrix& matrix) {
  double result = 0.0;
  for (unsigned i = 0; i < matrix.size1(); ++i) {
    for (unsigned j = 0; j < matrix.size2(); ++j) {
      result += matrix(i, j);
    }
  }
  return result;
}

/// maximum
double maximum(const Matrix& matrix) {
  double max = 0;
  if ((matrix.size1() > 0) && (matrix.size2() > 0)) {
    max = matrix(0, 0);
    for (unsigned i = 0; i < matrix.size1(); ++i) {
      for (unsigned j = 0; j < matrix.size2(); ++j) {
        max = std::max(max, matrix(i, j));
      }
    }
  }
  return max;
}

/// minimum
double minimum(const Matrix& matrix) {
  double min = 0;
  if ((matrix.size1() > 0) && (matrix.size2() > 0)) {
    min = matrix(0, 0);
    for (unsigned i = 0; i < matrix.size1(); ++i) {
      for (unsigned j = 0; j < matrix.size2(); ++j) {
        min = std::min(min, matrix(i, j));
      }
    }
  }
  return min;
}

/// mean
double mean(const Matrix& matrix) {
  double avg = 0;
  size_t N = matrix.size1() * matrix.size2();
  if (N > 0) {
    avg = sum(matrix) / N;
  }
  return avg;
}

/// get the connected components from an NxN adjacency matrix (1.0 for i-j connected, 0.0 for i-j not connected)
std::vector<std::vector<unsigned>> findConnectedComponents(const Matrix& matrix) {
  double tol = 0.001;

  std::vector<std::vector<unsigned>> result;

  size_t N = matrix.size1();
  if (N != matrix.size2()) {
    return result;
  }

  Matrix A(N, N, 0.0);
  for (unsigned i = 0; i < N; ++i) {

    A(i, i) = 1.0;  // must be self connected
    if (std::abs(matrix(i, i) - 1.0) > tol) {
      // warn
    }

    for (unsigned j = i + 1; j < N; ++j) {

      if (matrix(i, j) < 0) {
        // warn
      } else if (matrix(i, j) > tol) {
        A(i, j) = 1.0;
      }

      if (matrix(j, i) < 0) {
        // warn
      } else if (matrix(j, i) > tol) {
        A(j, i) = 1.0;
      }
    }
  }

  // raise A to the Nth power, maximum distance between two nodes
  for (unsigned n = 0; n < N; ++n) {
    A = prod(A, A);

    // re-normalize
    for (unsigned i = 0; i < N; ++i) {
      for (unsigned j = 0; j < N; ++j) {
        if (A(j, i) > 1) {
          A(j, i) = 1;
        }
      }
    }
  }

  std::set<unsigned> added;
  for (unsigned i = 0; i < N; ++i) {
    if (added.find(i) != added.end()) {
      continue;
    }

    std::vector<unsigned> group;
    group.push_back(i);
    added.insert(i);

    for (unsigned j = i + 1; j < N; ++j) {
      if ((A(i, j) > 0) || (A(j, i) > 0)) {
        group.push_back(j);
        added.insert(j);
      }
    }

    result.push_back(group);
  }

  return result;
}

}  // namespace openstudio
