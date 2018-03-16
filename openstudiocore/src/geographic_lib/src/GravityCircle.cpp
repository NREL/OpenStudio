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

#include <GeographicLib/GravityCircle.hpp>
#include <fstream>
#include <sstream>
#include <GeographicLib/Geocentric.hpp>

namespace GeographicLib {

  using namespace std;

  Math::real GravityCircle::Gravity(real lon, real& gx, real& gy, real& gz)
    const {
    real clam, slam, M[Geocentric::dim2_];
    Math::sincosd(lon, slam, clam);
    real Wres = W(clam, slam, gx, gy, gz);
    Geocentric::Rotation(_sphi, _cphi, slam, clam, M);
    Geocentric::Unrotate(M, gx, gy, gz, gx, gy, gz);
    return Wres;
  }

  Math::real GravityCircle::Disturbance(real lon, real& deltax, real& deltay,
                                        real& deltaz) const {
    real clam, slam, M[Geocentric::dim2_];
    Math::sincosd(lon, slam, clam);
    real Tres = InternalT(clam, slam, deltax, deltay, deltaz, true, true);
    Geocentric::Rotation(_sphi, _cphi, slam, clam, M);
    Geocentric::Unrotate(M, deltax, deltay, deltaz, deltax, deltay, deltaz);
    return Tres;
  }

  Math::real GravityCircle::GeoidHeight(real lon) const {
    if ((_caps & GEOID_HEIGHT) != GEOID_HEIGHT)
      return Math::NaN();
    real clam, slam, dummy;
    Math::sincosd(lon, slam, clam);
    real T = InternalT(clam, slam, dummy, dummy, dummy, false, false);
    real correction = _corrmult * _correction(clam, slam);
    return T/_gamma0 + correction;
  }

  void GravityCircle::SphericalAnomaly(real lon,
                                       real& Dg01, real& xi, real& eta)
    const {
    if ((_caps & SPHERICAL_ANOMALY) != SPHERICAL_ANOMALY) {
      Dg01 = xi = eta = Math::NaN();
      return;
    }
    real clam, slam;
    Math::sincosd(lon, slam, clam);
    real
      deltax, deltay, deltaz,
      T = InternalT(clam, slam, deltax, deltay, deltaz, true, false);
    // Rotate cartesian into spherical coordinates
    real MC[Geocentric::dim2_];
    Geocentric::Rotation(_spsi, _cpsi, slam, clam, MC);
    Geocentric::Unrotate(MC, deltax, deltay, deltaz, deltax, deltay, deltaz);
    // H+M, Eq 2-151c
    Dg01 = - deltaz - 2 * T * _invR;
    xi  = -(deltay/_gamma) / Math::degree();
    eta = -(deltax/_gamma) / Math::degree();
  }

  Math::real GravityCircle::W(real clam, real slam,
                              real& gX, real& gY, real& gZ) const {
    real Wres = V(clam, slam, gX, gY, gZ) + _frot * _Px / 2;
    gX += _frot * clam;
    gY += _frot * slam;
    return Wres;
  }

  Math::real GravityCircle::V(real clam, real slam,
                              real& GX, real& GY, real& GZ)
    const {
    if ((_caps & GRAVITY) != GRAVITY) {
      GX = GY = GZ = Math::NaN();
      return Math::NaN();
    }
    real
      Vres = _gravitational(clam, slam, GX, GY, GZ),
      f = _GMmodel / _amodel;
    Vres *= f;
    GX *= f;
    GY *= f;
    GZ *= f;
    return Vres;
  }

  Math::real GravityCircle::InternalT(real clam, real slam,
                                      real& deltaX, real& deltaY, real& deltaZ,
                                      bool gradp, bool correct) const {
    if (gradp) {
      if ((_caps & DISTURBANCE) != DISTURBANCE) {
        deltaX = deltaY = deltaZ = Math::NaN();
        return Math::NaN();
      }
    } else {
      if ((_caps & DISTURBING_POTENTIAL) != DISTURBING_POTENTIAL)
        return Math::NaN();
    }
    if (_dzonal0 == 0)
      correct = false;
    real T = (gradp
              ? _disturbing(clam, slam, deltaX, deltaY, deltaZ)
              : _disturbing(clam, slam));
    T = (T / _amodel - (correct ? _dzonal0 : 0) * _invR) * _GMmodel;
    if (gradp) {
      real f = _GMmodel / _amodel;
      deltaX *= f;
      deltaY *= f;
      deltaZ *= f;
      if (correct) {
        real r3 = _GMmodel * _dzonal0 * _invR * _invR * _invR;
        deltaX += _Px * clam * r3;
        deltaY += _Px * slam * r3;
        deltaZ += _Z * r3;
      }
    }
    return T;
  }

} // namespace GeographicLib
