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

#include <GeographicLib/Gnomonic.hpp>

#if defined(_MSC_VER)
// Squelch warnings about potentially uninitialized local variables and
// constant conditional expressions
#  pragma warning (disable: 4701 4127)
#endif

namespace GeographicLib {

  using namespace std;

  Gnomonic::Gnomonic(const Geodesic& earth)
    : eps0_(numeric_limits<real>::epsilon())
    , eps_(real(0.01) * sqrt(eps0_))
    , _earth(earth)
    , _a(_earth.MajorRadius())
    , _f(_earth.Flattening())
  {}

  void Gnomonic::Forward(real lat0, real lon0, real lat, real lon,
                         real& x, real& y, real& azi, real& rk)
    const {
    real azi0, m, M, t;
    _earth.GenInverse(lat0, lon0, lat, lon,
                      Geodesic::AZIMUTH | Geodesic::REDUCEDLENGTH |
                      Geodesic::GEODESICSCALE,
                      t, azi0, azi, m, M, t, t);
    rk = M;
    if (M <= 0)
      x = y = Math::NaN();
    else {
      real rho = m/M;
      Math::sincosd(azi0, x, y);
      x *= rho; y *= rho;
    }
  }

  void Gnomonic::Reverse(real lat0, real lon0, real x, real y,
                         real& lat, real& lon, real& azi, real& rk)
    const {
    real
      azi0 = Math::atan2d(x, y),
      rho = Math::hypot(x, y),
      s = _a * atan(rho/_a);
    bool little = rho <= _a;
    if (!little)
      rho = 1/rho;
    GeodesicLine line(_earth.Line(lat0, lon0, azi0,
                                  Geodesic::LATITUDE | Geodesic::LONGITUDE |
                                  Geodesic::AZIMUTH | Geodesic::DISTANCE_IN |
                                  Geodesic::REDUCEDLENGTH |
                                  Geodesic::GEODESICSCALE));
    int count = numit_, trip = 0;
    real lat1, lon1, azi1, M;
    while (count-- || GEOGRAPHICLIB_PANIC) {
      real m, t;
      line.Position(s, lat1, lon1, azi1, m, M, t);
      if (trip)
        break;
      // If little, solve rho(s) = rho with drho(s)/ds = 1/M^2
      // else solve 1/rho(s) = 1/rho with d(1/rho(s))/ds = -1/m^2
      real ds = little ? (m/M - rho) * M * M : (rho - M/m) * m * m;
      s -= ds;
      // Reversed test to allow escape with NaNs
      if (!(abs(ds) >= eps_ * _a))
        ++trip;
    }
    if (trip) {
      lat = lat1; lon = lon1; azi = azi1; rk = M;
    } else
      lat = lon = azi = rk = Math::NaN();
    return;
  }

} // namespace GeographicLib
