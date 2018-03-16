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

#include <GeographicLib/LocalCartesian.hpp>

namespace GeographicLib {

  using namespace std;

  void LocalCartesian::Reset(real lat0, real lon0, real h0) {
    _lat0 = Math::LatFix(lat0);
    _lon0 = Math::AngNormalize(lon0);
    _h0 = h0;
    _earth.Forward(_lat0, _lon0, _h0, _x0, _y0, _z0);
    real sphi, cphi, slam, clam;
    Math::sincosd(_lat0, sphi, cphi);
    Math::sincosd(_lon0, slam, clam);
    Geocentric::Rotation(sphi, cphi, slam, clam, _r);
  }

  void LocalCartesian::MatrixMultiply(real M[dim2_]) const {
    // M = r' . M
    real t[dim2_];
    copy(M, M + dim2_, t);
    for (size_t i = 0; i < dim2_; ++i) {
      size_t row = i / dim_, col = i % dim_;
      M[i] = _r[row] * t[col] + _r[row+3] * t[col+3] + _r[row+6] * t[col+6];
    }
  }

  void LocalCartesian::IntForward(real lat, real lon, real h,
                                  real& x, real& y, real& z,
                                  real M[dim2_]) const {
    real xc, yc, zc;
    _earth.IntForward(lat, lon, h, xc, yc, zc, M);
    xc -= _x0; yc -= _y0; zc -= _z0;
    x = _r[0] * xc + _r[3] * yc + _r[6] * zc;
    y = _r[1] * xc + _r[4] * yc + _r[7] * zc;
    z = _r[2] * xc + _r[5] * yc + _r[8] * zc;
    if (M)
      MatrixMultiply(M);
  }

  void LocalCartesian::IntReverse(real x, real y, real z,
                                  real& lat, real& lon, real& h,
                                  real M[dim2_]) const {
    real
      xc = _x0 + _r[0] * x + _r[1] * y + _r[2] * z,
      yc = _y0 + _r[3] * x + _r[4] * y + _r[5] * z,
      zc = _z0 + _r[6] * x + _r[7] * y + _r[8] * z;
    _earth.IntReverse(xc, yc, zc, lat, lon, h, M);
    if (M)
      MatrixMultiply(M);
  }

} // namespace GeographicLib
