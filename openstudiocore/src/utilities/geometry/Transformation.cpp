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

#include "Transformation.hpp"
#include "Point3d.hpp"
#include "Vector3d.hpp"
#include "Plane.hpp"
#include "BoundingBox.hpp"
#include "EulerAngles.hpp"
#include "Geometry.hpp"
#include "../core/Assert.hpp"

#include <boost/math/constants/constants.hpp>

#include <algorithm>

#include <math.h>

using boost::numeric::ublas::identity_matrix;
using std::min;

namespace openstudio{

  /// default constructor creates identity transformation
  Transformation::Transformation()
    : m_storage(identity_matrix<double>(4))
  {}

  /// copy constructor
  Transformation::Transformation(const Transformation& other)
    : m_storage(other.m_storage)
  {}

  /// constructor from storage, asserts matrix is 4x4
  Transformation::Transformation(const Matrix& matrix)
    : m_storage(matrix)
  {
    OS_ASSERT(matrix.size1() == 4);
    OS_ASSERT(matrix.size2() == 4);
  }

  /// constructor from storage, asserts vector is size 16
  Transformation::Transformation(const Vector& vector)
    : m_storage(Matrix(4,4))
  {
    OS_ASSERT(vector.size() == 16);
    
    m_storage(0,0) = vector[0];
    m_storage(1,0) = vector[1];
    m_storage(2,0) = vector[2];
    m_storage(3,0) = vector[3];
    m_storage(0,1) = vector[4];
    m_storage(1,1) = vector[5];
    m_storage(2,1) = vector[6];
    m_storage(2,1) = vector[7];
    m_storage(0,2) = vector[8];
    m_storage(1,2) = vector[9];
    m_storage(2,2) = vector[10];
    m_storage(3,2) = vector[11];
    m_storage(0,3) = vector[12];
    m_storage(1,3) = vector[13];
    m_storage(2,3) = vector[14];
    m_storage(3,3) = vector[15];
  }

  /// rotation about origin defined by axis and angle (radians)
  Transformation Transformation::rotation(const Vector3d& axis, double radians)
  {
    Matrix storage = identity_matrix<double>(4);

    Vector3d temp = axis;
    if (!temp.normalize()){
      LOG(Error, "Could not normalize axis");
    }
    Vector normalVector = temp.vector();

    // Rodrigues' rotation formula / Rotation matrix from Euler axis/angle
    // I*cos(radians) + I*(1-cos(radians))*axis*axis^T + Q*sin(radians)
    // Q = [0, -axis[2], axis[1]; axis[2], 0, -axis[0]; -axis[1], axis[0], 0]
    Matrix P = outer_prod(normalVector, normalVector);
    Matrix I = identity_matrix<double>(3);
    Matrix Q(3, 3, 0.0);
    Q(0,1) = -normalVector(2);
    Q(0,2) =  normalVector(1);
    Q(1,0) =  normalVector(2);
    Q(1,2) = -normalVector(0);
    Q(2,0) = -normalVector(1);
    Q(2,1) =  normalVector(0);

    // rotation matrix
    Matrix R = I*cos(radians) + (1-cos(radians))*P + Q*sin(radians);

    for (unsigned i = 0; i < 3; ++i){
      for (unsigned j = 0; j < 3; ++j){
        storage(i,j) = R(i,j);
      }
    }

    return Transformation(storage);
  }

  /// rotation about point defined by axis and angle (radians)
  Transformation Transformation::rotation(const Point3d& origin, const Vector3d& axis, double radians)
  {
    Vector3d temp(origin.x(), origin.y(), origin.z());

    // translate point to origin, rotate, and then translate back
    return translation(temp)*rotation(axis, radians)*translation(-temp);
  }

  /// rotation specified by Euler angles
  Transformation Transformation::rotation(const EulerAngles& angles)
  {
    Transformation result = Transformation::rotation(Vector3d(0,0,1), angles.phi()) * 
                            Transformation::rotation(Vector3d(0,1,0), angles.theta()) *
                            Transformation::rotation(Vector3d(1,0,0), angles.psi());
    return result;
  }

  /// translation along vector
  Transformation Transformation::translation(const Vector3d& translation)
  {
    Matrix storage = identity_matrix<double>(4);

    storage(0,3) = translation.x();
    storage(1,3) = translation.y();
    storage(2,3) = translation.z();

    return Transformation(storage);
  }

  /// transforms system with z' to regular system
  /// will try to align y' with z, but if that fails will align y' with y
  Transformation Transformation::alignZPrime(const Vector3d& zPrime)
  {
    Vector3d xp;
    Vector3d yp;
    Vector3d zp = zPrime;
    if (!zp.normalize()){
      LOG(Error, "Could not normalize zPrime");
    }

    Vector3d xAxis(1,0,0);
    Vector3d yAxis(0,1,0);
    Vector3d zAxis(0,0,1);
    Vector3d negXAxis(-1,0,0);

    // check if face normal is up or down
    if (fabs(zp.dot(zAxis)) < 0.99){
      // not facing up or down, set yPrime along zAxis
      yp = zAxis - (zp.dot(zAxis)*zp);
      if (!yp.normalize()){
        LOG(Error, "Could not normalize axis");
      }
      xp = yp.cross(zp);
    }else{
      // facing up or down, set xPrime along -xAxis
      xp = negXAxis - (zp.dot(negXAxis)*zp);
      if (!xp.normalize()){
        LOG(Error, "Could not normalize axis");
      }
      yp = zp.cross(xp);
    }

    Matrix storage = identity_matrix<double>(4);
    storage(0,0) = xp.x();
    storage(1,0) = xp.y();
    storage(2,0) = xp.z();
    storage(0,1) = yp.x();
    storage(1,1) = yp.y();
    storage(2,1) = yp.z();
    storage(0,2) = zp.x();
    storage(1,2) = zp.y();
    storage(2,2) = zp.z();

    return Transformation(storage);
  }

  /// transforms face coordinates to regular system, face normal will be z'
  /// will try to align y' with z, but if that fails will align y' with y
  /// face origin will be minimum point in x', y' and z'=0
  /// will return identity transformation if cannot compute plane for vertices
  Transformation Transformation::alignFace(const std::vector<Point3d>& vertices)
  {
    OptionalVector3d zPrime = getOutwardNormal(vertices);
    if (!zPrime){
      LOG(Error, "Cannot compute outward normal for vertices");
      return Transformation();
    }

    // align z' with outward normal
    Transformation align = alignZPrime(*zPrime);
    Point3dVector alignedVertices = align.inverse()*vertices;

    // compute translation to minimum in aligned system
    double minX = alignedVertices[0].x();
    double minY = alignedVertices[0].y();
    double minZ = alignedVertices[0].z();

    for (const Point3d& vertex : alignedVertices){
      minX = min(minX, vertex.x());
      minY = min(minY, vertex.y());
      minZ = min(minZ, vertex.z());
    }
    Transformation translate = translation(Vector3d(minX, minY, minZ));

    return align*translate;
  }

  /// returns a transformation which is the inverse of this
  Transformation Transformation::inverse() const
  {
    Matrix matrix(4,4);
    bool test = invert(m_storage, matrix);
    if (!test){
      // this should never happen
      LOG_AND_THROW("Matrix inversion failed");
    }
    return Transformation(matrix);
  }

  /// get the matrix representation directly
  Matrix Transformation::matrix() const
  {
    return m_storage;
  }

  /// get the vector representation directly
  Vector Transformation::vector() const
  {
    openstudio::Vector result(16);
    result[0] = m_storage(0,0);
    result[1] = m_storage(1,0);
    result[2] = m_storage(2,0);
    result[3] = m_storage(3,0);
    result[4] = m_storage(0,1);
    result[5] = m_storage(1,1);
    result[6] = m_storage(2,1);
    result[7] = m_storage(3,1);
    result[8] = m_storage(0,2);
    result[9] = m_storage(1,2);
    result[10] = m_storage(2,2);
    result[11] = m_storage(3,2);
    result[12] = m_storage(0,3);
    result[13] = m_storage(1,3);
    result[14] = m_storage(2,3);
    result[15] = m_storage(3,3);
    return result;
  }

  /// get the Euler angles for the transformation, does not include translation
  EulerAngles Transformation::eulerAngles() const
  {
    double psi;
    double theta;
    double phi;
    if (m_storage(2,0) == 1.0){
      phi = 0;
      theta = -boost::math::constants::pi<double>()/2.0;
      psi = atan2(-m_storage(0,1), -m_storage(0,2));
    }else if(m_storage(2,0) == -1.0){
      phi = 0;
      theta = boost::math::constants::pi<double>()/2.0;
      psi = atan2(m_storage(0,1), m_storage(0,2));
    }else{
      theta = -asin(m_storage(2,0));
      // theta = pi + asin(m_storage(2,0)); // alternate solution
      psi = atan2(m_storage(2,1)/cos(theta), m_storage(2,2)/cos(theta));
      phi = atan2(m_storage(1,0)/cos(theta), m_storage(0,0)/cos(theta));
      
    }
    EulerAngles result(psi, theta, phi);
    return result;
  }

  /// get the rotation matrix for the transformation, does not include translation
  Matrix Transformation::rotationMatrix() const
  {
    Matrix result(3,3);
    for(unsigned i = 0 ; i < 3; ++i){
      for(unsigned j = 0; j < 3; ++j){
        result(i,j) = m_storage(i,j);
      }
    }
    return result;
  }

  /// get the translation for the transformation, does not include rotation
  Vector3d Transformation::translation() const
  {
    Vector3d result(m_storage(0, 3), m_storage(1, 3), m_storage(2, 3));
    return result;
  }

  /// apply the transformation to the point
  Point3d Transformation::operator*(const Point3d& point) const
  {
    Vector temp(4);
    temp(0) = point.x();
    temp(1) = point.y();
    temp(2) = point.z();
    temp(3) = 1.0;
    temp = prod(m_storage, temp);
    return Point3d(temp[0], temp[1], temp[2]);
  }

  /// apply the transformation to the vector
  Vector3d Transformation::operator*(const Vector3d& vector) const
  {
    Vector temp(4);
    temp(0) = vector.x();
    temp(1) = vector.y();
    temp(2) = vector.z();
    temp(3) = 1.0;
    temp = prod(m_storage, temp);
    return Vector3d(temp[0], temp[1], temp[2]);
  }

  /// apply the transformation to the BoundingBox
  BoundingBox Transformation::operator*(const BoundingBox& boundingBox) const
  {
    BoundingBox result;
    std::vector<Point3d> transformedPoints = (*this)*boundingBox.corners();
    result.addPoints(transformedPoints);
    return result;
  }

  /// apply the transformation to the plane
  Plane Transformation::operator*(const Plane& plane) const
  {
    // translate a point on the plane, just project (0,0,0)
    Point3d point = plane.project(Point3d(0,0,0));

    // get a point at outward normal
    Vector3d outwardNormal = plane.outwardNormal();
    Point3d refPoint = point + outwardNormal;
    
    // translate the two points and recompute the normal
    Point3d newPoint = (*this) * point;
    Point3d newRefPoint = (*this) * refPoint;
    Vector3d newNormal = newRefPoint - newPoint;

    return Plane(newPoint, newNormal);
  }

  /// apply the transformation to a vector of points
  std::vector<Point3d> Transformation::operator*(const std::vector<Point3d>& points) const
  {
    std::vector<Point3d> result(points.size());
    for(unsigned i = 0; i < points.size(); ++i){
      result[i] = (*this)*points[i];
    }
    return result;
  }

  /// apply the transformation to a vector of vector
  std::vector<Vector3d> Transformation::operator*(const std::vector<Vector3d>& vectors) const
  {
    std::vector<Vector3d> result(vectors.size());
    for(unsigned i = 0; i < vectors.size(); ++i){
      result[i] = (*this)*vectors[i];
    }
    return result;
  }

  /// apply the transformation to the other transformation
  Transformation Transformation::operator*(const Transformation& other) const
  {
    return Transformation(prod(m_storage, other.m_storage));
  }

  /// ostream operator
  std::ostream& operator<<(std::ostream& os, const Transformation& t)
  {
    os << t.matrix();
    return os;
  }


  Transformation createRotation(const Vector3d& axis, double radians)
  {
    return Transformation::rotation(axis, radians);
  }

  Transformation createRotation(const Point3d& origin, const Vector3d& axis, double radians)
  {
    return Transformation::rotation(origin, axis, radians);
  }

  Transformation createRotation(const EulerAngles& angles)
  {
    return Transformation::rotation(angles);
  }

  Transformation createTranslation(const Vector3d& translation)
  {
    return Transformation::translation(translation);
  }

} // openstudio
