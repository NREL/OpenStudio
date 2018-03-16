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

#include <GeographicLib/CassiniSoldner.hpp>

namespace GeographicLib {

  using namespace std;

  CassiniSoldner::CassiniSoldner(const Geodesic& earth)
    : _earth(earth) {}

  CassiniSoldner::CassiniSoldner(real lat0, real lon0, const Geodesic& earth)
    : _earth(earth)
  { Reset(lat0, lon0); }

  void CassiniSoldner::Reset(real lat0, real lon0) {
    _meridian = _earth.Line(lat0, lon0, real(0),
                            Geodesic::LATITUDE | Geodesic::LONGITUDE |
                            Geodesic::DISTANCE | Geodesic::DISTANCE_IN |
                            Geodesic::AZIMUTH);
    real f = _earth.Flattening();
    Math::sincosd(LatitudeOrigin(), _sbet0, _cbet0);
    _sbet0 *= (1 - f);
    Math::norm(_sbet0, _cbet0);
  }

  void CassiniSoldner::Forward(real lat, real lon, real& x, real& y,
                               real& azi, real& rk) const {
    if (!Init())
      return;
    real dlon = Math::AngDiff(LongitudeOrigin(), lon);
    real sig12, s12, azi1, azi2;
    sig12 = _earth.Inverse(lat, -abs(dlon), lat, abs(dlon), s12, azi1, azi2);
    sig12 *= real(0.5);
    s12 *= real(0.5);
    if (s12 == 0) {
      real da = Math::AngDiff(azi1, azi2)/2;
      if (abs(dlon) <= 90) {
        azi1 = 90 - da;
        azi2 = 90 + da;
      } else {
        azi1 = -90 - da;
        azi2 = -90 + da;
      }
    }
    if (dlon < 0) {
      azi2 = azi1;
      s12 = -s12;
      sig12 = -sig12;
    }
    x = s12;
    azi = Math::AngNormalize(azi2);
    GeodesicLine perp(_earth.Line(lat, dlon, azi, Geodesic::GEODESICSCALE));
    real t;
    perp.GenPosition(true, -sig12,
                     Geodesic::GEODESICSCALE,
                     t, t, t, t, t, t, rk, t);

    real salp0, calp0;
    Math::sincosd(perp.EquatorialAzimuth(), salp0, calp0);
    real
      sbet1 = lat >=0 ? calp0 : -calp0,
      cbet1 = abs(dlon) <= 90 ? abs(salp0) : -abs(salp0),
      sbet01 = sbet1 * _cbet0 - cbet1 * _sbet0,
      cbet01 = cbet1 * _cbet0 + sbet1 * _sbet0,
      sig01 = atan2(sbet01, cbet01) / Math::degree();
    _meridian.GenPosition(true, sig01,
                          Geodesic::DISTANCE,
                          t, t, t, y, t, t, t, t);
  }

  void CassiniSoldner::Reverse(real x, real y, real& lat, real& lon,
                               real& azi, real& rk) const {
    if (!Init())
      return;
    real lat1, lon1;
    real azi0, t;
    _meridian.Position(y, lat1, lon1, azi0);
    _earth.Direct(lat1, lon1, azi0 + 90, x, lat, lon, azi, rk, t);
  }

} // namespace GeographicLib
