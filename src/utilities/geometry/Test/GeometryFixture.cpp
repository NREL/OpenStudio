/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "GeometryFixture.hpp"

#include "../Point3d.hpp"
#include "../Vector3d.hpp"

using openstudio::Logger;
using openstudio::FileLogSink;
using openstudio::toPath;
using openstudio::Point3d;
using openstudio::Vector3d;

bool pointEqual(const openstudio::Point3d& a, const openstudio::Point3d& b) {
  Vector3d diff = a - b;
  return diff.length() <= 0.0001;
}

bool pointsEqual(const std::vector<openstudio::Point3d>& a, const std::vector<openstudio::Point3d>& b) {
  if (a.size() != b.size()) {
    return false;
  }

  for (unsigned i = 0; i < a.size(); ++i) {
    if (!pointEqual(a[i], b[i])) {
      return false;
    }
  }

  return true;
}

bool vectorEqual(const openstudio::Vector3d& a, const openstudio::Vector3d& b) {
  Vector3d diff = a - b;
  return diff.length() <= 0.0001;
}

double totalArea(const std::vector<std::vector<Point3d>>& polygons) {
  double result = 0.0;
  for (const auto& polygon : polygons) {
    boost::optional<double> a = getArea(polygon);
    if (a) {
      result += a.get();
    }
  }
  return result;
}

bool checkNormals(const openstudio::Vector3d& normal, const std::vector<std::vector<Point3d>>& polygons) {
  for (const auto& polygon : polygons) {
    boost::optional<Vector3d> a = getOutwardNormal(polygon);
    if (a) {
      if (!vectorEqual(normal, *a)) {
        return false;
      }
    } else {
      return false;
    }
  }
  return true;
}

// initialize for each test
void GeometryFixture::SetUp() {}

// tear down after for each test
void GeometryFixture::TearDown() {}

// initialize static members
void GeometryFixture::SetUpTestSuite() {
  logFile = FileLogSink(toPath("./GeometryFixture.log"));
  logFile->setLogLevel(Debug);
}

// tear down static members
void GeometryFixture::TearDownTestSuite() {
  logFile->disable();
}

boost::optional<openstudio::FileLogSink> GeometryFixture::logFile;
