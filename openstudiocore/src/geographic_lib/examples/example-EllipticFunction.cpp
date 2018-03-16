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
#include <cmath>
#include <GeographicLib/Math.hpp>
#include <GeographicLib/EllipticFunction.hpp>

using namespace std;
using namespace GeographicLib;

int main() {
  try {
    EllipticFunction ell(0.1);  // parameter m = 0.1
    // See Abramowitz and Stegun, table 17.1
    cout << ell.K() << " " << ell.E() << "\n";
    double phi = 20 * Math::degree();
    // See Abramowitz and Stegun, table 17.6 with
    // alpha = asin(sqrt(m)) = 18.43 deg and phi = 20 deg
    cout << ell.E(phi) << " "
         << ell.E(sin(phi), cos(phi), sqrt(1 - ell.k2() * Math::sq(sin(phi))))
         << "\n";
    // See Carlson 1995, Sec 3.
    cout << fixed << setprecision(16)
         << "RF(1,2,0)      = " << EllipticFunction::RF(1,2)      << "\n"
         << "RF(2,3,4)      = " << EllipticFunction::RF(2,3,4)    << "\n"
         << "RC(0,1/4)      = " << EllipticFunction::RC(0,0.25)   << "\n"
         << "RC(9/4,2)      = " << EllipticFunction::RC(2.25,2)   << "\n"
         << "RC(1/4,-2)     = " << EllipticFunction::RC(0.25,-2)  << "\n"
         << "RJ(0,1,2,3)    = " << EllipticFunction::RJ(0,1,2,3)  << "\n"
         << "RJ(2,3,4,5)    = " << EllipticFunction::RJ(2,3,4,5)  << "\n"
         << "RD(0,2,1)      = " << EllipticFunction::RD(0,2,1)    << "\n"
         << "RD(2,3,4)      = " << EllipticFunction::RD(2,3,4)    << "\n"
         << "RG(0,16,16)    = " << EllipticFunction::RG(16,16)    << "\n"
         << "RG(2,3,4)      = " << EllipticFunction::RG(2,3,4)    << "\n"
         << "RG(0,0.0796,4) = " << EllipticFunction::RG(0.0796,4) << "\n";
  }
  catch (const GeographicErr& e) {
    cout << "Caught exception: " << e.what() << "\n";
  }
  return 0;
}
