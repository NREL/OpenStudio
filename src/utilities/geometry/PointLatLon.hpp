/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_GEOMETRY_POINTLATLON_HPP
#define UTILITIES_GEOMETRY_POINTLATLON_HPP

#include "../UtilitiesAPI.hpp"
#include "../data/Vector.hpp"
#include "../core/Logger.hpp"

#include <vector>
#include <boost/optional.hpp>

// do not include anything from GeographicLib in header or expose in the API
namespace GeographicLib {
class LocalCartesian;
class UTMUPS;
}  // namespace GeographicLib

namespace openstudio {

class Point3d;

/* PointLatLon represents a point on the Earth's (reference ellipsoid) surface, an optional height parameter is available
  ** for points above or below the geoid surface.  Currently only the WGS84 ellipsoid is supported,
  ** other systems may be added in the future. When one PointLatLon is taken as an origin, it can be used to
  ** translate other PointLatLon's into either a local Cartesian coordinate system (centered on the origin, with z up and y oriented North)
  ** or the UTM (Universal Transverse Mercator) zone which contains the origin. The origin PointLatLon can be used to
  ** translate Point3d's in either local Cartesian or UTM coordinate systems back to lat/lon.
  */
class UTILITIES_API PointLatLon
{
 public:
  /// default constructor creates point at 0, 0, 0
  PointLatLon();

  /// constructor with lat, lon, and height
  PointLatLon(double lat, double lon, double height = 0);

  /// copy constructor
  PointLatLon(const PointLatLon& other);

  /// destructor
  ~PointLatLon();

  /// get lat
  double lat() const;

  /// get lon
  double lon() const;

  /// get height
  double height() const;

  PointLatLon& operator=(const openstudio::PointLatLon& other);

  /// check equality
  bool operator==(const PointLatLon& other) const;

  /// point minus another point is distance along geodesic line
  double operator-(const PointLatLon& other) const;

  /// converts a PointLatLon to a Point3d in a Local Cartesian frame centered on this with y oriented North and z oriented up
  Point3d toLocalCartesian(const PointLatLon& point) const;
  std::vector<Point3d> toLocalCartesian(const std::vector<PointLatLon>& points) const;

  /// converts a Point3d in Local Cartesian coordinate system centered on this to LatLon
  PointLatLon fromLocalCartesian(const Point3d& point) const;
  std::vector<PointLatLon> fromLocalCartesian(const std::vector<Point3d>& points) const;

  /// gets the UTM zone for this point
  int utmZone() const;

  /// converts a PointLatLon to a Point3d in UTM centered on this
  Point3d toUTM(const PointLatLon& point) const;
  std::vector<Point3d> toUTM(const std::vector<PointLatLon>& points) const;

  /// converts a Point3d in UTM centered on this to LatLon
  PointLatLon fromUTM(const Point3d& point) const;
  std::vector<PointLatLon> fromUTM(const std::vector<Point3d>& points) const;

 private:
  REGISTER_LOGGER("utilities.PointLatLon");
  Vector m_storage;

  void initLocalCartesianConverter() const;

  mutable std::unique_ptr<GeographicLib::LocalCartesian> m_localCartesianConverter;
};

/// ostream operator
UTILITIES_API std::ostream& operator<<(std::ostream& os, const PointLatLon& point);

/// ostream operator
UTILITIES_API std::ostream& operator<<(std::ostream& os, const std::vector<PointLatLon>& pointVector);

// optional PointLatLon
typedef boost::optional<PointLatLon> OptionalPointLatLon;

// vector of PointLatLon
typedef std::vector<PointLatLon> PointLatLonVector;

/// compute distance in meters between two points on the Earth's surface
/// lat and lon are specified in degrees
UTILITIES_API double getDistanceLatLon(double lat1, double lon1, double lat2, double lon2);

}  // namespace openstudio

#endif  //UTILITIES_GEOMETRY_POINTLATLON_HPP
