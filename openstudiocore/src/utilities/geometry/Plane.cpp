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

#include "Plane.hpp"

#include "Point3d.hpp"
#include "Vector3d.hpp"
#include "Geometry.hpp"
#include "../data/Vector.hpp"
#include "../data/Matrix.hpp"

#include "../core/Assert.hpp"

#include <iomanip>

namespace openstudio{

  Plane::Plane(const Plane& other)
    : m_a(other.a()), m_b(other.b()), m_c(other.c()), m_d(other.d())
  {
    // test that normal has length 1
    double length = m_a*m_a + m_b*m_b + m_c*m_c;
    double tol = 0.0001;
    OS_ASSERT(fabs(1.0-length) < tol);
  }

  Plane::Plane(const Point3d& point, const Vector3d& outwardNormal)
  {
    Vector3d thisNormal(outwardNormal);
    if (!thisNormal.normalize()){
      LOG_AND_THROW("Cannot initialize plane because normal is undefined");
    }

    m_a = thisNormal.x();
    m_b = thisNormal.y();
    m_c = thisNormal.z();
    m_d = -thisNormal.x()*point.x() - thisNormal.y()*point.y() - thisNormal.z()*point.z();
  }

  double det3x3(const Matrix& m)
  {
    double det = m(0,0)*m(1,1)*m(2,2) + 
                 m(0,1)*m(1,2)*m(2,0) +
                 m(0,2)*m(1,0)*m(2,1) -
                 m(0,2)*m(1,1)*m(2,0) -
                 m(0,1)*m(1,0)*m(2,2) - 
                 m(0,0)*m(1,2)*m(2,1);
    return det;
  }

  Plane::Plane(const std::vector<Point3d>& points)
    : m_a(0.0), m_b(0.0), m_c(0.0), m_d(0.0)
  {
    unsigned N = points.size();
    if (N < 3){
      LOG_AND_THROW("Cannot compute plane with fewer than three points");
    }else if (N == 3){

      // duplicates code in point and normal ctor, points[1] is the point and (a x b) is the normal
      Point3d point = points[1];
      Vector3d a = points[1]-points[0];
      Vector3d b = points[2]-points[1];
      Vector3d thisNormal = a.cross(b);

      if (!thisNormal.normalize()){
        LOG_AND_THROW("Cannot initialize plane because normal is undefined");
      }

      m_a = thisNormal.x();
      m_b = thisNormal.y();
      m_c = thisNormal.z();
      m_d = -thisNormal.x()*point.x() - thisNormal.y()*point.y() - thisNormal.z()*point.z();

    }else{

      bool foundSolution = false;
      double tol = 1e-8; // 0.0001 was too big for the determinant tolerance, 1e-12 was too small
      double maxDet = tol;

      // solve the equation ax+by+cz+d=0 in a few different ways, keep the best one

      {
        // Ax = b, x = [a/c; b/c; d/c]
        Matrix A(N,3);
        Matrix At(3,N);
        Vector b(N);
        for (unsigned i = 0; i < N; ++i){
          A(i,0) = points[i].x() - points[0].x(); 
          A(i,1) = points[i].y() - points[0].y();
          A(i,2) = 1.0;
          At(0,i) = A(i,0); 
          At(1,i) = A(i,1);
          At(2,i) = A(i,2);
          b[i] = -(points[i].z() - points[0].z());
        }
      
        Matrix AtA = prod(At, A);
        double det = det3x3(AtA); // always positive for A'*A
        if (det > maxDet){
          Matrix AtAInv(3,3);
          bool test = invert(AtA, AtAInv);
          if (test){
            maxDet = det;
            Vector x = prod(prod(AtAInv, At), b);
            double a_c = x[0];
            double b_c = x[1];
            double d_c = x[2];

            // a = a_c*c
            // b = b_c*c
            // d = d_c*c
            // a^2 + b^2 + c^2 = 1
            // c^2*(a_c^2 + b_c^2 + 1) = 1

            m_c = 1.0/sqrt(a_c*a_c + b_c*b_c + 1.0);
            m_a = a_c*m_c;
            m_b = b_c*m_c;
            m_d = d_c*m_c - m_a*points[0].x() - m_b*points[0].y() - m_c*points[0].z();
            foundSolution = true;
          }
        }
      }

      {
        // Ax = b, x = [a/b; c/b; d/b]
        Matrix A(N,3);
        Matrix At(3,N);
        Vector b(N);
        for (unsigned i = 0; i < N; ++i){
          A(i,0) = points[i].x() - points[0].x(); 
          A(i,1) = points[i].z() - points[0].z();
          A(i,2) = 1.0;
          At(0,i) = A(i,0); 
          At(1,i) = A(i,1);
          At(2,i) = A(i,2);
          b[i] = -(points[i].y() - points[0].y());
        }
      
        Matrix AtA = prod(At, A);
        double det = det3x3(AtA); // always positive for A'*A
        if (det > maxDet){
          Matrix AtAInv(3,3);
          bool test = invert(AtA, AtAInv);
          if (test){
            maxDet = det;
            Vector x = prod(prod(AtAInv, At), b);
            double a_b = x[0];
            double c_b = x[1];
            double d_b = x[2];

            // a = a_b*b
            // c = c_b*b
            // d = d_b*b
            // a^2 + b^2 + c^2 = 1
            // b^2*(a_b^2 + c_b^2 + 1) = 1

            m_b = 1.0/sqrt(a_b*a_b + c_b*c_b + 1.0);
            m_a = a_b*m_b;
            m_c = c_b*m_b;
            m_d = d_b*m_b - m_a*points[0].x() - m_b*points[0].y() - m_c*points[0].z();
            foundSolution = true;
          }
        }
      }

      {
        // Ax = b, x = [b/a; c/a; d/a]
        Matrix A(N,3);
        Matrix At(3,N);
        Vector b(N);
        for (unsigned i = 0; i < N; ++i){
          A(i,0) = points[i].y() - points[0].y(); 
          A(i,1) = points[i].z() - points[0].z();
          A(i,2) = 1.0;
          At(0,i) = A(i,0); 
          At(1,i) = A(i,1);
          At(2,i) = A(i,2);
          b[i] = -(points[i].x() - points[0].x());
        }
      
        Matrix AtA = prod(At, A);
        double det = det3x3(AtA); // always positive for A'*A
        if (det > maxDet){
          Matrix AtAInv(3,3);
          bool test = invert(AtA, AtAInv);
          if (test){
            maxDet = det;
            Vector x = prod(prod(AtAInv, At), b);
            double b_a = x[0];
            double c_a = x[1];
            double d_a = x[2];

            // b = b_a*a
            // c = c_a*a
            // d = d_a*a
            // a^2 + b^2 + c^2 = 1
            // a^2*(b_a^2 + c_a^2 + 1) = 1

            m_a = 1.0/sqrt(b_a*b_a + c_a*c_a + 1.0);
            m_b = b_a*m_a;
            m_c = c_a*m_a;
            m_d = d_a*m_a - m_a*points[0].x() - m_b*points[0].y() - m_c*points[0].z();
            foundSolution = true;
          }
        }
      }

      if (!foundSolution){
        //std::stringstream ss;
        //ss << std::fixed << std::setprecision(20) << points << std::endl;
        //std::string s = ss.str();
        LOG_AND_THROW("Cannot compute plane for points " << points);
      }

      // get outward normal from vertices, plane outward normal should match sense of vertices
      // this corresponds to the other solution to the sqrt
      boost::optional<Vector3d> outwardNormal = getOutwardNormal(points);
      if (outwardNormal){
        double dot = m_a*outwardNormal.get().x() + m_b*outwardNormal.get().y() + m_c*outwardNormal.get().z();
        if (dot < 0){
          m_a = -m_a;
          m_b = -m_b;
          m_c = -m_c;
          m_d = -m_d;
        }
      }

      // test that normal has length 1
      double length = m_a*m_a + m_b*m_b + m_c*m_c;
      tol = 0.0001;
      OS_ASSERT(fabs(1.0-length) <= tol);
    }
  }

  Plane::Plane(double a, double b, double c, double d)
    : m_a(a), m_b(b), m_c(c), m_d(d)
  {
    // test that normal has length 1
    double length = m_a*m_a + m_b*m_b + m_c*m_c;
    double tol = 0.0001;
    OS_ASSERT(fabs(1.0-length) <= tol);
  }

  Plane::~Plane()
  {
  }

  Vector3d Plane::outwardNormal() const
  {
    return Vector3d(m_a, m_b, m_c);
  }

  bool Plane::parallel(const Plane& other, double tol) const
  {
    // dot product of outward normals should be 1 or negative 1
    double dot = (m_a*other.a() + m_b*other.b() + m_c*other.c());
    bool result = (fabs(dot) >= 1.0-tol);
    return result;
  }

  // is this plane equal to the other plane
  // true if dot product of outward normals is >= (1-tol) and abs(d1-d2) <= tol
  bool Plane::equal(const Plane& other, double tol) const
  {
    double dot = (m_a*other.a() + m_b*other.b() + m_c*other.c());
    double dist = m_d - other.d();
    bool result = (dot >= 1-tol) && (fabs(dist) <= tol);
    return result;
  }

  // is this plane reverse equal to the other plane
  // true if dot product of outward normals is <= (-1+tol) and abs(d1+d2) <= tol
  bool Plane::reverseEqual(const Plane& other, double tol) const
  {
    double dot = (m_a*other.a() + m_b*other.b() + m_c*other.c());
    double dist = m_d + other.d();
    bool result = (dot <= -1+tol) && (fabs(dist) <= tol);
    return result;
  }

  Plane Plane::reversePlane() const
  {
    return Plane(-m_a, -m_b, -m_c, -m_d);
  }

  Point3d Plane::project(const Point3d& point) const
  {
    // http://www.9math.com/book/projection-point-plane
    double u = point.x();
    double v = point.y();
    double w = point.z();

    double num = m_a*u + m_b*v + m_c*w + m_d;
    double den = m_a*m_a + m_b*m_b + m_c*m_c; // this should always be 1.0
    double ratio = num/den;

    double x = u - m_a*ratio;
    double y = v - m_b*ratio;
    double z = w - m_c*ratio;

    return Point3d(x,y,z);
  }

  std::vector<Point3d> Plane::project(const std::vector<Point3d>& points) const
  {
    std::vector<Point3d> result;
    result.reserve(points.size());
    for (const Point3d& point : points){
      result.push_back(this->project(point));
    }
    return result;
  }

  bool Plane::pointOnPlane(const Point3d& point, double tol) const
  {
    // project point to plane
    Point3d projected = project(point);

    // get distance
    Vector3d diff = point - projected;
    double dist = diff.length();

    return (dist <= tol);
  }

  double Plane::a() const
  {
    return m_a;
  }

  double Plane::b() const
  {
    return m_b;
  }

  double Plane::c() const
  {
    return m_c;
  }

  double Plane::d() const
  {
    return m_d;
  }

  std::ostream& operator<<(std::ostream& os, const Plane& plane)
  {
    os << "[" << plane.a() << ", " << plane.b() << ", " << plane.c() << ", " << plane.d() << "]";
    return os;
  }

}
