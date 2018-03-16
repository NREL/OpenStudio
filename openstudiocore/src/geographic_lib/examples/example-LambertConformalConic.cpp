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
#include <exception>
#include <GeographicLib/LambertConformalConic.hpp>

using namespace std;
using namespace GeographicLib;

int main() {
  try {
    // Define the Pennsylvania South state coordinate system EPSG:3364
    // http://www.spatialreference.org/ref/epsg/3364/
    const double
      a = Constants::WGS84_a(),
      f = 1/298.257222101,                      // GRS80
      lat1 = 40 + 58/60.0, lat2 = 39 + 56/60.0, // standard parallels
      k1 = 1,                                   // scale
      lat0 = 39 + 20/60.0, lon0 =-77 - 45/60.0, // origin
      fe = 600000, fn = 0;                      // false easting and northing
    // Set up basic projection
    const LambertConformalConic PASouth(a, f, lat1, lat2, k1);
    double x0, y0;
    // Transform origin point
    PASouth.Forward(lon0, lat0, lon0, x0, y0);
    x0 -= fe; y0 -= fn;
    {
      // Sample conversion from geodetic to PASouth grid
      double lat = 39.95, lon = -75.17;    // Philadelphia
      double x, y;
      PASouth.Forward(lon0, lat, lon, x, y);
      x -= x0; y -= y0;
      cout << x << " " << y << "\n";
    }
    {
      // Sample conversion from PASouth grid to geodetic
      double x = 820e3, y = 72e3;
      double lat, lon;
      x += x0; y += y0;
      PASouth.Reverse(lon0, x, y, lat, lon);
      cout << lat << " " << lon << "\n";
    }
  }
  catch (const exception& e) {
    cerr << "Caught exception: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
