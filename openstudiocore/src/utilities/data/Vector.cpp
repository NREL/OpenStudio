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

#include "Vector.hpp"

#include <algorithm>
#include <random>

// this should all be moved to a utilities/core/Random.h
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

namespace openstudio{

  Vector createVector(const std::vector<double>& values) {
    unsigned n = values.size();
    Vector result(n);
    for (unsigned i = 0; i < n; ++i) {
      result[i] = values[i];
    }
    return result;
  }

  Vector createVector(const std::vector<long>& values) {
    unsigned n = values.size();
    Vector result(n);
    for (unsigned i = 0; i < n; ++i) {
      result[i] = (double)(values[i]);
    }
    return result;
  }

  std::vector<double> toStandardVector(const Vector& values) {
    unsigned n = values.size();
    std::vector<double> result(n);
    for (unsigned i = 0; i < n; ++i) {
      result[i] = values[i];
    }
    return result;
  }

  bool operator==(const Vector& lhs, const Vector& rhs)
  {
    bool result = false;
    if (lhs.size() == rhs.size()){
      result = (norm_1(lhs-rhs) == 0);
    }
    return result;
  }

  bool operator!=(const Vector& lhs, const Vector& rhs)
  {
    return !(lhs==rhs);
  }

  /// linear interpolation of the function y = f(x) at point xi
  /// assumes that x is strictly increasing
  InterpInfo interpInfo(const Vector& x, double xi)
  {
    unsigned N = x.size();

    InterpInfo result;

    if (x(0) == xi){
      result.ia = 0; result.ib = 0; result.wa = 1.0; result.wb = 0.0; result.extrapolated = false;
    }else if (xi < x(0)){
      result.ia = 0; result.ib = 0; result.wa = 1.0; result.wb = 0.0; result.extrapolated = true;
    }else if (x(N-1) == xi){
      result.ia = N-1; result.ib = N-1; result.wa = 0.0; result.wb = 1.0; result.extrapolated = false;
    }else if (xi > x(N-1)){
      result.ia = N-1; result.ib = N-1; result.wa = 0.0; result.wb = 1.0; result.extrapolated = true;
    }else{

      // ((xi > x(0)) && (xi < x(N-1)))
      result.extrapolated = false;
      Vector::const_iterator begin = x.begin();
      Vector::const_iterator it = std::lower_bound(begin, x.end(), xi);

      result.ia = (unsigned)(it-begin-1);
      result.ib = (unsigned)(it-begin);
      result.wa = (x(result.ib)-xi)/(x(result.ib)-x(result.ia));
      result.wb = (xi-x(result.ia))/(x(result.ib)-x(result.ia));
    }

    return result;
  }

  /// linear interpolation of the function y = f(x) at point xi
  /// assumes that x is strictly increasing
  double interp(const Vector& x, const Vector& y, double xi, InterpMethod interpMethod, ExtrapMethod extrapMethod){

    unsigned N = x.size();

    double result = 0.0;

    if (y.size() != N){
      return result;
    }

    InterpInfo info = interpInfo(x, xi);

    if (info.extrapolated){
      switch(extrapMethod){
        case NoneExtrap:
          // set to zero
          result = 0.0;
          break;
        case NearestExtrap:
          // pick closest point
          result = (info.wa > info.wb ? y(info.ia) : y(info.ib));
          break;
      }
    }else{
      switch(interpMethod){
        case LinearInterp:
          // linear interpolation
          result = info.wa*y(info.ia) + info.wb*y(info.ib);
          break;
        case NearestInterp:
          // pick closest point
          result = (info.wa > info.wb ? y(info.ia) : y(info.ib));
          break;
        case HoldLastInterp:
          // set to previous value
          result = y(info.ia);
          break;
        case HoldNextInterp:
          // set to next value
          result = y(info.ib);
          break;
      }
    }

    return result;
  }

  /// linear interpolation of the function y = f(x) at points xi
  /// assumes that x is strictly increasing
  Vector interp(const Vector& x, const Vector& y, const Vector& xi, InterpMethod interpMethod, ExtrapMethod extrapMethod){

    unsigned N = x.size();

    Vector result(N);

    if (y.size() != N){
      return result;
    }

    for (unsigned i = 0; i < N; ++i){
      result(i) = interp(x, y, xi(i), interpMethod, extrapMethod);
    }

    return result;
  }

  /// generates a Vector of N points randomly drawn between and including a and b.
  Vector randVector(double a, double b, unsigned N)
  {
    // seed random number generator
    static std::minstd_rand generator(42u);

    // define distribution
    boost::uniform_real<> dist(a,b);

    // create a generator
    boost::variate_generator<std::minstd_rand&, boost::uniform_real<> > uniformGenerator(generator, dist);

    Vector result(N);
    for (unsigned n = 0; n < N; ++n){
      result(n) = uniformGenerator(); // draw from generator
    }

    return result;
  }

  /// generates a Vector of N points linearly spaced between and including a and b.
  Vector linspace(double a, double b, unsigned N)
  {
    double delta = (b-a)/(N-1);

    Vector result(N);
    for (unsigned n = 0; n < N; ++n){
      result(n) = a + n*delta;
    }

    return result;
  }

  /// generates a Vector linearly spaced points starting at a and ending before or at b with interval delta.
  Vector deltaSpace(double a, double b, double delta)
  {
    unsigned N = (unsigned) floor((b-a)/delta) + 1;

    Vector result(N);
    for (unsigned n = 0; n < N; ++n){
      result(n) = a + n*delta;
    }

    return result;
  }

  /// generates a Vector of N points logarithmically spaced between and including base^a and base^b.
  Vector logspace(double a, double b, unsigned N, double base)
  {
    Vector powers = linspace(a,b,N);

    Vector result(N);
    for (unsigned n = 0; n < N; ++n){
      result(n) = std::pow(base, powers(n));
    }

    return result;
  }

  /// take the natural logarithm of elements of a Vector
  Vector log(const Vector& x)
  {
    unsigned N = x.size();
    Vector result(N);
    for (unsigned i = 0; i < N; ++i){
      result(i) = std::log(x(i));
    }
    return result;
  }

  /// take the logarithm of elements of a Vector with certain base
  Vector log(const Vector& x, double base)
  {
    double logBase = std::log(base);
    unsigned N = x.size();
    Vector result(N);
    for (unsigned i = 0; i < N; ++i){
      result(i) = std::log(x(i)) / logBase;
    }
    return result;
  }

  /// compute the cumulative sum of a Vector
  Vector cumsum(const Vector& x, double runningSum)
  {
    unsigned N = x.size();
    Vector result(N);
    for (unsigned i = 0; i < N; ++i){
      runningSum += x(i);
      result(i) = runningSum;
    }
    return result;
  }

  /// dot product
  double dot(const Vector& lhs, const Vector& rhs)
  {
    return boost::numeric::ublas::inner_prod(lhs, rhs);
  }

  /// sum
  double sum(const Vector& vector)
  {
    return boost::numeric::ublas::sum(vector);
  }

  /// maximum
  double maximum(const Vector& vector)
  {
    double max = 0;
    if (vector.size() > 0){
      max = *std::max_element(vector.begin(), vector.end());
    }
    return max;
  }

  /// minimum
  double minimum(const Vector& vector)
  {
    double min = 0;
    if (vector.size() > 0){
      min = *std::min_element(vector.begin(), vector.end());
    }
    return min;
  }

  /// mean
  double mean(const Vector& vector)
  {
    double avg = 0;
    unsigned N = vector.size();
    if (N > 0){
      avg = sum(vector) / N;
    }
    return avg;
  }

  /// variance
  double variance(const Vector& vector)
  {
    double result = 0;
    unsigned N = vector.size();
    if (N > 0)
    {
      double sumSquares = pow(boost::numeric::ublas::norm_2(vector), 2.0);
      result = sumSquares/N - pow(mean(vector), 2);
    }
    return result;
  }

  /// std dev
  double stdDev(const Vector& vector)
  {
    return sqrt(variance(vector));
  }

  std::function<double (const Vector&)> sumVectorFunctor() {
    return std::function<double (const Vector&)> (&sum);
  }

  std::function<double (const Vector&)> maximumVectorFunctor() {
    return std::function<double (const Vector&)> (&maximum);
  }

  std::function<double (const Vector&)> minimumVectorFunctor() {
    return std::function<double (const Vector&)> (&minimum);
  }

  std::function<double (const Vector&)> meanVectorFunctor() {
    return std::function<double (const Vector&)> (&mean);
  }

  std::function<double (const Vector&)> varianceVectorFunctor() {
    return std::function<double (const Vector&)> (&variance);
  }

  std::function<double (const Vector&)> stdDevVectorFunctor() {
    return std::function<double (const Vector&)> (&stdDev);
  }

  double evaluateDoubleFromVectorFunctor(const std::function<double (const Vector&)>& functor,
                                         const Vector& vector)
  {
    return functor(vector);
  }
} // openstudio
