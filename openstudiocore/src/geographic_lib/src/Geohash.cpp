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

#include <GeographicLib/Geohash.hpp>
#include <GeographicLib/Utility.hpp>

namespace GeographicLib {

  using namespace std;

  const string Geohash::lcdigits_ = "0123456789bcdefghjkmnpqrstuvwxyz";
  const string Geohash::ucdigits_ = "0123456789BCDEFGHJKMNPQRSTUVWXYZ";

  void Geohash::Forward(real lat, real lon, int len, std::string& geohash) {
    static const real shift = pow(real(2), 45);
    static const real loneps = 180 / shift;
    static const real lateps =  90 / shift;
    if (abs(lat) > 90)
      throw GeographicErr("Latitude " + Utility::str(lat)
                          + "d not in [-90d, 90d]");
    if (Math::isnan(lat) || Math::isnan(lon)) {
      geohash = "invalid";
      return;
    }
    if (lat == 90) lat -= lateps / 2;
    lon = Math::AngNormalize(lon); // lon in [-180,180)
    // lon/loneps in [-2^45,2^45); lon/loneps + shift in [0,2^46)
    // similarly for lat
    len = max(0, min(int(maxlen_), len));
    unsigned long long
      ulon = (unsigned long long)(floor(lon/loneps) + shift),
      ulat = (unsigned long long)(floor(lat/lateps) + shift);
    char geohash1[maxlen_];
    unsigned byte = 0;
    for (unsigned i = 0; i < 5 * unsigned(len);) {
      if ((i & 1) == 0) {
        byte = (byte << 1) + unsigned((ulon & mask_) != 0);
        ulon <<= 1;
      } else {
        byte = (byte << 1) + unsigned((ulat & mask_) != 0);
        ulat <<= 1;
      }
      ++i;
      if (i % 5 == 0) {
        geohash1[(i/5)-1] = lcdigits_[byte];
        byte = 0;
      }
    }
    geohash.resize(len);
    copy(geohash1, geohash1 + len, geohash.begin());
  }

  void Geohash::Reverse(const std::string& geohash, real& lat, real& lon,
                        int& len, bool centerp) {
    static const real shift = pow(real(2), 45);
    static const real loneps = 180 / shift;
    static const real lateps =  90 / shift;
    int len1 = min(int(maxlen_), int(geohash.length()));
    if (len1 >= 3 &&
        ((toupper(geohash[0]) == 'I' &&
          toupper(geohash[1]) == 'N' &&
          toupper(geohash[2]) == 'V') ||
         // Check A first because it is not in a standard geohash
         (toupper(geohash[1]) == 'A' &&
          toupper(geohash[0]) == 'N' &&
          toupper(geohash[2]) == 'N'))) {
      lat = lon = Math::NaN();
      return;
    }
    unsigned long long ulon = 0, ulat = 0;
    for (unsigned k = 0, j = 0; k < unsigned(len1); ++k) {
      int byte = Utility::lookup(ucdigits_, geohash[k]);
      if (byte < 0)
        throw GeographicErr("Illegal character in geohash " + geohash);
      for (unsigned m = 16; m; m >>= 1) {
        if (j == 0)
          ulon = (ulon << 1) + unsigned((byte & m) != 0);
        else
          ulat = (ulat << 1) + unsigned((byte & m) != 0);
        j ^= 1;
      }
    }
    ulon <<= 1; ulat <<= 1;
    if (centerp) {
      ulon += 1;
      ulat += 1;
    }
    int s = 5 * (maxlen_ - len1);
    ulon <<=     (s / 2);
    ulat <<= s - (s / 2);
    lon = ulon * loneps - 180;
    lat = ulat * lateps - 90;
    len = len1;
  }

} // namespace GeographicLib
