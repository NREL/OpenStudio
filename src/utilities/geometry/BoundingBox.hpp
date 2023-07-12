/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_GEOMETRY_BOUNDINGBOX_HPP
#define UTILITIES_GEOMETRY_BOUNDINGBOX_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Logger.hpp"

#include <boost/optional.hpp>

#include <vector>

namespace openstudio {

// forward declaration
class Point3d;

/** BoundingBox is an axis aligned cuboid that can check for intersections with another BoundingBox.
   *  BoundingBox intersections require that both boxes are specified in the same coordinate system.
   */
class UTILITIES_API BoundingBox
{
 public:
  /// default constructor creates empty BoundingBox, will not intersect with anything
  BoundingBox() = default;

  /// add another BoundingBox
  void add(const BoundingBox& other);

  /// add a point to the BoundingBox
  void addPoint(const Point3d& point);

  /// add points to the BoundingBox
  void addPoints(const std::vector<Point3d>& points);

  /// test for intersection. Default tolerance is 1cm
  bool intersects(const BoundingBox& other, double tol = 0.01) const;

  bool isEmpty() const;

  boost::optional<double> minX() const;
  boost::optional<double> minY() const;
  boost::optional<double> minZ() const;

  boost::optional<double> maxX() const;
  boost::optional<double> maxY() const;
  boost::optional<double> maxZ() const;

  std::vector<Point3d> corners() const;

 private:
  REGISTER_LOGGER("utilities.BoundingBox");

  boost::optional<double> m_minX;
  boost::optional<double> m_minY;
  boost::optional<double> m_minZ;

  boost::optional<double> m_maxX;
  boost::optional<double> m_maxY;
  boost::optional<double> m_maxZ;
};

// optional BoundingBox
using OptionalBoundingBox = boost::optional<BoundingBox>;

// vector of BoundingBox
using BoundingBoxVector = std::vector<BoundingBox>;

}  // namespace openstudio

#endif  //UTILITIES_GEOMETRY_BOUNDINGBOX_HPP
