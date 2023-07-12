/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_GEOMETRY_TEST_GEOMETRYFIXTURE_HPP
#define UTILITIES_GEOMETRY_TEST_GEOMETRYFIXTURE_HPP

#include <gtest/gtest.h>
#include "../Geometry.hpp"
#include "../../core/Logger.hpp"
#include "../../core/FileLogSink.hpp"

namespace openstudio {
class Point3d;
class Vector3d;
}  // namespace openstudio

bool pointEqual(const openstudio::Point3d& a, const openstudio::Point3d& b);

bool pointsEqual(const std::vector<openstudio::Point3d>& a, const std::vector<openstudio::Point3d>& b);

bool vectorEqual(const openstudio::Vector3d& a, const openstudio::Vector3d& b);

double totalArea(const std::vector<std::vector<openstudio::Point3d>>& polygons);

bool checkNormals(const openstudio::Vector3d& normal, const std::vector<std::vector<openstudio::Point3d>>& polygons);

class GeometryFixture : public ::testing::Test
{
 protected:
  // initialize for each test
  virtual void SetUp() override;

  // tear down after for each test
  virtual void TearDown() override;

  // initialize static members
  static void SetUpTestSuite();

  // tear down static members
  static void TearDownTestSuite();

 public:
  // set up logging
  REGISTER_LOGGER("GeometryFixture");

  static boost::optional<openstudio::FileLogSink> logFile;
};

#endif  // UTILITIES_GEOMETRY_TEST_GEOMETRYFIXTURE_HPP
