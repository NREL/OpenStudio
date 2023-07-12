/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_GEOMETRY_PLANE_HPP
#define UTILITIES_GEOMETRY_PLANE_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Logger.hpp"

#include <boost/optional.hpp>

#include <vector>

namespace openstudio {

// forward declaration
class Point3d;
class Vector3d;

/** Plane defines an infinite plane in 3D space.  The equation of a plane is
   *  a*x + b*y + c*z + d = 0, any point that satisfies this equation is on the plane.
   */
class UTILITIES_API Plane
{
 public:
  /// construct with point and outward normal, throws openstudio::Exception if outwardNormal has 0 length.
  Plane(const Point3d& point, const Vector3d& outwardNormal);

  /// construct with points, will fit least squares plane.
  /// attempts to align outward normal with that of the points (according to right hand rule).
  /// throws openstudio::Exception if cannot compute plane for these points.
  Plane(const std::vector<Point3d>& points);

  // Copy and move operators are implicitly declared (Rule of 1)
  // There's no need to check if the length of the normal is zero since we never allow another plane to not satisfy this condition
  // Plane(const Plane& other) = default;
  // Plane(Plane&& other) = default;
  // Plane& operator=(const Plane&) = default;
  // Plane& operator=(Plane&&) = default;
  // ~Plane() noexcept = default;

  /// get the outward normal of this plane
  Vector3d outwardNormal() const;

  /// is this plane parallel to the other plane
  /// true if absolute value of dot product of outward normals is >= (1-tol)
  bool parallel(const Plane& other, double tol = 0.001) const;

  /// is this plane equal to the other plane
  /// true if dot product of outward normals is >= (1-tol) and abs(d1-d2) <= tol
  bool equal(const Plane& other, double tol = 0.001) const;

  /// is this plane reverse equal to the other plane
  /// true if dot product of outward normals is <= (-1+tol) and abs(d1+d2) <= tol
  bool reverseEqual(const Plane& other, double tol = 0.001) const;

  /// flip the plane so outward normal is opposite
  Plane reversePlane() const;

  /// project a point to this plane
  Point3d project(const Point3d& point) const;

  /// project a vector of points to this plane
  std::vector<Point3d> project(const std::vector<Point3d>& points) const;

  /// returns true if the point is on this plane
  bool pointOnPlane(const Point3d& point, double tol = 0.001) const;

  /// coefficient a
  double a() const;

  /// coefficient b
  double b() const;

  /// coefficient c
  double c() const;

  /// coefficient d
  double d() const;

  // Returns any Point3d that's on this plane
  Point3d anyPointOnPlane() const;

  /** Project a directed ray towards a plane and returns the intersection point if any. The RayDirection should be shooting TOWARDS the plane
    * If enforceDirectionOfPlane is true, the ray direction and the plane outward normal must be facing each other */
  boost::optional<Point3d> rayIntersection(const Point3d& rayOrigin, const Vector3d& rayDirection, bool enforceDirectionOfPlane = false) const;

 private:
  // construct with coefficients
  Plane(double a, double b, double c, double d);

  REGISTER_LOGGER("utilities.Plane");

  double m_a;
  double m_b;
  double m_c;
  double m_d;
};

/// ostream operator
UTILITIES_API std::ostream& operator<<(std::ostream& os, const Plane& plane);

/// optional Plane
using OptionalPlane = boost::optional<Plane>;

/// vector of Plane
using PlaneVector = std::vector<Plane>;

}  // namespace openstudio

#endif  //UTILITIES_GEOMETRY_BOUNDINGBOX_HPP
