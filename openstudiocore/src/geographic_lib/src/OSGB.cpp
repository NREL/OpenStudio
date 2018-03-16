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

#include <GeographicLib/OSGB.hpp>
#include <GeographicLib/Utility.hpp>

namespace GeographicLib {

  using namespace std;

  const string OSGB::letters_ = "ABCDEFGHJKLMNOPQRSTUVWXYZ";
  const string OSGB::digits_ = "0123456789";

  const TransverseMercator& OSGB::OSGBTM() {
    static const TransverseMercator osgbtm(MajorRadius(), Flattening(),
                                           CentralScale());
    return osgbtm;
  }

  Math::real OSGB::computenorthoffset() {
    real x, y;
    static const real northoffset =
      ( OSGBTM().Forward(real(0), OriginLatitude(), real(0), x, y),
        FalseNorthing() - y );
    return northoffset;
  }

  void OSGB::GridReference(real x, real y, int prec, std::string& gridref) {
    CheckCoords(x, y);
    if (!(prec >= 0 && prec <= maxprec_))
      throw GeographicErr("OSGB precision " + Utility::str(prec)
                          + " not in [0, "
                          + Utility::str(int(maxprec_)) + "]");
    if (Math::isnan(x) || Math::isnan(y)) {
      gridref = "INVALID";
      return;
    }
    char grid[2 + 2 * maxprec_];
    int
      xh = int(floor(x / tile_)),
      yh = int(floor(y / tile_));
    real
      xf = x - tile_ * xh,
      yf = y - tile_ * yh;
    xh += tileoffx_;
    yh += tileoffy_;
    int z = 0;
    grid[z++] = letters_[(tilegrid_ - (yh / tilegrid_) - 1)
                        * tilegrid_ + (xh / tilegrid_)];
    grid[z++] = letters_[(tilegrid_ - (yh % tilegrid_) - 1)
                        * tilegrid_ + (xh % tilegrid_)];
    real mult = pow(real(base_), max(tilelevel_ - prec, 0));
    int
      ix = int(floor(xf / mult)),
      iy = int(floor(yf / mult));
    for (int c = min(prec, int(tilelevel_)); c--;) {
      grid[z + c] = digits_[ ix % base_ ];
      ix /= base_;
      grid[z + c + prec] = digits_[ iy % base_ ];
      iy /= base_;
    }
    if (prec > tilelevel_) {
      xf -= floor(xf / mult);
      yf -= floor(yf / mult);
      mult = pow(real(base_), prec - tilelevel_);
      ix = int(floor(xf * mult));
      iy = int(floor(yf * mult));
      for (int c = prec - tilelevel_; c--;) {
        grid[z + c + tilelevel_] = digits_[ ix % base_ ];
        ix /= base_;
        grid[z + c + tilelevel_ + prec] = digits_[ iy % base_ ];
        iy /= base_;
      }
    }
    int mlen = z + 2 * prec;
    gridref.resize(mlen);
    copy(grid, grid + mlen, gridref.begin());
  }

  void OSGB::GridReference(const std::string& gridref,
                           real& x, real& y, int& prec,
                           bool centerp) {
    int
      len = int(gridref.size()),
      p = 0;
    if (len >= 2 &&
        toupper(gridref[0]) == 'I' &&
        toupper(gridref[1]) == 'N') {
      x = y = Math::NaN();
      prec = -2;                // For compatibility with MGRS::Reverse.
      return;
    }
    char grid[2 + 2 * maxprec_];
    for (int i = 0; i < len; ++i) {
      if (!isspace(gridref[i])) {
        if (p >= 2 + 2 * maxprec_)
          throw GeographicErr("OSGB string " + gridref + " too long");
        grid[p++] = gridref[i];
      }
    }
    len = p;
    p = 0;
    if (len < 2)
      throw GeographicErr("OSGB string " + gridref + " too short");
    if (len % 2)
      throw GeographicErr("OSGB string " + gridref +
                          " has odd number of characters");
    int
      xh = 0,
      yh = 0;
    while (p < 2) {
      int i = Utility::lookup(letters_, grid[p++]);
      if (i < 0)
        throw GeographicErr("Illegal prefix character " + gridref);
      yh = yh * tilegrid_ + tilegrid_ - (i / tilegrid_) - 1;
      xh = xh * tilegrid_ + (i % tilegrid_);
    }
    xh -= tileoffx_;
    yh -= tileoffy_;

    int prec1 = (len - p)/2;
    real
      unit = tile_,
      x1 = unit * xh,
      y1 = unit * yh;
    for (int i = 0; i < prec1; ++i) {
      unit /= base_;
      int
        ix = Utility::lookup(digits_, grid[p + i]),
        iy = Utility::lookup(digits_, grid[p + i + prec1]);
      if (ix < 0 || iy < 0)
        throw GeographicErr("Encountered a non-digit in " + gridref);
      x1 += unit * ix;
      y1 += unit * iy;
    }
    if (centerp) {
      x1 += unit/2;
      y1 += unit/2;
    }
    x = x1;
    y = y1;
    prec = prec1;
  }

  void OSGB::CheckCoords(real x, real y) {
    // Limits are all multiples of 100km and are all closed on the lower end
    // and open on the upper end -- and this is reflected in the error
    // messages.  NaNs are let through.
    if (x < minx_ || x >= maxx_)
      throw GeographicErr("Easting " + Utility::str(int(floor(x/1000)))
                          + "km not in OSGB range ["
                          + Utility::str(minx_/1000) + "km, "
                          + Utility::str(maxx_/1000) + "km)");
    if (y < miny_ || y >= maxy_)
      throw GeographicErr("Northing " + Utility::str(int(floor(y/1000)))
                          + "km not in OSGB range ["
                          + Utility::str(miny_/1000) + "km, "
                          + Utility::str(maxy_/1000) + "km)");
  }

} // namespace GeographicLib
