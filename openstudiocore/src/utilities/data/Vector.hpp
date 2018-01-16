/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef UTILITIES_DATA_VECTOR_HPP
#define UTILITIES_DATA_VECTOR_HPP

#include "../UtilitiesAPI.hpp"

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>

namespace openstudio {

/// Workaround to get Vector typedef, http://www.gotw.ca/gotw/079.htm
struct UTILITIES_API VectorStruct
{
  typedef boost::numeric::ublas::vector<double> VectorType;
  typedef boost::numeric::ublas::scalar_vector<double> ScalarVectorType;
};

/// Vector
typedef VectorStruct::VectorType Vector;

/// ScalarVector
typedef VectorStruct::ScalarVectorType ScalarVector;

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
enum InterpMethod{LinearInterp, NearestInterp, HoldLastInterp, HoldNextInterp};

/** Enum to specify the extrapolation method. */
enum ExtrapMethod{NoneExtrap, NearestExtrap};

/** Data structure for holding interpolation information. */
struct UTILITIES_API InterpInfo{
  bool extrapolated; // was point out of range
  unsigned ia, ib; // indices of two nearest points
  double wa, wb; // weights of two nearest points
};

/** Linear interpolation of the function y = f(x) at point xi. Assumes that x is strictly
 *  increasing. */
UTILITIES_API InterpInfo interpInfo(const Vector& x, double xi);

/** Linear interpolation of the function y = f(x) at point xi. Assumes that x is strictly
 *  increasing */
UTILITIES_API double interp(const Vector& x, const Vector& y, double xi,
                            InterpMethod interpMethod = LinearInterp,
                            ExtrapMethod extrapMethod = NoneExtrap);

/** Linear interpolation of the function y = f(x) at points xi. Assumes that x is strictly
 *  increasing. */
UTILITIES_API Vector interp(const Vector& x, const Vector& y, const Vector& xi,
                            InterpMethod interpMethod = LinearInterp,
                            ExtrapMethod extrapMethod = NoneExtrap);

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
UTILITIES_API std::function<double (const Vector&)> sumVectorFunctor();

/** Returns std::function pointer to maximum(const Vector&). */
UTILITIES_API std::function<double (const Vector&)> maximumVectorFunctor();

/** Returns std::function pointer to minimum(const Vector&). */
UTILITIES_API std::function<double (const Vector&)> minimumVectorFunctor();

/** Returns std::function pointer to mean(const Vector&). */
UTILITIES_API std::function<double (const Vector&)> meanVectorFunctor();

/** Returns std::function pointer to variance(const Vector&). */
UTILITIES_API std::function<double (const Vector&)> varianceVectorFunctor();

/** Returns std::function pointer to stdDev(const Vector&). */
UTILITIES_API std::function<double (const Vector&)> stdDevVectorFunctor();

/** Evaluates functor(vector). For use in SWIG bindings. */
UTILITIES_API double evaluateDoubleFromVectorFunctor(
    const std::function<double (const Vector&)>& functor,
    const Vector& vector);

//@}

} // openstudio

#endif //UTILITIES_DATA_VECTOR_HPP
