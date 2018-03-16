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

#include <GeographicLib/CircularEngine.hpp>

namespace GeographicLib {

  using namespace std;

  Math::real CircularEngine::Value(bool gradp, real cl, real sl,
                                   real& gradx, real& grady, real& gradz)
    const {
    gradp = _gradp && gradp;
    const vector<real>& root_( SphericalEngine::root_ );

    // Initialize outer sum
    real vc  = 0, vc2  = 0, vs  = 0, vs2  = 0;   // v [N + 1], v [N + 2]
    // vr, vt, vl and similar w variable accumulate the sums for the
    // derivatives wrt r, theta, and lambda, respectively.
    real vrc = 0, vrc2 = 0, vrs = 0, vrs2 = 0;   // vr[N + 1], vr[N + 2]
    real vtc = 0, vtc2 = 0, vts = 0, vts2 = 0;   // vt[N + 1], vt[N + 2]
    real vlc = 0, vlc2 = 0, vls = 0, vls2 = 0;   // vl[N + 1], vl[N + 2]
    for (int m = _M; m >= 0; --m) {   // m = M .. 0
      // Now Sc[m] = wc, Ss[m] = ws
      // Sc'[m] = wtc, Ss'[m] = wtc
      if (m) {
        real v, A, B;           // alpha[m], beta[m + 1]
        switch (_norm) {
        case FULL:
          v = root_[2] * root_[2 * m + 3] / root_[m + 1];
          A = cl * v * _uq;
          B = - v * root_[2 * m + 5] / (root_[8] * root_[m + 2]) * _uq2;
          break;
        case SCHMIDT:
          v = root_[2] * root_[2 * m + 1] / root_[m + 1];
          A = cl * v * _uq;
          B = - v * root_[2 * m + 3] / (root_[8] * root_[m + 2]) * _uq2;
          break;
        default:
          A = B = 0;
        }
        v = A * vc  + B * vc2  +  _wc[m] ; vc2  = vc ; vc  = v;
        v = A * vs  + B * vs2  +  _ws[m] ; vs2  = vs ; vs  = v;
        if (gradp) {
          v = A * vrc + B * vrc2 +  _wrc[m]; vrc2 = vrc; vrc = v;
          v = A * vrs + B * vrs2 +  _wrs[m]; vrs2 = vrs; vrs = v;
          v = A * vtc + B * vtc2 +  _wtc[m]; vtc2 = vtc; vtc = v;
          v = A * vts + B * vts2 +  _wts[m]; vts2 = vts; vts = v;
          v = A * vlc + B * vlc2 + m*_ws[m]; vlc2 = vlc; vlc = v;
          v = A * vls + B * vls2 - m*_wc[m]; vls2 = vls; vls = v;
        }
      } else {
        real A, B, qs;
        switch (_norm) {
        case FULL:
          A = root_[3] * _uq;       // F[1]/(q*cl) or F[1]/(q*sl)
          B = - root_[15]/2 * _uq2; // beta[1]/q
          break;
        case SCHMIDT:
          A = _uq;
          B = - root_[3]/2 * _uq2;
          break;
        default:
          A = B = 0;
        }
        qs = _q / SphericalEngine::scale();
        vc = qs * (_wc[m] + A * (cl * vc + sl * vs ) + B * vc2);
        if (gradp) {
          qs /= _r;
          // The components of the gradient in circular coordinates are
          // r: dV/dr
          // theta: 1/r * dV/dtheta
          // lambda: 1/(r*u) * dV/dlambda
          vrc =    - qs * (_wrc[m] + A * (cl * vrc + sl * vrs) + B * vrc2);
          vtc =      qs * (_wtc[m] + A * (cl * vtc + sl * vts) + B * vtc2);
          vlc = qs / _u * (          A * (cl * vlc + sl * vls) + B * vlc2);
        }
      }
    }

    if (gradp) {
      // Rotate into cartesian (geocentric) coordinates
      gradx = cl * (_u * vrc + _t * vtc) - sl * vlc;
      grady = sl * (_u * vrc + _t * vtc) + cl * vlc;
      gradz =           _t * vrc - _u * vtc                ;
    }
    return vc;
  }

} // namespace GeographicLib
