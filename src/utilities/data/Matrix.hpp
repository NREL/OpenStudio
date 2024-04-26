/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_DATA_MATRIX_HPP
#define UTILITIES_DATA_MATRIX_HPP

#include "Vector.hpp"
#include "../UtilitiesAPI.hpp"
#include "../core/Logger.hpp"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>

namespace openstudio {

/// Matrix
using Matrix = boost::numeric::ublas::matrix<double>;

//////////////////////////////////////////////////////////////////////////
// Begin SWIG'able, copy and paste into Matrix.i
//////////////////////////////////////////////////////////////////////////

/// new operators

UTILITIES_API bool operator==(const Matrix& lhs, const Matrix& rhs);
UTILITIES_API bool operator!=(const Matrix& lhs, const Matrix& rhs);

/// common methods

/// linear interpolation of the function v = f(x, y) at point xi, yi
/// assumes that x and y are strictly increasing
/// ti is the total seconds of the timestep interval; it is used for
/// AverageInterp and must be positive
UTILITIES_API double interp(const Vector& x, const Vector& y, const Matrix& v, double xi, double yi, InterpMethod interpMethod = LinearInterp,
                            ExtrapMethod extrapMethod = NoneExtrap, double ti = -9999.0);

/// linear interpolation of the function v = f(x, y) at points xi, yi
/// assumes that x and y are strictly increasing
/// ti is the total seconds of the timestep interval; it is used for
/// AverageInterp and must be positive
UTILITIES_API Vector interp(const Vector& x, const Vector& y, const Matrix& v, const Vector& xi, double yi, InterpMethod interpMethod = LinearInterp,
                            ExtrapMethod extrapMethod = NoneExtrap, double ti = -9999.0);

/// linear interpolation of the function v = f(x, y) at points xi, yi
/// assumes that x and y are strictly increasing
/// ti is the total seconds of the timestep interval; it is used for
/// AverageInterp and must be positive
UTILITIES_API Vector interp(const Vector& x, const Vector& y, const Matrix& v, double xi, const Vector& yi, InterpMethod interpMethod = LinearInterp,
                            ExtrapMethod extrapMethod = NoneExtrap, double ti = -9999.0);

/// linear interpolation of the function v = f(x, y) at points xi, yi
/// assumes that x and y are strictly increasing
/// ti is the total seconds of the timestep interval; it is used for
/// AverageInterp and must be positive
UTILITIES_API Matrix interp(const Vector& x, const Vector& y, const Matrix& v, const Vector& xi, const Vector& yi,
                            InterpMethod interpMethod = LinearInterp, ExtrapMethod extrapMethod = NoneExtrap, double ti = -9999.0);

/// matrix product
UTILITIES_API Matrix prod(const Matrix& lop, const Matrix& rop);

/// vector product
UTILITIES_API Vector prod(const Matrix& m, const Vector& v);

/// outer product
UTILITIES_API Matrix outerProd(const Vector& lhs, const Vector& rhs);

/// take the natural logarithm of Matrix elements, componentwise
UTILITIES_API Matrix log(const Matrix& v);

/// take the logarithm of Matrix elements with respect to base, componentwise
UTILITIES_API Matrix log(const Matrix& v, double base);

/// generates a M x N Matrix whose elements come from the uniform distribution on [a,b].
UTILITIES_API Matrix randMatrix(double a, double b, unsigned M, unsigned N);

/// sum of all elements
UTILITIES_API double sum(const Matrix& matrix);

/// maximum of all elements
UTILITIES_API double maximum(const Matrix& matrix);

/// minimum of all elements
UTILITIES_API double minimum(const Matrix& matrix);

/// mean of all elements
UTILITIES_API double mean(const Matrix& matrix);

/// get the connected components from an NxN adjacency matrix (1.0 for i-j connected, 0.0 for i-j not connected)
UTILITIES_API std::vector<std::vector<unsigned>> findConnectedComponents(const Matrix& matrix);

// from the boost vault:
// The following code inverts the matrix input using LU-decomposition with backsubstitution of unit vectors. Reference: Numerical Recipes in C, 2nd ed., by Press, Teukolsky, Vetterling & Flannery.
/// Matrix inversion routine, using lu_factorize and lu_substitute in uBLAS to invert a matrix */
template <class T>
bool invert(const boost::numeric::ublas::matrix<T>& input, boost::numeric::ublas::matrix<T>& inverse) {

  // create a working copy of the input
  boost::numeric::ublas::matrix<T> A(input);

  // create a permutation matrix for the LU-factorization
  boost::numeric::ublas::permutation_matrix<std::size_t> pm(A.size1());

  // perform LU-factorization
  typename boost::numeric::ublas::matrix<T>::size_type res = boost::numeric::ublas::lu_factorize(A, pm);
  if (res != 0) {
    LOG_FREE(Info, "boost.ublas",
             "boost::numeric::ublas::lu_factorize returned res = " << res << ", A = " << A << ", pm = " << pm << " for input = " << input);
    return false;
  }

  // create identity matrix of "inverse"
  inverse.assign(boost::numeric::ublas::identity_matrix<T>(A.size1()));

  // backsubstitute to get the inverse
  try {
    boost::numeric::ublas::lu_substitute(A, pm, inverse);
  } catch (std::exception& e) {
    LOG_FREE(Info, "boost.ublas", "boost::numeric::ublas::lu_substitute threw exception '" << e.what() << "' for A = " << A << ", pm = " << pm);
    return false;
  }

  return true;
}

}  // namespace openstudio

#endif  //UTILITIES_DATA_MATRIX_HPP
