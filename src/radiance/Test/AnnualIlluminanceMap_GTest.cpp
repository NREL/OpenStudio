/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "../AnnualIlluminanceMap.hpp"

#include <resources.hxx>

using namespace std;
using namespace boost;
using namespace openstudio::radiance;
using openstudio::toPath;

///////////////////////////////////////////////////////////////////////////////
// *** BEGIN FIXTURE ***
///////////////////////////////////////////////////////////////////////////////

class RadAnnualIlluminanceMapFixture : public ::testing::Test
{
 protected:
  // initialize for each test
  virtual void SetUp() override {}

  // tear down after for each test
  virtual void TearDown() override {}

  // initialize static members
  static void SetUpTestSuite() {
    openstudio::path path = resourcesPath() / toPath("radiance/Daylighting/annual_day.ill");
    outFile = AnnualIlluminanceMap(path);
  }

  // tear down static members
  static void TearDownTestSuite() {}

 public:
  // pointer to AnnualIlluminanceMap
  static AnnualIlluminanceMap outFile;
};

// define static storage
AnnualIlluminanceMap RadAnnualIlluminanceMapFixture::outFile;

///////////////////////////////////////////////////////////////////////////////
// *** BEGIN TESTS ***
///////////////////////////////////////////////////////////////////////////////

TEST_F(RadAnnualIlluminanceMapFixture, AnnualIlluminanceMap) {}
