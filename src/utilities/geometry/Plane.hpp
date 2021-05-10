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
   *  a*x + b*y + c*z = d, any point that satisfies this equation is on the plane.
   */
class UTILITIES_API Plane
{
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

}  // namespace openstudio

#endif  //UTILITIES_GEOMETRY_BOUNDINGBOX_HPP
