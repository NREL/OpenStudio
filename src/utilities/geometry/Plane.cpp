/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Plane.hpp"

#include "Point3d.hpp"
#include "Vector3d.hpp"
#include "Geometry.hpp"
#include "../data/Vector.hpp"
#include "../data/Matrix.hpp"

#include "../core/Assert.hpp"

#include <cmath>
#include <iomanip>

namespace openstudio {

Plane::Plane(const Point3d& point, const Vector3d& outwardNormal) {
  Vector3d thisNormal(outwardNormal);
  if (!thisNormal.normalize()) {
    LOG_AND_THROW("Cannot initialize plane because normal is undefined");
  }

  m_a = thisNormal.x();
  m_b = thisNormal.y();
  m_c = thisNormal.z();
  m_d = -thisNormal.x() * point.x() - thisNormal.y() * point.y() - thisNormal.z() * point.z();
}

double det3x3(const Matrix& m) {
  const double det = m(0, 0) * m(1, 1) * m(2, 2) + m(0, 1) * m(1, 2) * m(2, 0) + m(0, 2) * m(1, 0) * m(2, 1) - m(0, 2) * m(1, 1) * m(2, 0)
                     - m(0, 1) * m(1, 0) * m(2, 2) - m(0, 0) * m(1, 2) * m(2, 1);
  return det;
}

Plane::Plane(const std::vector<Point3d>& points) : m_a(0.0), m_b(0.0), m_c(0.0), m_d(0.0) {
  const size_t N = points.size();
  if (N < 3) {
    LOG_AND_THROW("Cannot compute plane with fewer than three points");
  } else if (N == 3) {

    // duplicates code in point and normal ctor, points[1] is the point and (a x b) is the normal
    const Point3d& point = points[1];
    const Vector3d a = points[1] - points[0];
    const Vector3d b = points[2] - points[1];
    Vector3d thisNormal = a.cross(b);

    if (!thisNormal.normalize()) {
      LOG_AND_THROW("Cannot initialize plane because normal is undefined");
    }

    m_a = thisNormal.x();
    m_b = thisNormal.y();
    m_c = thisNormal.z();
    m_d = -thisNormal.x() * point.x() - thisNormal.y() * point.y() - thisNormal.z() * point.z();

  } else {

    bool foundSolution = false;
    // 0.0001 was too big for the determinant tolerance, 1e-12 was too small
    double maxDet = 1e-8;

    // solve the equation ax+by+cz+d=0 in a few different ways, keep the best one

    {
      // Ax = b, x = [a/c; b/c; d/c]
      Matrix A(N, 3);
      Matrix At(3, N);
      Vector b(N);
      for (unsigned i = 0; i < N; ++i) {
        A(i, 0) = points[i].x() - points[0].x();
        A(i, 1) = points[i].y() - points[0].y();
        A(i, 2) = 1.0;
        At(0, i) = A(i, 0);
        At(1, i) = A(i, 1);
        At(2, i) = A(i, 2);
        b[i] = -(points[i].z() - points[0].z());
      }

      const Matrix AtA = prod(At, A);
      const double det = det3x3(AtA);  // always positive for A'*A
      if (det > maxDet) {
        Matrix AtAInv(3, 3);
        const bool test = invert(AtA, AtAInv);
        if (test) {
          maxDet = det;
          Vector x = prod(prod(AtAInv, At), b);
          const double a_c = x[0];
          const double b_c = x[1];
          const double d_c = x[2];

          // a = a_c*c
          // b = b_c*c
          // d = d_c*c
          // a^2 + b^2 + c^2 = 1
          // c^2*(a_c^2 + b_c^2 + 1) = 1

          m_c = 1.0 / sqrt(a_c * a_c + b_c * b_c + 1.0);
          m_a = a_c * m_c;
          m_b = b_c * m_c;
          m_d = d_c * m_c - m_a * points[0].x() - m_b * points[0].y() - m_c * points[0].z();
          foundSolution = true;
        }
      }
    }

    {
      // Ax = b, x = [a/b; c/b; d/b]
      Matrix A(N, 3);
      Matrix At(3, N);
      Vector b(N);
      for (unsigned i = 0; i < N; ++i) {
        A(i, 0) = points[i].x() - points[0].x();
        A(i, 1) = points[i].z() - points[0].z();
        A(i, 2) = 1.0;
        At(0, i) = A(i, 0);
        At(1, i) = A(i, 1);
        At(2, i) = A(i, 2);
        b[i] = -(points[i].y() - points[0].y());
      }

      const Matrix AtA = prod(At, A);
      const double det = det3x3(AtA);  // always positive for A'*A
      if (det > maxDet) {
        Matrix AtAInv(3, 3);
        const bool test = invert(AtA, AtAInv);
        if (test) {
          maxDet = det;
          Vector x = prod(prod(AtAInv, At), b);
          const double a_b = x[0];
          const double c_b = x[1];
          const double d_b = x[2];

          // a = a_b*b
          // c = c_b*b
          // d = d_b*b
          // a^2 + b^2 + c^2 = 1
          // b^2*(a_b^2 + c_b^2 + 1) = 1

          m_b = 1.0 / sqrt(a_b * a_b + c_b * c_b + 1.0);
          m_a = a_b * m_b;
          m_c = c_b * m_b;
          m_d = d_b * m_b - m_a * points[0].x() - m_b * points[0].y() - m_c * points[0].z();
          foundSolution = true;
        }
      }
    }

    {
      // Ax = b, x = [b/a; c/a; d/a]
      Matrix A(N, 3);
      Matrix At(3, N);
      Vector b(N);
      for (unsigned i = 0; i < N; ++i) {
        A(i, 0) = points[i].y() - points[0].y();
        A(i, 1) = points[i].z() - points[0].z();
        A(i, 2) = 1.0;
        At(0, i) = A(i, 0);
        At(1, i) = A(i, 1);
        At(2, i) = A(i, 2);
        b[i] = -(points[i].x() - points[0].x());
      }

      const Matrix AtA = prod(At, A);
      const double det = det3x3(AtA);  // always positive for A'*A
      if (det > maxDet) {
        Matrix AtAInv(3, 3);
        const bool test = invert(AtA, AtAInv);
        if (test) {
          Vector x = prod(prod(AtAInv, At), b);
          const double b_a = x[0];
          const double c_a = x[1];
          const double d_a = x[2];

          // b = b_a*a
          // c = c_a*a
          // d = d_a*a
          // a^2 + b^2 + c^2 = 1
          // a^2*(b_a^2 + c_a^2 + 1) = 1

          m_a = 1.0 / sqrt(b_a * b_a + c_a * c_a + 1.0);
          m_b = b_a * m_a;
          m_c = c_a * m_a;
          m_d = d_a * m_a - m_a * points[0].x() - m_b * points[0].y() - m_c * points[0].z();
          foundSolution = true;
        }
      }
    }

    if (!foundSolution) {
      //std::stringstream ss;
      //ss << std::fixed << std::setprecision(20) << points << '\n';
      //std::string s = ss.str();
      LOG_AND_THROW("Cannot compute plane for points " << points);
    }

    // get outward normal from vertices, plane outward normal should match sense of vertices
    // this corresponds to the other solution to the sqrt
    boost::optional<Vector3d> outwardNormal = getOutwardNormal(points);
    if (outwardNormal) {
      const double thisDot = m_a * outwardNormal.get().x() + m_b * outwardNormal.get().y() + m_c * outwardNormal.get().z();
      if (thisDot < 0) {
        m_a = -m_a;
        m_b = -m_b;
        m_c = -m_c;
        m_d = -m_d;
      }
    }

    // test that normal has length 1
    const double length = m_a * m_a + m_b * m_b + m_c * m_c;
    constexpr double tol = 0.0001;
    OS_ASSERT(std::fabs(1.0 - length) <= tol);
  }
}

Plane::Plane(double a, double b, double c, double d) : m_a(a), m_b(b), m_c(c), m_d(d) {
  // test that normal has length 1
  const double length = m_a * m_a + m_b * m_b + m_c * m_c;
  constexpr double tol = 0.0001;
  OS_ASSERT(std::fabs(1.0 - length) <= tol);
}

Vector3d Plane::outwardNormal() const {
  return {m_a, m_b, m_c};
}

bool Plane::parallel(const Plane& other, double tol) const {
  // dot product of outward normals should be 1 or negative 1
  const double thisDot = (m_a * other.a() + m_b * other.b() + m_c * other.c());
  const bool result = (std::fabs(thisDot) >= 1.0 - tol);
  return result;
}

// is this plane equal to the other plane
// true if dot product of outward normals is >= (1-tol) and abs(d1-d2) <= tol
bool Plane::equal(const Plane& other, double tol) const {
  const double thisDot = (m_a * other.a() + m_b * other.b() + m_c * other.c());
  const double dist = m_d - other.d();
  const bool result = (thisDot >= 1 - tol) && (std::fabs(dist) <= tol);
  return result;
}

// is this plane reverse equal to the other plane
// true if dot product of outward normals is <= (-1+tol) and abs(d1+d2) <= tol
bool Plane::reverseEqual(const Plane& other, double tol) const {
  const double thisDot = (m_a * other.a() + m_b * other.b() + m_c * other.c());
  const double dist = m_d + other.d();
  const bool result = (thisDot <= -1 + tol) && (std::fabs(dist) <= tol);
  return result;
}

Plane Plane::reversePlane() const {
  return {-m_a, -m_b, -m_c, -m_d};
}

Point3d Plane::project(const Point3d& point) const {
  // http://www.9math.com/book/projection-point-plane
  const double u = point.x();
  const double v = point.y();
  const double w = point.z();

  const double num = m_a * u + m_b * v + m_c * w + m_d;
  const double den = m_a * m_a + m_b * m_b + m_c * m_c;  // this should always be 1.0
  const double ratio = num / den;

  const double x = u - m_a * ratio;
  const double y = v - m_b * ratio;
  const double z = w - m_c * ratio;

  return {x, y, z};
}

std::vector<Point3d> Plane::project(const std::vector<Point3d>& points) const {
  std::vector<Point3d> result;
  result.reserve(points.size());
  for (const Point3d& point : points) {
    result.push_back(this->project(point));
  }
  return result;
}

bool Plane::pointOnPlane(const Point3d& point, double tol) const {
  // project point to plane
  const Point3d projected = project(point);

  // get distance
  const Vector3d diff = point - projected;
  const double dist = diff.length();

  return (dist <= tol);
}

double Plane::a() const {
  return m_a;
}

double Plane::b() const {
  return m_b;
}

double Plane::c() const {
  return m_c;
}

double Plane::d() const {
  return m_d;
}

Point3d Plane::anyPointOnPlane() const {

  if (std::abs(m_d) < 0.0001) {
    return {0.0, 0.0, 0.0};
  }
  //  a*x + b*y + c*z + d = 0, any point that satisfies this equation is on the plane.
  if (std::abs(m_a) > 0.0) {
    return {-m_d / m_a, 0.0, 0.0};
  }
  if (std::abs(m_b) > 0.0) {
    return {0.0, -m_d / m_b, 0.0};
  }

  return {0.0, 0.0, -m_d / m_c};
}

boost::optional<Point3d> Plane::rayIntersection(const Point3d& rayOrigin, const Vector3d& rayDirection, bool enforceDirectionOfPlane) const {

  // Any point on plane p = (x, y, z):
  // a*x + b*y +c*z + d = 0
  // <=> Pn · p + d = 0, where Pn is the plane outwardNormal (1)
  //
  // Any point on a ray, given it's origin r0 (X0, Y0, Z0) and the direction rD (Xd, Yd, Zd), satisfies this equation
  // r(t) = r0 + t * rD, **with t > 0**  (2)
  // (t > 0 because, directed ray, otherwise, it's a line)
  //
  // If there is an intersection, then this equation must be satisfied:
  // Pn · r(t) + d = 0
  // <=> Pn · R0 + t * (Pn·Rd) + d = 0
  // <=> t = -((Pn · R0) + d) / (Pn · Rd),  **With t > 0**

  const Point3d p0{};

  auto thisOutwardNormal = outwardNormal();

  const double vd = thisOutwardNormal.dot(rayDirection);

  if (std::abs(vd) < 0.001) {
    LOG(Debug, "Plane is orthogonal to Ray, no intersection possible");
    return {};
  }
  if (vd > 0) {
    if (enforceDirectionOfPlane) {
      LOG(Debug, "Plane normal and ray are poiting in the same direction...");
      return {};
    } else {
      LOG(Trace, "Plane normal and ray are poiting in the same direction...");
    }
  } else {
    LOG(Trace, "Plane normal and ray are poiting in the opposite direction...");
  }

  const double v0 = -(thisOutwardNormal.dot(rayOrigin - p0) + m_d);
  const double t = v0 / vd;
  LOG(Trace, "vd=" << vd << ", v0=" << v0 << ", t=" << t);
  if (t > 0.0) {
    return rayOrigin + t * rayDirection;
  }

  return {};
}

std::ostream& operator<<(std::ostream& os, const Plane& plane) {
  // os << fmt::format("Plane: {:g}x {:+g}y {:+g}z {:+g} = 0", plane.a(), plane.b(), plane.c(), plane.d());
  os << "[" << plane.a() << ", " << plane.b() << ", " << plane.c() << ", " << plane.d() << "]";
  return os;
}

}  // namespace openstudio
