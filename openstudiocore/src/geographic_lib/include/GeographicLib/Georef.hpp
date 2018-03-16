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

#if !defined(GEOGRAPHICLIB_GEOREF_HPP)
#define GEOGRAPHICLIB_GEOREF_HPP 1

#include <GeographicLib/Constants.hpp>

#if defined(_MSC_VER)
// Squelch warnings about dll vs string
#  pragma warning (push)
#  pragma warning (disable: 4251)
#endif

namespace GeographicLib {

  /**
   * \brief Conversions for the World Geographic Reference System (georef)
   *
   * The World Geographic Reference System is described in
   * - https://en.wikipedia.org/wiki/Georef
   * - http://earth-info.nga.mil/GandG/coordsys/grids/georef.pdf
   * .
   * It provides a compact string representation of a geographic area
   * (expressed as latitude and longitude).  The classes GARS and Geohash
   * implement similar compact representations.
   *
   * Example of use:
   * \include example-Georef.cpp
   **********************************************************************/

  class GEOGRAPHICLIB_EXPORT Georef {
  private:
    typedef Math::real real;
    static const std::string digits_;
    static const std::string lontile_;
    static const std::string lattile_;
    static const std::string degrees_;
    enum {
      tile_ = 15,               // The size of tile in degrees
      lonorig_ = -180,          // Origin for longitude
      latorig_ = -90,           // Origin for latitude
      base_ = 10,               // Base for minutes
      baselen_ = 4,
      maxprec_ = 11,            // approximately equivalent to MGRS class
      maxlen_ = baselen_ + 2 * maxprec_,
    };
    Georef();                     // Disable constructor

  public:

    /**
     * Convert from geographic coordinates to georef.
     *
     * @param[in] lat latitude of point (degrees).
     * @param[in] lon longitude of point (degrees).
     * @param[in] prec the precision of the resulting georef.
     * @param[out] georef the georef string.
     * @exception GeographicErr if \e lat is not in [&minus;90&deg;,
     *   90&deg;].
     * @exception std::bad_alloc if memory for \e georef can't be allocated.
     *
     * \e prec specifies the precision of \e georef as follows:
     * - \e prec = &minus;1 (min), 15&deg;
     * - \e prec = 0, 1&deg;
     * - \e prec = 1, converted to \e prec = 2
     * - \e prec = 2, 1'
     * - \e prec = 3, 0.1'
     * - \e prec = 4, 0.01'
     * - \e prec = 5, 0.001'
     * - &hellip;
     * - \e prec = 11 (max), 10<sup>&minus;9</sup>'
     *
     * If \e lat or \e lon is NaN, then \e georef is set to "INVALID".
     **********************************************************************/
    static void Forward(real lat, real lon, int prec, std::string& georef);

    /**
     * Convert from Georef to geographic coordinates.
     *
     * @param[in] georef the Georef.
     * @param[out] lat latitude of point (degrees).
     * @param[out] lon longitude of point (degrees).
     * @param[out] prec the precision of \e georef.
     * @param[in] centerp if true (the default) return the center
     *   \e georef, otherwise return the south-west corner.
     * @exception GeographicErr if \e georef is illegal.
     *
     * The case of the letters in \e georef is ignored.  \e prec is in the
     * range [&minus;1, 11] and gives the precision of \e georef as follows:
     * - \e prec = &minus;1 (min), 15&deg;
     * - \e prec = 0, 1&deg;
     * - \e prec = 1, not returned
     * - \e prec = 2, 1'
     * - \e prec = 3, 0.1'
     * - \e prec = 4, 0.01'
     * - \e prec = 5, 0.001'
     * - &hellip;
     * - \e prec = 11 (max), 10<sup>&minus;9</sup>'
     *
     * If the first 3 characters of \e georef are "INV", then \e lat and \e lon
     * are set to NaN and \e prec is unchanged.
     **********************************************************************/
    static void Reverse(const std::string& georef, real& lat, real& lon,
                        int& prec, bool centerp = true);

    /**
     * The angular resolution of a Georef.
     *
     * @param[in] prec the precision of the Georef.
     * @return the latitude-longitude resolution (degrees).
     *
     * Internally, \e prec is first put in the range [&minus;1, 11].
     **********************************************************************/
    static Math::real Resolution(int prec) {
      if (prec < 1)
        return real(prec < 0 ? 15 : 1);
      else {
        using std::pow;
        // Treat prec = 1 as 2.
        prec = (std::max)(2, (std::min)(int(maxprec_), prec));
        return 1/(60 * pow(real(base_), prec - 2));
      }
    }

    /**
     * The Georef precision required to meet a given geographic resolution.
     *
     * @param[in] res the minimum of resolution in latitude and longitude
     *   (degrees).
     * @return Georef precision.
     *
     * The returned length is in the range [0, 11].
     **********************************************************************/
    static int Precision(real res) {
      using std::abs; res = abs(res);
      for (int prec = 0; prec < maxprec_; ++prec) {
        if (prec == 1)
          continue;
        if (Resolution(prec) <= res)
          return prec;
      }
      return maxprec_;
    }

  };

} // namespace GeographicLib

#if defined(_MSC_VER)
#  pragma warning (pop)
#endif

#endif  // GEOGRAPHICLIB_GEOREF_HPP
