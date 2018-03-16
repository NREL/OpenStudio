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

#if !defined(GEOGRAPHICLIB_GARS_HPP)
#define GEOGRAPHICLIB_GARS_HPP 1

#include <GeographicLib/Constants.hpp>

#if defined(_MSC_VER)
// Squelch warnings about dll vs string
#  pragma warning (push)
#  pragma warning (disable: 4251)
#endif

namespace GeographicLib {

  /**
   * \brief Conversions for the Global Area Reference System (GARS)
   *
   * The Global Area Reference System is described in
   * - https://en.wikipedia.org/wiki/Global_Area_Reference_System
   * - http://earth-info.nga.mil/GandG/coordsys/grids/gars.html
   * .
   * It provides a compact string representation of a geographic area
   * (expressed as latitude and longitude).  The classes Georef and Geohash
   * implement similar compact representations.
   *
   * Example of use:
   * \include example-GARS.cpp
   **********************************************************************/

  class GEOGRAPHICLIB_EXPORT GARS {
  private:
    typedef Math::real real;
    static const std::string digits_;
    static const std::string letters_;
    enum {
      lonorig_ = -180,          // Origin for longitude
      latorig_ = -90,           // Origin for latitude
      baselon_ = 10,            // Base for longitude tiles
      baselat_ = 24,            // Base for latitude tiles
      lonlen_ = 3,
      latlen_ = 2,
      baselen_ = lonlen_ + latlen_,
      mult1_ = 2,               // base precision = 1/2 degree
      mult2_ = 2,               // 6th char gives 2x more precision
      mult3_ = 3,               // 7th char gives 3x more precision
      m_ = mult1_ * mult2_ * mult3_,
      maxprec_ = 2,
      maxlen_ = baselen_ + maxprec_,
    };
    GARS();                     // Disable constructor

  public:

    /**
     * Convert from geographic coordinates to GARS.
     *
     * @param[in] lat latitude of point (degrees).
     * @param[in] lon longitude of point (degrees).
     * @param[in] prec the precision of the resulting GARS.
     * @param[out] gars the GARS string.
     * @exception GeographicErr if \e lat is not in [&minus;90&deg;,
     *   90&deg;].
     * @exception std::bad_alloc if memory for \e gars can't be allocated.
     *
     * \e prec specifies the precision of \e gars as follows:
     * - \e prec = 0 (min), 30' precision, e.g., 006AG;
     * - \e prec = 1, 15' precision, e.g., 006AG3;
     * - \e prec = 2 (max), 5' precision, e.g., 006AG39.
     *
     * If \e lat or \e lon is NaN, then \e gars is set to "INVALID".
     **********************************************************************/
    static void Forward(real lat, real lon, int prec, std::string& gars);

    /**
     * Convert from GARS to geographic coordinates.
     *
     * @param[in] gars the GARS.
     * @param[out] lat latitude of point (degrees).
     * @param[out] lon longitude of point (degrees).
     * @param[out] prec the precision of \e gars.
     * @param[in] centerp if true (the default) return the center of the
     *   \e gars, otherwise return the south-west corner.
     * @exception GeographicErr if \e gars is illegal.
     *
     * The case of the letters in \e gars is ignored.  \e prec is in the range
     * [0, 2] and gives the precision of \e gars as follows:
     * - \e prec = 0 (min), 30' precision, e.g., 006AG;
     * - \e prec = 1, 15' precision, e.g., 006AG3;
     * - \e prec = 2 (max), 5' precision, e.g., 006AG39.
     *
     * If the first 3 characters of \e gars are "INV", then \e lat and \e lon
     * are set to NaN and \e prec is unchanged.
     **********************************************************************/
    static void Reverse(const std::string& gars, real& lat, real& lon,
                        int& prec, bool centerp = true);

    /**
     * The angular resolution of a GARS.
     *
     * @param[in] prec the precision of the GARS.
     * @return the latitude-longitude resolution (degrees).
     *
     * Internally, \e prec is first put in the range [0, 2].
     **********************************************************************/
    static Math::real Resolution(int prec) {
      return 1/real(prec <= 0 ? mult1_ : (prec == 1 ? mult1_ * mult2_ :
                                          mult1_ * mult2_ * mult3_));
    }

    /**
     * The GARS precision required to meet a given geographic resolution.
     *
     * @param[in] res the minimum of resolution in latitude and longitude
     *   (degrees).
     * @return GARS precision.
     *
     * The returned length is in the range [0, 2].
     **********************************************************************/
    static int Precision(real res) {
      using std::abs; res = abs(res);
      for (int prec = 0; prec < maxprec_; ++prec)
        if (Resolution(prec) <= res)
          return prec;
      return maxprec_;
    }

  };

} // namespace GeographicLib

#if defined(_MSC_VER)
#  pragma warning (pop)
#endif

#endif  // GEOGRAPHICLIB_GARS_HPP
