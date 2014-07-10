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

#include <gtest/gtest.h>
#include "GeometryFixture.hpp"

#include "../Transformation.hpp"
#include "../Point3d.hpp"
#include "../Vector3d.hpp"
#include "../EulerAngles.hpp"

#include <boost/math/constants/constants.hpp>

using namespace std;
using namespace boost;
using namespace openstudio;

TEST_F(GeometryFixture, RotationTransformations)
{
  double tol = 1.0E-12;
  Vector3d z(0,0,1);
  Point3d point1(1,0,0);
  Transformation T;
  Point3d temp;

  // identity transformation
  temp = T*point1;
  EXPECT_NEAR(1.0, temp.x(), tol);
  EXPECT_NEAR(0.0, temp.y(), tol);
  EXPECT_NEAR(0.0, temp.z(), tol);

  // rotation 30 degrees about z
  T = Transformation::rotation(z, degToRad(30));
  temp = T*point1;
  EXPECT_NEAR(cos(degToRad(30)), temp.x(), tol);
  EXPECT_NEAR(sin(degToRad(30)), temp.y(), tol);
  EXPECT_NEAR(0.0, temp.z(), tol);

  // rotation -30 degrees about z
  temp = T.inverse()*point1;
  EXPECT_NEAR(cos(degToRad(30)), temp.x(), tol);
  EXPECT_NEAR(-sin(degToRad(30)), temp.y(), tol);
  EXPECT_NEAR(0.0, temp.z(), tol);

  // rotation 0 degrees about z
  temp = T.inverse()*T*point1;
  EXPECT_NEAR(1.0, temp.x(), tol);
  EXPECT_NEAR(0.0, temp.y(), tol);
  EXPECT_NEAR(0.0, temp.z(), tol);

  // rotation 0 degrees about z
  temp = T*T.inverse()*point1;
  EXPECT_NEAR(1.0, temp.x(), tol);
  EXPECT_NEAR(0.0, temp.y(), tol);
  EXPECT_NEAR(0.0, temp.z(), tol);

  // rotation 90 degrees about z
  T = Transformation::rotation(z, degToRad(90));
  temp = T*point1;
  EXPECT_NEAR(0.0, temp.x(), tol);
  EXPECT_NEAR(1.0, temp.y(), tol);
  EXPECT_NEAR(0.0, temp.z(), tol);
}

TEST_F(GeometryFixture, RotationAboutPointTransformations)
{
  double tol = 1.0E-6;
  Vector3d z(0,0,1);
  Point3d point1(1,0,0);
  Transformation T;
  Point3d temp;

  // rotation around origin, 30 degrees about z
  T = Transformation::rotation(Point3d(0,0,0), z, degToRad(30));
  temp = T*point1;
  EXPECT_NEAR(0.866025, temp.x(), tol);
  EXPECT_NEAR(0.5, temp.y(), tol);
  EXPECT_NEAR(0.0, temp.z(), tol);

  // rotation around 1*x, 30 degrees about z
  T = Transformation::rotation(Point3d(1,0,0), z, degToRad(30));
  temp = T*point1;
  EXPECT_NEAR(1.0, temp.x(), tol);
  EXPECT_NEAR(0.0, temp.y(), tol);
  EXPECT_NEAR(0.0, temp.z(), tol);

  // rotation around 1*y, 30 degrees about z
  T = Transformation::rotation(Point3d(0,1,0), z, degToRad(30));
  temp = T*point1;
  EXPECT_NEAR(1.366025, temp.x(), tol);
  EXPECT_NEAR(0.633975, temp.y(), tol);
  EXPECT_NEAR(0.0, temp.z(), tol);

  // rotation around 1*z, 30 degrees about z
  T = Transformation::rotation(Point3d(0,0,1), z, degToRad(30));
  temp = T*point1;
  EXPECT_NEAR(0.866025, temp.x(), tol);
  EXPECT_NEAR(0.5, temp.y(), tol);
  EXPECT_NEAR(0.0, temp.z(), tol);

  // rotation around 1*x + 1*y + 1*z, 30 degrees about z
  T = Transformation::rotation(Point3d(1,1,1), z, degToRad(30));
  temp = T*point1;
  EXPECT_NEAR(1.5, temp.x(), tol);
  EXPECT_NEAR(0.133975, temp.y(), tol);
  EXPECT_NEAR(0.0, temp.z(), tol);
}

TEST_F(GeometryFixture, TranslationTransformations)
{
  double tol = 1.0E-12;
  Vector3d trans(1,1,1);
  Point3d point1(1,0,0);
  Transformation T;
  Point3d temp;

  // identity transformation
  temp = T*point1;
  EXPECT_NEAR(1.0, temp.x(), tol);
  EXPECT_NEAR(0.0, temp.y(), tol);
  EXPECT_NEAR(0.0, temp.z(), tol);

  // move by 1, 1, 1
  T = Transformation::translation(trans);
  temp = T*point1;
  EXPECT_NEAR(2.0, temp.x(), tol);
  EXPECT_NEAR(1.0, temp.y(), tol);
  EXPECT_NEAR(1.0, temp.z(), tol);

  // move by -1, -1, -1
  temp = T.inverse()*point1;
  EXPECT_NEAR(0.0, temp.x(), tol);
  EXPECT_NEAR(-1.0, temp.y(), tol);
  EXPECT_NEAR(-1.0, temp.z(), tol);

  // identity transformation
  temp = T.inverse()*T*point1;
  EXPECT_NEAR(1.0, temp.x(), tol);
  EXPECT_NEAR(0.0, temp.y(), tol);
  EXPECT_NEAR(0.0, temp.z(), tol);

  // identity transformation
  temp = T*T.inverse()*point1;
  EXPECT_NEAR(1.0, temp.x(), tol);
  EXPECT_NEAR(0.0, temp.y(), tol);
  EXPECT_NEAR(0.0, temp.z(), tol);
}

TEST_F(GeometryFixture, AlignZPrimeTransformations)
{
  Vector3d xAxis(1,0,0);
  Vector3d yAxis(0,1,0);
  Vector3d zAxis(0,0,1);
  Transformation T;

  Vector3d outwardNormal(0, -1, 0);
  T = Transformation::alignZPrime(outwardNormal);
  EXPECT_TRUE(xAxis == T*xAxis);
  EXPECT_TRUE(zAxis == T*yAxis);
  EXPECT_TRUE(outwardNormal == T*zAxis);

  outwardNormal = Vector3d(1, 0, 0);
  T = Transformation::alignZPrime(outwardNormal);
  EXPECT_TRUE(yAxis == T*xAxis);
  EXPECT_TRUE(zAxis == T*yAxis);
  EXPECT_TRUE(outwardNormal == T*zAxis);

  outwardNormal = Vector3d(0, 1, 0);
  T = Transformation::alignZPrime(outwardNormal);
  EXPECT_TRUE(-xAxis == T*xAxis);
  EXPECT_TRUE(zAxis == T*yAxis);
  EXPECT_TRUE(outwardNormal == T*zAxis);

  outwardNormal = Vector3d(-1, 0, 0);
  T = Transformation::alignZPrime(outwardNormal);
  EXPECT_TRUE(-yAxis == T*xAxis);
  EXPECT_TRUE(zAxis == T*yAxis);
  EXPECT_TRUE(outwardNormal == T*zAxis);

  outwardNormal = Vector3d(0, 0, 1);
  T = Transformation::alignZPrime(outwardNormal);
  EXPECT_TRUE(-xAxis == T*xAxis);
  EXPECT_TRUE(-yAxis == T*yAxis);
  EXPECT_TRUE(outwardNormal == T*zAxis);

  outwardNormal = Vector3d(0, 0, -1);
  T = Transformation::alignZPrime(outwardNormal);
  EXPECT_TRUE(-xAxis == T*xAxis);
  EXPECT_TRUE(yAxis == T*yAxis);
  EXPECT_TRUE(outwardNormal == T*zAxis);
}

TEST_F(GeometryFixture, AlignFaceTransformations)
{
  double tol = 1.0E-12;

  Point3dVector vertices(4);
  vertices[0] = Point3d(1, 0, 1);
  vertices[1] = Point3d(1, 0, 0);
  vertices[2] = Point3d(2, 0, 0);
  vertices[3] = Point3d(2, 0, 1);
  Point3dVector testVertices;
  Point3dVector tempVertices;
  Transformation T;

  // rotate 0 degrees about z
  testVertices = Transformation::rotation(Vector3d(0,0,1), 0)*vertices;
  T = Transformation::alignFace(testVertices);
  tempVertices = T.inverse()*testVertices;
  EXPECT_NEAR(0, tempVertices[0].x(), tol);
  EXPECT_NEAR(1, tempVertices[0].y(), tol);
  EXPECT_NEAR(0, tempVertices[0].z(), tol);
  EXPECT_NEAR(0, tempVertices[1].x(), tol);
  EXPECT_NEAR(0, tempVertices[1].y(), tol);
  EXPECT_NEAR(0, tempVertices[1].z(), tol);
  EXPECT_NEAR(1, tempVertices[2].x(), tol);
  EXPECT_NEAR(0, tempVertices[2].y(), tol);
  EXPECT_NEAR(0, tempVertices[2].z(), tol);
  EXPECT_NEAR(1, tempVertices[3].x(), tol);
  EXPECT_NEAR(1, tempVertices[3].y(), tol);
  EXPECT_NEAR(0, tempVertices[3].z(), tol);

  // rotate 30 degrees about z
  testVertices = Transformation::rotation(Vector3d(0,0,1), degToRad(30))*vertices;
  T = Transformation::alignFace(testVertices);
  tempVertices = T.inverse()*testVertices;
  EXPECT_NEAR(0, tempVertices[0].x(), tol);
  EXPECT_NEAR(1, tempVertices[0].y(), tol);
  EXPECT_NEAR(0, tempVertices[0].z(), tol);
  EXPECT_NEAR(0, tempVertices[1].x(), tol);
  EXPECT_NEAR(0, tempVertices[1].y(), tol);
  EXPECT_NEAR(0, tempVertices[1].z(), tol);
  EXPECT_NEAR(1, tempVertices[2].x(), tol);
  EXPECT_NEAR(0, tempVertices[2].y(), tol);
  EXPECT_NEAR(0, tempVertices[2].z(), tol);
  EXPECT_NEAR(1, tempVertices[3].x(), tol);
  EXPECT_NEAR(1, tempVertices[3].y(), tol);
  EXPECT_NEAR(0, tempVertices[3].z(), tol);

  // rotate -30 degrees about z
  testVertices = Transformation::rotation(Vector3d(0,0,1), -degToRad(30))*vertices;
  T = Transformation::alignFace(testVertices);
  tempVertices = T.inverse()*testVertices;
  EXPECT_NEAR(0, tempVertices[0].x(), tol);
  EXPECT_NEAR(1, tempVertices[0].y(), tol);
  EXPECT_NEAR(0, tempVertices[0].z(), tol);
  EXPECT_NEAR(0, tempVertices[1].x(), tol);
  EXPECT_NEAR(0, tempVertices[1].y(), tol);
  EXPECT_NEAR(0, tempVertices[1].z(), tol);
  EXPECT_NEAR(1, tempVertices[2].x(), tol);
  EXPECT_NEAR(0, tempVertices[2].y(), tol);
  EXPECT_NEAR(0, tempVertices[2].z(), tol);
  EXPECT_NEAR(1, tempVertices[3].x(), tol);
  EXPECT_NEAR(1, tempVertices[3].y(), tol);
  EXPECT_NEAR(0, tempVertices[3].z(), tol);

  // rotate -30 degrees about x
  testVertices = Transformation::rotation(Vector3d(1,0,0), -degToRad(30))*vertices;
  T = Transformation::alignFace(testVertices);
  tempVertices = T.inverse()*testVertices;
  EXPECT_NEAR(0, tempVertices[0].x(), tol);
  EXPECT_NEAR(1, tempVertices[0].y(), tol);
  EXPECT_NEAR(0, tempVertices[0].z(), tol);
  EXPECT_NEAR(0, tempVertices[1].x(), tol);
  EXPECT_NEAR(0, tempVertices[1].y(), tol);
  EXPECT_NEAR(0, tempVertices[1].z(), tol);
  EXPECT_NEAR(1, tempVertices[2].x(), tol);
  EXPECT_NEAR(0, tempVertices[2].y(), tol);
  EXPECT_NEAR(0, tempVertices[2].z(), tol);
  EXPECT_NEAR(1, tempVertices[3].x(), tol);
  EXPECT_NEAR(1, tempVertices[3].y(), tol);
  EXPECT_NEAR(0, tempVertices[3].z(), tol);

  // rotate -90 degrees about x
  testVertices = Transformation::rotation(Vector3d(1,0,0), -degToRad(90))*vertices;
  T = Transformation::alignFace(testVertices);
  tempVertices = T.inverse()*testVertices;
  EXPECT_NEAR(1, tempVertices[0].x(), tol);
  EXPECT_NEAR(0, tempVertices[0].y(), tol);
  EXPECT_NEAR(0, tempVertices[0].z(), tol);
  EXPECT_NEAR(1, tempVertices[1].x(), tol);
  EXPECT_NEAR(1, tempVertices[1].y(), tol);
  EXPECT_NEAR(0, tempVertices[1].z(), tol);
  EXPECT_NEAR(0, tempVertices[2].x(), tol);
  EXPECT_NEAR(1, tempVertices[2].y(), tol);
  EXPECT_NEAR(0, tempVertices[2].z(), tol);
  EXPECT_NEAR(0, tempVertices[3].x(), tol);
  EXPECT_NEAR(0, tempVertices[3].y(), tol);
  EXPECT_NEAR(0, tempVertices[3].z(), tol);

  // rotate 30 degrees about x
  testVertices = Transformation::rotation(Vector3d(1,0,0), degToRad(30))*vertices;
  T = Transformation::alignFace(testVertices);
  tempVertices = T.inverse()*testVertices;
  EXPECT_NEAR(0, tempVertices[0].x(), tol);
  EXPECT_NEAR(1, tempVertices[0].y(), tol);
  EXPECT_NEAR(0, tempVertices[0].z(), tol);
  EXPECT_NEAR(0, tempVertices[1].x(), tol);
  EXPECT_NEAR(0, tempVertices[1].y(), tol);
  EXPECT_NEAR(0, tempVertices[1].z(), tol);
  EXPECT_NEAR(1, tempVertices[2].x(), tol);
  EXPECT_NEAR(0, tempVertices[2].y(), tol);
  EXPECT_NEAR(0, tempVertices[2].z(), tol);
  EXPECT_NEAR(1, tempVertices[3].x(), tol);
  EXPECT_NEAR(1, tempVertices[3].y(), tol);
  EXPECT_NEAR(0, tempVertices[3].z(), tol);

  // rotate 90 degrees about x
  testVertices = Transformation::rotation(Vector3d(1,0,0), degToRad(90))*vertices;
  T = Transformation::alignFace(testVertices);
  tempVertices = T.inverse()*testVertices;
  EXPECT_NEAR(1, tempVertices[0].x(), tol);
  EXPECT_NEAR(0, tempVertices[0].y(), tol);
  EXPECT_NEAR(0, tempVertices[0].z(), tol);
  EXPECT_NEAR(1, tempVertices[1].x(), tol);
  EXPECT_NEAR(1, tempVertices[1].y(), tol);
  EXPECT_NEAR(0, tempVertices[1].z(), tol);
  EXPECT_NEAR(0, tempVertices[2].x(), tol);
  EXPECT_NEAR(1, tempVertices[2].y(), tol);
  EXPECT_NEAR(0, tempVertices[2].z(), tol);
  EXPECT_NEAR(0, tempVertices[3].x(), tol);
  EXPECT_NEAR(0, tempVertices[3].y(), tol);
  EXPECT_NEAR(0, tempVertices[3].z(), tol);
}

TEST_F(GeometryFixture, AlignFaceTransformations_TrapezoidFloor)
{
  double tol = 1.0E-12;

  Point3dVector vertices(4);
  vertices[0] = Point3d(27.69, 0, 0);
  vertices[1] = Point3d(0, 0, 0);
  vertices[2] = Point3d(5, 5, 0);
  vertices[3] = Point3d(22.69, 5, 0);
  Point3dVector tempVertices;
  Transformation T;

  T = Transformation::alignFace(vertices);
  tempVertices = T.inverse()*vertices;
  EXPECT_NEAR(0, tempVertices[0].x(), tol);
  EXPECT_NEAR(0, tempVertices[0].y(), tol);
  EXPECT_NEAR(0, tempVertices[0].z(), tol);
  EXPECT_NEAR(27.69, tempVertices[1].x(), tol);
  EXPECT_NEAR(0, tempVertices[1].y(), tol);
  EXPECT_NEAR(0, tempVertices[1].z(), tol);
  EXPECT_NEAR(22.69, tempVertices[2].x(), tol);
  EXPECT_NEAR(5, tempVertices[2].y(), tol);
  EXPECT_NEAR(0, tempVertices[2].z(), tol);
  EXPECT_NEAR(5, tempVertices[3].x(), tol);
  EXPECT_NEAR(5, tempVertices[3].y(), tol);
  EXPECT_NEAR(0, tempVertices[3].z(), tol);

}


TEST_F(GeometryFixture, Transformations)
{

  /* SketchUp implementation of this test:
      origin = Geom::Point3d.new(0,0,0)
      x_axis = Geom::Vector3d.new(0,0,1)
      y_axis = Geom::Vector3d.new(0,0,1)
      z_axis = Geom::Vector3d.new(0,0,1)
      trans = Geom::Vector3d.new(1,1,1)

      point = Geom::Point3d.new(1,0,0)

      temp = Geom::Transformation.rotation(origin, z_axis, 30.degrees)*point
      puts temp

      temp = Geom::Transformation.translation(trans )*temp
      puts temp

      temp = Geom::Transformation.rotation(origin, x_axis, 30.degrees)*temp
      puts temp

      temp = Geom::Transformation.translation(trans )*temp
      puts temp

      temp = Geom::Transformation.rotation(origin, y_axis, 30.degrees)*temp
      puts temp

      temp = Geom::Transformation.translation(trans )*temp
      puts temp
  */

  double tol = 1.0E-6;
  Vector3d xAxis(0,0,1);
  Vector3d yAxis(0,0,1);
  Vector3d zAxis(0,0,1);
  Vector3d trans(1,1,1);
  Point3d point(1,0,0);
  Point3d temp;

  temp = Transformation::rotation(zAxis, degToRad(30))*point;
  EXPECT_NEAR(0.866025, temp.x(), tol);
  EXPECT_NEAR(0.5, temp.y(), tol);
  EXPECT_NEAR(0.0, temp.z(), tol);

  temp = Transformation::translation(trans)*temp;
  EXPECT_NEAR(1.866025, temp.x(), tol);
  EXPECT_NEAR(1.5, temp.y(), tol);
  EXPECT_NEAR(1.0, temp.z(), tol);

  temp = Transformation::rotation(xAxis, degToRad(30))*temp;
  EXPECT_NEAR(0.866025, temp.x(), tol);
  EXPECT_NEAR(2.232051, temp.y(), tol);
  EXPECT_NEAR(1.0, temp.z(), tol);

  temp = Transformation::translation(trans)*temp;
  EXPECT_NEAR(1.866025, temp.x(), tol);
  EXPECT_NEAR(3.232051, temp.y(), tol);
  EXPECT_NEAR(2.0, temp.z(), tol);

  temp = Transformation::rotation(yAxis, degToRad(30))*temp;
  EXPECT_NEAR(0.0, temp.x(), tol);
  EXPECT_NEAR(3.732051, temp.y(), tol);
  EXPECT_NEAR(2.0, temp.z(), tol);

  temp = Transformation::translation(trans)*temp;
  EXPECT_NEAR(1.0, temp.x(), tol);
  EXPECT_NEAR(4.732051, temp.y(), tol);
  EXPECT_NEAR(3.0, temp.z(), tol);
}

TEST_F(GeometryFixture, EulerAngles)
{
  Transformation transformation;
  EulerAngles angles = transformation.eulerAngles();
  EXPECT_EQ(0.0, angles.psi());
  EXPECT_EQ(0.0, angles.theta());
  EXPECT_EQ(0.0, angles.phi());

  angles = EulerAngles(0,0,0);
  transformation = Transformation::rotation(angles);
  Matrix rotationMatrix = transformation.rotationMatrix();
  ASSERT_EQ(static_cast<unsigned>(3), rotationMatrix.size1());
  ASSERT_EQ(static_cast<unsigned>(3), rotationMatrix.size2());
  EXPECT_EQ(1.0, rotationMatrix(0,0));
  EXPECT_EQ(0.0, rotationMatrix(0,1));
  EXPECT_EQ(0.0, rotationMatrix(0,2));
  EXPECT_EQ(0.0, rotationMatrix(1,0));
  EXPECT_EQ(1.0, rotationMatrix(1,1));
  EXPECT_EQ(0.0, rotationMatrix(1,2));
  EXPECT_EQ(0.0, rotationMatrix(2,0));
  EXPECT_EQ(0.0, rotationMatrix(2,1));
  EXPECT_EQ(1.0, rotationMatrix(2,2));

  transformation = Transformation::translation(Vector3d(1,1,1));
  angles = transformation.eulerAngles();
  EXPECT_EQ(0.0, angles.psi());
  EXPECT_EQ(0.0, angles.theta());
  EXPECT_EQ(0.0, angles.phi());

  transformation = Transformation::rotation(Vector3d(1,0,0), 1.0);
  angles = transformation.eulerAngles();
  EXPECT_EQ(1.0, angles.psi());
  EXPECT_EQ(0.0, angles.theta());
  EXPECT_EQ(0.0, angles.phi());

  angles = EulerAngles(1,0,0);
  transformation = Transformation::rotation(angles);
  rotationMatrix = transformation.rotationMatrix();
  ASSERT_EQ(static_cast<unsigned>(3), rotationMatrix.size1());
  ASSERT_EQ(static_cast<unsigned>(3), rotationMatrix.size2());
  EXPECT_NEAR(1.0, rotationMatrix(0,0), 0.0001);
  EXPECT_NEAR(0.0, rotationMatrix(0,1), 0.0001);
  EXPECT_NEAR(0.0, rotationMatrix(0,2), 0.0001);
  EXPECT_NEAR(0.0, rotationMatrix(1,0), 0.0001);
  EXPECT_NEAR(cos(1.0), rotationMatrix(1,1), 0.0001);
  EXPECT_NEAR(-sin(1.0), rotationMatrix(1,2), 0.0001);
  EXPECT_NEAR(0.0, rotationMatrix(2,0), 0.0001);
  EXPECT_NEAR(sin(1.0), rotationMatrix(2,1), 0.0001);
  EXPECT_NEAR(cos(1.0), rotationMatrix(2,2), 0.0001);

  transformation = Transformation::rotation(Vector3d(0,1,0), 1.0);
  angles = transformation.eulerAngles();
  EXPECT_EQ(0.0, angles.psi());
  EXPECT_EQ(1.0, angles.theta());
  EXPECT_EQ(0.0, angles.phi());

  angles = EulerAngles(0,1,0);
  transformation = Transformation::rotation(angles);
  rotationMatrix = transformation.rotationMatrix();
  ASSERT_EQ(static_cast<unsigned>(3), rotationMatrix.size1());
  ASSERT_EQ(static_cast<unsigned>(3), rotationMatrix.size2());
  EXPECT_NEAR(cos(1.0), rotationMatrix(0,0), 0.0001);
  EXPECT_NEAR(0.0, rotationMatrix(0,1), 0.0001);
  EXPECT_NEAR(sin(1.0), rotationMatrix(0,2), 0.0001);
  EXPECT_NEAR(0.0, rotationMatrix(1,0), 0.0001);
  EXPECT_NEAR(1.0, rotationMatrix(1,1), 0.0001);
  EXPECT_NEAR(0.0, rotationMatrix(1,2), 0.0001);
  EXPECT_NEAR(-sin(1.0), rotationMatrix(2,0), 0.0001);
  EXPECT_NEAR(0.0, rotationMatrix(2,1), 0.0001);
  EXPECT_NEAR(cos(1.0), rotationMatrix(2,2), 0.0001);

  transformation = Transformation::rotation(Vector3d(0,0,1), 1.0);
  angles = transformation.eulerAngles();
  EXPECT_EQ(0.0, angles.psi());
  EXPECT_EQ(0.0, angles.theta());
  EXPECT_EQ(1.0, angles.phi());

  angles = EulerAngles(0,0,1);
  transformation = Transformation::rotation(angles);
  rotationMatrix = transformation.rotationMatrix();
  ASSERT_EQ(static_cast<unsigned>(3), rotationMatrix.size1());
  ASSERT_EQ(static_cast<unsigned>(3), rotationMatrix.size2());
  EXPECT_NEAR(cos(1.0), rotationMatrix(0,0), 0.0001);
  EXPECT_NEAR(-sin(1.0), rotationMatrix(0,1), 0.0001);
  EXPECT_NEAR(0.0, rotationMatrix(0,2), 0.0001);
  EXPECT_NEAR(sin(1.0), rotationMatrix(1,0), 0.0001);
  EXPECT_NEAR(cos(1.0), rotationMatrix(1,1), 0.0001);
  EXPECT_NEAR(0.0, rotationMatrix(1,2), 0.0001);
  EXPECT_NEAR(0.0, rotationMatrix(2,0), 0.0001);
  EXPECT_NEAR(0.0, rotationMatrix(2,1), 0.0001);
  EXPECT_NEAR(1.0, rotationMatrix(2,2), 0.0001);

  Matrix matrix(4,4);
  matrix(0,0) = 0.5;
  matrix(0,1) = -0.1464;
  matrix(0,2) = 0.8536;
  matrix(1,0) = 0.5;
  matrix(1,1) = 0.8536;
  matrix(1,2) = -0.1464;
  matrix(2,0) = -0.7071;
  matrix(2,1) = 0.5;
  matrix(2,2) = 0.5;
  matrix(3,3) = 1.0;

  transformation = Transformation(matrix);
  angles = transformation.eulerAngles();
  EXPECT_NEAR(boost::math::constants::pi<double>()/4.0, angles.psi(), 0.0001);
  EXPECT_NEAR(boost::math::constants::pi<double>()/4.0, angles.theta(), 0.0001);
  EXPECT_NEAR(boost::math::constants::pi<double>()/4.0, angles.phi(), 0.0001);

  angles = EulerAngles(boost::math::constants::pi<double>()/4.0,
                       boost::math::constants::pi<double>()/4.0,
                       boost::math::constants::pi<double>()/4.0);
  transformation = Transformation::rotation(angles);
  rotationMatrix = transformation.rotationMatrix();
  EXPECT_NEAR(0.5, rotationMatrix(0,0), 0.0001);
  EXPECT_NEAR(-0.1464, rotationMatrix(0,1), 0.0001);
  EXPECT_NEAR(0.8536, rotationMatrix(0,2), 0.0001);
  EXPECT_NEAR(0.5, rotationMatrix(1,0), 0.0001);
  EXPECT_NEAR(0.8536, rotationMatrix(1,1), 0.0001);
  EXPECT_NEAR(-0.1464, rotationMatrix(1,2), 0.0001);
  EXPECT_NEAR(-0.7071, rotationMatrix(2,0), 0.0001);
  EXPECT_NEAR(0.5, rotationMatrix(2,1), 0.0001);
  EXPECT_NEAR(0.5, rotationMatrix(2,2), 0.0001);
}

TEST_F(GeometryFixture, Transformation_Decompose)
{
  Transformation translation = Transformation::translation(Vector3d(1, 0, 0));
  Transformation rotation = Transformation::rotation(Vector3d(0,0,1), degToRad(-90));

  Transformation transformation = translation*rotation;
  Vector3d origin = transformation.translation();
  EulerAngles angles = transformation.eulerAngles();
  Transformation test = Transformation::translation(origin)*Transformation::rotation(angles);

  EXPECT_TRUE(transformation.matrix() == test.matrix()) << transformation.matrix() << std::endl << test.matrix();

  transformation = rotation*translation;
  origin = transformation.translation();
  angles = transformation.eulerAngles();
  test = Transformation::translation(origin)*Transformation::rotation(angles);

  EXPECT_TRUE(transformation.matrix() == test.matrix()) << transformation.matrix() << std::endl << test.matrix();

}
