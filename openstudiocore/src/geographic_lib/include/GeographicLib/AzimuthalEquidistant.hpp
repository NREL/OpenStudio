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

#if !defined(GEOGRAPHICLIB_AZIMUTHALEQUIDISTANT_HPP)
#define GEOGRAPHICLIB_AZIMUTHALEQUIDISTANT_HPP 1

#include <GeographicLib/Geodesic.hpp>
#include <GeographicLib/Constants.hpp>

namespace GeographicLib {

  /**
   * \brief Azimuthal equidistant projection
   *
   * Azimuthal equidistant projection centered at an arbitrary position on the
   * ellipsoid.  For a point in projected space (\e x, \e y), the geodesic
   * distance from the center position is hypot(\e x, \e y) and the azimuth of
   * the geodesic from the center point is atan2(\e x, \e y).  The Forward and
   * Reverse methods also return the azimuth \e azi of the geodesic at (\e x,
   * \e y) and reciprocal scale \e rk in the azimuthal direction which,
   * together with the basic properties of the projection, serve to specify
   * completely the local affine transformation between geographic and
   * projected coordinates.
   *
   * The conversions all take place using a Geodesic object (by default
   * Geodesic::WGS84()).  For more information on geodesics see \ref geodesic.
   *
   * Example of use:
   * \include example-AzimuthalEquidistant.cpp
   *
   * <a href="GeodesicProj.1.html">GeodesicProj</a> is a command-line utility
   * providing access to the functionality of AzimuthalEquidistant, Gnomonic,
   * and CassiniSoldner.
   **********************************************************************/

  class GEOGRAPHICLIB_EXPORT AzimuthalEquidistant {
  private:
    typedef Math::real real;
    real eps_;
    Geodesic _earth;
  public:

    /**
     * Constructor for AzimuthalEquidistant.
     *
     * @param[in] earth the Geodesic object to use for geodesic calculations.
     *   By default this uses the WGS84 ellipsoid.
     **********************************************************************/
    explicit AzimuthalEquidistant(const Geodesic& earth = Geodesic::WGS84());

    /**
     * Forward projection, from geographic to azimuthal equidistant.
     *
     * @param[in] lat0 latitude of center point of projection (degrees).
     * @param[in] lon0 longitude of center point of projection (degrees).
     * @param[in] lat latitude of point (degrees).
     * @param[in] lon longitude of point (degrees).
     * @param[out] x easting of point (meters).
     * @param[out] y northing of point (meters).
     * @param[out] azi azimuth of geodesic at point (degrees).
     * @param[out] rk reciprocal of azimuthal scale at point.
     *
     * \e lat0 and \e lat should be in the range [&minus;90&deg;, 90&deg;].
     * The scale of the projection is 1 in the "radial" direction, \e azi
     * clockwise from true north, and is 1/\e rk in the direction perpendicular
     * to this.  A call to Forward followed by a call to Reverse will return
     * the original (\e lat, \e lon) (to within roundoff).
     **********************************************************************/
    void Forward(real lat0, real lon0, real lat, real lon,
                 real& x, real& y, real& azi, real& rk) const;

    /**
     * Reverse projection, from azimuthal equidistant to geographic.
     *
     * @param[in] lat0 latitude of center point of projection (degrees).
     * @param[in] lon0 longitude of center point of projection (degrees).
     * @param[in] x easting of point (meters).
     * @param[in] y northing of point (meters).
     * @param[out] lat latitude of point (degrees).
     * @param[out] lon longitude of point (degrees).
     * @param[out] azi azimuth of geodesic at point (degrees).
     * @param[out] rk reciprocal of azimuthal scale at point.
     *
     * \e lat0 should be in the range [&minus;90&deg;, 90&deg;].  \e lat will
     * be in the range [&minus;90&deg;, 90&deg;] and \e lon will be in the
     * range [&minus;180&deg;, 180&deg;).  The scale of the projection is 1 in
     * the "radial" direction, \e azi clockwise from true north, and is 1/\e rk
     * in the direction perpendicular to this.  A call to Reverse followed by a
     * call to Forward will return the original (\e x, \e y) (to roundoff) only
     * if the geodesic to (\e x, \e y) is a shortest path.
     **********************************************************************/
    void Reverse(real lat0, real lon0, real x, real y,
                 real& lat, real& lon, real& azi, real& rk) const;

    /**
     * AzimuthalEquidistant::Forward without returning the azimuth and scale.
     **********************************************************************/
    void Forward(real lat0, real lon0, real lat, real lon,
                 real& x, real& y) const {
      real azi, rk;
      Forward(lat0, lon0, lat, lon, x, y, azi, rk);
    }

    /**
     * AzimuthalEquidistant::Reverse without returning the azimuth and scale.
     **********************************************************************/
    void Reverse(real lat0, real lon0, real x, real y,
                 real& lat, real& lon) const {
      real azi, rk;
      Reverse(lat0, lon0, x, y, lat, lon, azi, rk);
    }

    /** \name Inspector functions
     **********************************************************************/
    ///@{
    /**
     * @return \e a the equatorial radius of the ellipsoid (meters).  This is
     *   the value inherited from the Geodesic object used in the constructor.
     **********************************************************************/
    Math::real MajorRadius() const { return _earth.MajorRadius(); }

    /**
     * @return \e f the flattening of the ellipsoid.  This is the value
     *   inherited from the Geodesic object used in the constructor.
     **********************************************************************/
    Math::real Flattening() const { return _earth.Flattening(); }
    ///@}

    /// \cond SKIP
    /**
     * <b>DEPRECATED</b>
     * @return \e r the inverse flattening of the ellipsoid.
     **********************************************************************/
    Math::real InverseFlattening() const
    { return _earth.InverseFlattening(); }
    /// \endcond
  };

} // namespace GeographicLib

#endif  // GEOGRAPHICLIB_AZIMUTHALEQUIDISTANT_HPP
