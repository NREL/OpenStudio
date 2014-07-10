/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/


#ifndef UTILITIES_GEOMETRY_PLANE_HPP
#define UTILITIES_GEOMETRY_PLANE_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Logger.hpp"

#include <boost/optional.hpp>

namespace openstudio{

  // forward declaration
  class Point3d;
  class Vector3d;

  /** Plane defines an infinite plane in 3D space.  The equation of a plane is 
   *  a*x + b*y + c*z = d, any point that satisfies this equation is on the plane.
   */
  class UTILITIES_API Plane{
  public:

    /// copy constructor
    Plane(const Plane& other);

    /// construct with point and outward normal, throws openstudio::Exception if outwardNormal has 0 length.
    Plane(const Point3d& point, const Vector3d& outwardNormal);

    /// construct with points, will fit least squares plane.
    /// attempts to align outward normal with that of the points (according to right hand rule).
    /// throws openstudio::Exception if cannot compute plane for these points.
    Plane(const std::vector<Point3d>& points);

    /// virtual destructor
    virtual ~Plane();

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
  typedef boost::optional<Plane> OptionalPlane;

  /// vector of Plane
  typedef std::vector<Plane> PlaneVector;

} // openstudio

#endif //UTILITIES_GEOMETRY_BOUNDINGBOX_HPP
