/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_DATA_VECTOR_HPP
#define UTILITIES_DATA_VECTOR_HPP

#include "../UtilitiesAPI.hpp"

#define BOOST_UBLAS_MOVE_SEMANTICS

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>

namespace openstudio {

/// Workaround to get Vector typedef, http://www.gotw.ca/gotw/079.htm
struct UTILITIES_API VectorStruct
{
  using VectorType = boost::numeric::ublas::vector<double>;
  using ScalarVectorType = boost::numeric::ublas::scalar_vector<double>;
};

/// Vector
using Vector = VectorStruct::VectorType;

/// ScalarVector
using ScalarVector = VectorStruct::ScalarVectorType;

/// Helper function to construct Vector from std::vector<double>.
UTILITIES_API Vector createVector(const std::vector<double>& values);

/// Helper function to construct Vector from std::vector<long>
UTILITIES_API Vector createVector(const std::vector<long>& values);

UTILITIES_API std::vector<double> toStandardVector(const Vector& values);

//////////////////////////////////////////////////////////////////////////
// Begin SWIG'able, copy and paste into Vector.i
//////////////////////////////////////////////////////////////////////////

/** @name Operators */
//@{

UTILITIES_API bool operator==(const Vector& lhs, const Vector& rhs);
UTILITIES_API bool operator!=(const Vector& lhs, const Vector& rhs);

//@}
/** @name Interpolation */
//@{

// The following link has hints for interpolation
// http://o2scl.sourceforge.net/o2scl/html/index.html#intp_section

/** Enum to specify the interpolation method. */
enum InterpMethod
{
  LinearInterp,
  NearestInterp,
  HoldLastInterp,
  HoldNextInterp,
  AverageInterp
};

/** Enum to specify the extrapolation method. */
enum ExtrapMethod
{
  NoneExtrap,
  NearestExtrap
};

/** Data structure for holding interpolation information. */
struct UTILITIES_API InterpInfo
{
  bool extrapolated;  // was point out of range
  unsigned ia, ib;    // indices of two nearest points
  double wa, wb;      // weights of two nearest points
  double ti;          // length of interval
};

/** Linear interpolation of the function y = f(x) at point xi. Assumes that x is strictly
 *  increasing. ti is the total seconds of the timestep interval; it is used for
 *  AverageInterp and must be positive. */
UTILITIES_API InterpInfo interpInfo(const Vector& x, double xi, double ti);

/** Linear interpolation of the function y = f(x) at point xi. Assumes that x is strictly
 *  increasing. ti is the total seconds of the timestep interval; it is used for
 *  AverageInterp and must be positive. */
UTILITIES_API double interp(const Vector& x, const Vector& y, double xi, InterpMethod interpMethod = LinearInterp,
                            ExtrapMethod extrapMethod = NoneExtrap, double ti = -9999.0);

/** Linear interpolation of the function y = f(x) at points xi. Assumes that x is strictly
 *  increasing. ti is the total seconds of the timestep interval; it is used for
 *  AverageInterp and must be positive. */
UTILITIES_API Vector interp(const Vector& x, const Vector& y, const Vector& xi, InterpMethod interpMethod = LinearInterp,
                            ExtrapMethod extrapMethod = NoneExtrap, double ti = -9999.0);

//@}
/** @name Common Methods and Vector Operations */
//@{

/** Generates a Vector of N points randomly drawn between and including a and b. */
UTILITIES_API Vector randVector(double a, double b, unsigned N);

/** Generates a Vector of N points linearly spaced between and including a and b. */
UTILITIES_API Vector linspace(double a, double b, unsigned N);

/** Generates a Vector linearly spaced points starting at a and ending before or at b with
 *  interval delta. */
UTILITIES_API Vector deltaSpace(double a, double b, double delta);

/** Generates a Vector of N points logarithmically spaced between and including base^a and
 *  base^b. */
UTILITIES_API Vector logspace(double a, double b, unsigned N, double base = 10.0);

/** Take the natural logarithm of elements of a Vector. */
UTILITIES_API Vector log(const Vector& x);

/** Take the logarithm of elements of a Vector with certain base. */
UTILITIES_API Vector log(const Vector& x, double base);

/** Compute the cumulative sum of a Vector. */
UTILITIES_API Vector cumsum(const Vector& x, double runningSum = 0.0);

/** Returns the dot product between lhs and rhs. */
UTILITIES_API double dot(const Vector& lhs, const Vector& rhs);

/** Returns the sum of vector's values. */
UTILITIES_API double sum(const Vector& vector);

/** Returns the largest element of vector. */
UTILITIES_API double maximum(const Vector& vector);

/** Returns the smallest element of vector. */
UTILITIES_API double minimum(const Vector& vector);

/** Returns the mean of vector's values */
UTILITIES_API double mean(const Vector& vector);

/** Returns the sample variance of vector's values. */
UTILITIES_API double variance(const Vector& vector);

/** Returns the standard deviation of vector's values. */
UTILITIES_API double stdDev(const Vector& vector);

/** Returns std::function pointer to sum(const Vector&). */
UTILITIES_API std::function<double(const Vector&)> sumVectorFunctor();

/** Returns std::function pointer to maximum(const Vector&). */
UTILITIES_API std::function<double(const Vector&)> maximumVectorFunctor();

/** Returns std::function pointer to minimum(const Vector&). */
UTILITIES_API std::function<double(const Vector&)> minimumVectorFunctor();

/** Returns std::function pointer to mean(const Vector&). */
UTILITIES_API std::function<double(const Vector&)> meanVectorFunctor();

/** Returns std::function pointer to variance(const Vector&). */
UTILITIES_API std::function<double(const Vector&)> varianceVectorFunctor();

/** Returns std::function pointer to stdDev(const Vector&). */
UTILITIES_API std::function<double(const Vector&)> stdDevVectorFunctor();

/** Evaluates functor(vector). For use in SWIG bindings. */
UTILITIES_API double evaluateDoubleFromVectorFunctor(const std::function<double(const Vector&)>& functor, const Vector& vector);

//@}

}  // namespace openstudio

#endif  //UTILITIES_DATA_VECTOR_HPP
