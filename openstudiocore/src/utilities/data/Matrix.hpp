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

#ifndef UTILITIES_DATA_MATRIX_HPP
#define UTILITIES_DATA_MATRIX_HPP

#include "Vector.hpp"
#include "../UtilitiesAPI.hpp"
#include "../core/Logger.hpp"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>

namespace openstudio{

  /// Matrix
  typedef boost::numeric::ublas::matrix<double> Matrix;

//////////////////////////////////////////////////////////////////////////
// Begin SWIG'able, copy and paste into Matrix.i
//////////////////////////////////////////////////////////////////////////

  /// new operators

  UTILITIES_API bool operator==(const Matrix& lhs, const Matrix& rhs);
  UTILITIES_API bool operator!=(const Matrix& lhs, const Matrix& rhs);

  /// common methods

  /// linear interpolation of the function v = f(x, y) at point xi, yi
  /// assumes that x and y are strictly increasing
  UTILITIES_API double interp(const Vector& x, const Vector& y, const Matrix& v, double xi, double yi, InterpMethod interpMethod = LinearInterp, ExtrapMethod extrapMethod = NoneExtrap);

  /// linear interpolation of the function v = f(x, y) at points xi, yi
  /// assumes that x and y are strictly increasing
  UTILITIES_API Vector interp(const Vector& x, const Vector& y, const Matrix& v, const Vector& xi, double yi, InterpMethod interpMethod = LinearInterp, ExtrapMethod extrapMethod = NoneExtrap);

  /// linear interpolation of the function v = f(x, y) at points xi, yi
  /// assumes that x and y are strictly increasing
  UTILITIES_API Vector interp(const Vector& x, const Vector& y, const Matrix& v, double xi, const Vector& yi, InterpMethod interpMethod = LinearInterp, ExtrapMethod extrapMethod = NoneExtrap);

  /// linear interpolation of the function v = f(x, y) at points xi, yi
  /// assumes that x and y are strictly increasing
  UTILITIES_API Matrix interp(const Vector& x, const Vector& y, const Matrix& v, const Vector& xi, const Vector& yi, InterpMethod interpMethod = LinearInterp, ExtrapMethod extrapMethod = NoneExtrap);

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
  UTILITIES_API std::vector<std::vector<unsigned> > findConnectedComponents(const Matrix& matrix);

  // from the boost vault:
  // The following code inverts the matrix input using LU-decomposition with backsubstitution of unit vectors. Reference: Numerical Recipes in C, 2nd ed., by Press, Teukolsky, Vetterling & Flannery.
  /// Matrix inversion routine, using lu_factorize and lu_substitute in uBLAS to invert a matrix */
 template<class T>
 bool invert(const boost::numeric::ublas::matrix<T>& input, boost::numeric::ublas::matrix<T>& inverse) {

   // create a working copy of the input
   boost::numeric::ublas::matrix<T> A(input);

   // create a permutation matrix for the LU-factorization
   boost::numeric::ublas::permutation_matrix<std::size_t> pm(A.size1());

   // perform LU-factorization
   typename boost::numeric::ublas::matrix<T>::size_type res = boost::numeric::ublas::lu_factorize(A, pm);
   if( res != 0 ){
     LOG_FREE(Info, "boost.ublas", "boost::numeric::ublas::lu_factorize returned res = " << res <<
                    ", A = " << A << ", pm = " << pm << " for input = " << input);
     return false;
   }

   // create identity matrix of "inverse"
   inverse.assign(boost::numeric::ublas::identity_matrix<T>(A.size1()));

   // backsubstitute to get the inverse
   try {
     boost::numeric::ublas::lu_substitute(A, pm, inverse);
   }catch (std::exception& e){
     LOG_FREE(Info, "boost.ublas", "boost::numeric::ublas::lu_substitute threw exception '" << e.what() <<
                    "' for A = " << A << ", pm = " << pm);
     return false;
   }

   return true;
 }


} // openstudio

#endif //UTILITIES_DATA_MATRIX_HPP
