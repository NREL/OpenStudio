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

#include <GeographicLib/MagneticCircle.hpp>
#include <fstream>
#include <sstream>
#include <GeographicLib/Geocentric.hpp>

namespace GeographicLib {

  using namespace std;

  void MagneticCircle::Field(real lon, bool diffp,
                             real& Bx, real& By, real& Bz,
                             real& Bxt, real& Byt, real& Bzt) const {
    real clam, slam;
    Math::sincosd(lon, slam, clam);
    real M[Geocentric::dim2_];
    Geocentric::Rotation(_sphi, _cphi, slam, clam, M);
    real BX0, BY0, BZ0, BX1, BY1, BZ1; // Components in geocentric basis
    real BXc = 0, BYc = 0, BZc = 0;
    _circ0(clam, slam, BX0, BY0, BZ0);
    _circ1(clam, slam, BX1, BY1, BZ1);
    if (_constterm)
      _circ2(clam, slam, BXc, BYc, BZc);
    if (_interpolate) {
      BX1 = (BX1 - BX0) / _dt0;
      BY1 = (BY1 - BY0) / _dt0;
      BZ1 = (BZ1 - BZ0) / _dt0;
    }
    BX0 += _t1 * BX1 + BXc;
    BY0 += _t1 * BY1 + BYc;
    BZ0 += _t1 * BZ1 + BZc;
    if (diffp) {
      Geocentric::Unrotate(M, BX1, BY1, BZ1, Bxt, Byt, Bzt);
      Bxt *= - _a;
      Byt *= - _a;
      Bzt *= - _a;
    }
    Geocentric::Unrotate(M, BX0, BY0, BZ0, Bx, By, Bz);
    Bx *= - _a;
    By *= - _a;
    Bz *= - _a;
  }

} // namespace GeographicLib
