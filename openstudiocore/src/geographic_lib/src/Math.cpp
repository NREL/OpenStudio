/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include <GeographicLib/Math.hpp>

#if defined(_MSC_VER)
// Squelch warnings about constant conditional expressions
#  pragma warning (disable: 4127)
#endif

namespace GeographicLib {

  /// \cond SKIP

  using namespace std;

  template<typename T> T Math::eatanhe(T x, T es)  {
    return es > T(0) ? es * atanh(es * x) : -es * atan(es * x);
  }

  template<typename T> T Math::taupf(T tau, T es) {
    T tau1 = hypot(T(1), tau),
      sig = sinh( eatanhe(tau / tau1, es ) );
    return hypot(T(1), sig) * tau - sig * tau1;
  }

  template<typename T> T Math::tauf(T taup, T es) {
    static const int numit = 5;
    static const T tol = sqrt(numeric_limits<T>::epsilon()) / T(10);
    T e2m = T(1) - sq(es),
      // To lowest order in e^2, taup = (1 - e^2) * tau = _e2m * tau; so use
      // tau = taup/_e2m as a starting guess.  (This starting guess is the
      // geocentric latitude which, to first order in the flattening, is equal
      // to the conformal latitude.)  Only 1 iteration is needed for |lat| <
      // 3.35 deg, otherwise 2 iterations are needed.  If, instead, tau = taup
      // is used the mean number of iterations increases to 1.99 (2 iterations
      // are needed except near tau = 0).
      tau = taup/e2m,
      stol = tol * max(T(1), abs(taup));
    // min iterations = 1, max iterations = 2; mean = 1.94
    for (int i = 0; i < numit || GEOGRAPHICLIB_PANIC; ++i) {
      T taupa = taupf(tau, es),
        dtau = (taup - taupa) * (1 + e2m * sq(tau)) /
        ( e2m * hypot(T(1), tau) * hypot(T(1), taupa) );
      tau += dtau;
      if (!(abs(dtau) >= stol))
        break;
    }
    return tau;
  }

  // Instantiate
  template Math::real Math::eatanhe<Math::real>(Math::real, Math::real);
  template Math::real Math::taupf<Math::real>(Math::real, Math::real);
  template Math::real Math::tauf<Math::real>(Math::real, Math::real);

  /// \endcond

} // namespace GeographicLib
