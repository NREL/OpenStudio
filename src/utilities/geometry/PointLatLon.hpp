/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

  // Rule of 5/6 because the assignment operators need to reset the LocalCartesian converter and the LocalCartesian converter has a deleted copy
  PointLatLon(const PointLatLon& other);
  PointLatLon(PointLatLon&& other) noexcept;
  PointLatLon& operator=(const PointLatLon& other);
  PointLatLon& operator=(PointLatLon&& other) noexcept;
  ~PointLatLon() noexcept;

  /// get lat
  double lat() const;

  /// get lon
  double lon() const;

  /// get height
  double height() const;

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
using OptionalPointLatLon = boost::optional<PointLatLon>;

// vector of PointLatLon
using PointLatLonVector = std::vector<PointLatLon>;

/// compute distance in meters between two points on the Earth's surface
/// lat and lon are specified in degrees
UTILITIES_API double getDistanceLatLon(double lat1, double lon1, double lat2, double lon2);

}  // namespace openstudio

#endif  //UTILITIES_GEOMETRY_POINTLATLON_HPP
