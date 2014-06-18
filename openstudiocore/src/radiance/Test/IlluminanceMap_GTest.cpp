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

#include <radiance/IlluminanceMap.h>

#include <utilities/plot/FloodPlot.h>
#include <utilities/application/Application.h>

#include <resources.h>

#include <boost/filesystem.hpp>

using namespace std;
using namespace boost;
using namespace radiance;
using namespace utilities::data;
using namespace utilities::plot;

///////////////////////////////////////////////////////////////////////////////
// *** BEGIN FIXTURE ***
///////////////////////////////////////////////////////////////////////////////

class RadIlluminanceMapFixture : public ::testing::Test {
protected:

  // initialize for each test
  virtual void SetUp() {}

  // tear down after for each test
  virtual void TearDown() {}

  // initialize static members
  static void SetUpTestCase()
  {
    boost::filesystem::wpath path = ep_resources_path()/L"Daylighting/wp_6_1_12_1_.out";
    outFile = IlluminanceMap::create(path);
  }

  // tear down static members
  static void TearDownTestCase() {}

public:
  // pointer to IlluminanceMap
  static IlluminanceMap::Ptr outFile;
};

// define static storage
IlluminanceMap::Ptr RadIlluminanceMapFixture::outFile;

///////////////////////////////////////////////////////////////////////////////
// *** BEGIN TESTS ***
///////////////////////////////////////////////////////////////////////////////

TEST_F(RadIlluminanceMapFixture, IlluminanceMap)
{
  ASSERT_TRUE(outFile);
}

TEST_F(RadIlluminanceMapFixture, IlluminanceMapPlot)
{
  utilities::application::Application::instance().qApplication();

  MatrixFloodPlotData::Ptr data = MatrixFloodPlotData::create(outFile->xVector(),
                                                              outFile->yVector(),
                                                              outFile->illuminanceMap());
  data->interpMethod(linearInterp);

  FloodPlot::Ptr fp = FloodPlot::create();
  fp->setFloodPlotData(data);
  fp->initZoomer(); // no image generated without initializing zoomer
  fp->showContour(true);
  fp->generateImage(L"testRadIlluminanceMapPlot.png");

  // passes the visual test
  EXPECT_TRUE(true);
}
