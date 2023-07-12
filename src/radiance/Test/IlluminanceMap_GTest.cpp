/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include <radiance/IlluminanceMap.h>

#include <utilities/application/Application.h>

#include <resources.h>

using namespace std;
using namespace boost;
using namespace radiance;
using namespace utilities::data;
using namespace utilities::plot;

///////////////////////////////////////////////////////////////////////////////
// *** BEGIN FIXTURE ***
///////////////////////////////////////////////////////////////////////////////

class RadIlluminanceMapFixture : public ::testing::Test
{
 protected:
  // initialize for each test
  virtual void SetUp() {}

  // tear down after for each test
  virtual void TearDown() {}

  // initialize static members
  static void SetUpTestSuite() {
    openstudio::filesystem::wpath path = ep_resources_path() / L"Daylighting/wp_6_1_12_1_.out";
    outFile = IlluminanceMap::create(path);
  }

  // tear down static members
  static void TearDownTestSuite() {}

 public:
  // pointer to IlluminanceMap
  static IlluminanceMap::Ptr outFile;
};

// define static storage
IlluminanceMap::Ptr RadIlluminanceMapFixture::outFile;

///////////////////////////////////////////////////////////////////////////////
// *** BEGIN TESTS ***
///////////////////////////////////////////////////////////////////////////////

TEST_F(RadIlluminanceMapFixture, IlluminanceMap) {
  ASSERT_TRUE(outFile);
}
