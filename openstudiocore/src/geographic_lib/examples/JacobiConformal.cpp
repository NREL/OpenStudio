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

#include <iostream>
#include <iomanip>
#include <GeographicLib/Utility.hpp>
#include "JacobiConformal.hpp"

using namespace std;
using namespace GeographicLib;

int main() {
  Utility::set_digits();
  // These parameters were derived from the EGM2008 geoid; see 2011-07-04
  // E-mail to PROJ.4 list, "Analyzing the bumps in the EGM2008 geoid".  The
  // longitude of the major axis is -15.  These are close to the values given
  // by Milan Bursa, Vladimira Fialova, "Parameters of the Earth's tri-axial
  // level ellipsoid", Studia Geophysica et Geodaetica 37(1), 1-13 (1993):
  //
  //    longitude of major axis = -14.93 +/- 0.05
  //    a = 6378171.36 +/- 0.30
  //    a/(a-c) = 297.7738 +/- 0.0003
  //    a/(a-b) = 91449 +/- 60
  // which gives: a = 6378171.36, b = 6378101.61, c = 6356751.84
  Math::real a = 6378137+35, b = 6378137-35, c = 6356752;
  JacobiConformal jc(a, b, c, a-b, b-c);
  cout  << fixed << setprecision(1)
        << "Ellipsoid parameters: a = "
        << a << ", b = " << b << ", c = " << c << "\n"
        << setprecision(10)
        << "Quadrants: x = " << jc.x() << ", y = " << jc.y() << "\n";
  cout << "Coordinates (angle x y) in degrees:\n";
  for (int i = 0; i <= 90; i += 5) {
    Math::real omg = i, bet = i;
    cout << i << " " << jc.x(omg) << " " << jc.y(bet) << "\n";
  }
}
