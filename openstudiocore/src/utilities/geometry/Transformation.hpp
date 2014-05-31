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

#ifndef UTILITIES_GEOMETRY_TRANSFORMATION_HPP
#define UTILITIES_GEOMETRY_TRANSFORMATION_HPP

#include "../UtilitiesAPI.hpp"
#include "../data/Matrix.hpp"
#include "../data/Vector.hpp"
#include "../core/Logger.hpp"

#include <vector>
#include <boost/optional.hpp>

namespace openstudio{

  // forward declaration
  class Point3d;
  class Vector3d;
  class Plane;
  class BoundingBox;
  class EulerAngles;

  class UTILITIES_API Transformation{
  public:

    /// default constructor creates identity transformation
    Transformation();

    /// constructor from storage, asserts matrix is 4x4
    Transformation(const Matrix& matrix);

    /// constructor from storage, asserts vector is size 16
    Transformation(const Vector& vector);

    /// copy constructor
    Transformation(const Transformation& other);

    /// rotation about origin defined by axis and angle (radians)
    static Transformation rotation(const Vector3d& axis, double radians);

    /// rotation about point defined by axis and angle (radians)
    static Transformation rotation(const Point3d& origin, const Vector3d& axis, double radians);

    /// rotation specified by Euler angles
    static Transformation rotation(const EulerAngles& angles);

    /// translation along vector
    static Transformation translation(const Vector3d& translation);

    /// transforms system with z' to regular system
    /// will try to align y' with z, but if that fails will align y' with y
    static Transformation alignZPrime(const Vector3d& zPrime);

    /// transforms face coordinates to regular system, face normal will be z'
    /// will try to align y' with z, but if that fails will align y' with y
    /// face origin will be minimum point in x', y' and z'=0
    /// will return identity transformation if cannot compute plane for vertices
    static Transformation alignFace(const std::vector<Point3d>& vertices);

    /// returns a transformation which is the inverse of this
    Transformation inverse() const;

    /// get the matrix representation directly
    Matrix matrix() const;

    /// get the vector representation directly
    Vector vector() const;

    /// get the Euler angles for the transformation, does not include translation
    EulerAngles eulerAngles() const;

    /// get the rotation matrix for the transformation, does not include translation
    Matrix rotationMatrix() const;

    /// get the translation for the transformation, does not include rotation
    Vector3d translation() const;

    /// apply the transformation to the point
    Point3d operator*(const Point3d& point) const;

    /// apply the transformation to the vector
    Vector3d operator*(const Vector3d& vector) const;

    /// apply the transformation to the bounding box
    BoundingBox operator*(const BoundingBox& boundingBox) const;

    /// apply the transformation to the plane
    Plane operator*(const Plane& plane) const;

    /// apply the transformation to a vector of points
    std::vector<Point3d> operator*(const std::vector<Point3d>& points) const;

    /// apply the transformation to a vector of vector
    std::vector<Vector3d> operator*(const std::vector<Vector3d>& vectors) const;

    /// apply the transformation to the other transformation
    Transformation operator*(const Transformation& other) const;

  private:

    REGISTER_LOGGER("utilities.Transformation");
    Matrix m_storage;

  };

  /// ostream operator
  UTILITIES_API std::ostream& operator<<(std::ostream& os, const Transformation& t);

  // optional Transformation
  typedef boost::optional<Transformation> OptionalTransformation;

  // vector of Transformation
  typedef std::vector<Transformation> TransformationVector;

  /// create rotation about origin defined by axis and angle (radians)
  UTILITIES_API Transformation createRotation(const Vector3d& axis, double radians);

  /// create rotation about point defined by axis and angle (radians)
  UTILITIES_API Transformation createRotation(const Point3d& origin, const Vector3d& axis, double radians);

  /// create rotation specified by Euler angles
  UTILITIES_API Transformation createRotation(const EulerAngles& angles);

  /// create translation along vector
  UTILITIES_API Transformation createTranslation(const Vector3d& translation);

} // openstudio

#endif //UTILITIES_GEOMETRY_TRANSFORMATION_HPP
