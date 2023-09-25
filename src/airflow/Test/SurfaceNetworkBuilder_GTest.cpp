/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "AirflowFixture.hpp"

#include "../SurfaceNetworkBuilder.hpp"

#include "../../model/Model.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/Surface.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../osversion/VersionTranslator.hpp"
#include "../../utilities/idf/Handle.hpp"

#include "DemoModel.hpp"

#include <resources.hxx>

class SurfaceCounter : public openstudio::airflow::SurfaceNetworkBuilder
{
 public:
  SurfaceCounter() : openstudio::airflow::SurfaceNetworkBuilder(nullptr) {
    clear();
  }

  virtual void clear() override {
    interiorSurface = 0;
    interiorSubSurface = 0;
    exteriorSurface = 0;
    exteriorSubSurface = 0;
  }

  int interiorSurface;
  int exteriorSurface;
  int interiorSubSurface;
  int exteriorSubSurface;

 protected:
  virtual bool linkExteriorSurface(openstudio::model::ThermalZone /*zone*/, openstudio::model::Space /*space*/,
                                   openstudio::model::Surface /*surface*/) override {
    exteriorSurface++;
    return true;
  }
  virtual bool linkExteriorSubSurface(openstudio::model::ThermalZone /*zone*/, openstudio::model::Space /*space*/,
                                      openstudio::model::Surface /*surface*/, openstudio::model::SubSurface /*subSurface*/) override {
    exteriorSubSurface++;
    return true;
  }
  virtual bool linkInteriorSurface(openstudio::model::ThermalZone /*zone*/, openstudio::model::Space /*space*/,
                                   openstudio::model::Surface /*surface*/, openstudio::model::Surface /*adjacentSurface*/,
                                   openstudio::model::Space /*adjacentSpace*/, openstudio::model::ThermalZone /*adjacentZone*/) override {
    interiorSurface++;
    return true;
  }
  virtual bool linkInteriorSubSurface(openstudio::model::ThermalZone /*zone*/, openstudio::model::Space /*space*/,
                                      openstudio::model::Surface /*surface*/, openstudio::model::SubSurface /*subSurface*/,
                                      openstudio::model::SubSurface /*adjacentSubSurface*/, openstudio::model::Surface /*adjacentSurface*/,
                                      openstudio::model::Space /*adjacentSpace*/, openstudio::model::ThermalZone /*adjacentZone*/) override {
    interiorSubSurface++;
    return true;
  }
};

TEST_F(AirflowFixture, SurfaceNetworkBuilder_DemoModel_2012) {
  openstudio::path modelPath = (resourcesPath() / openstudio::toPath("contam") / openstudio::toPath("CONTAMTemplate.osm"));
  openstudio::osversion::VersionTranslator vt;
  boost::optional<openstudio::model::Model> optionalModel = vt.loadModel(modelPath);
  ASSERT_TRUE(optionalModel);
  openstudio::model::Model model = optionalModel.get();

  boost::optional<openstudio::model::Model> demoModel = buildDemoModel2012(model);

  ASSERT_TRUE(demoModel);

  SurfaceCounter counter;
  counter.build(demoModel.get());

  EXPECT_EQ(5, counter.interiorSurface);
  EXPECT_EQ(12, counter.exteriorSurface);
  EXPECT_EQ(0, counter.interiorSubSurface);
  EXPECT_EQ(0, counter.exteriorSubSurface);
}

TEST_F(AirflowFixture, SurfaceNetworkBuilder_DemoModel_2014) {
  openstudio::path modelPath = (resourcesPath() / openstudio::toPath("contam") / openstudio::toPath("CONTAMTemplate.osm"));
  openstudio::osversion::VersionTranslator vt;
  boost::optional<openstudio::model::Model> optionalModel = vt.loadModel(modelPath);
  ASSERT_TRUE(optionalModel);
  openstudio::model::Model model = optionalModel.get();

  boost::optional<openstudio::model::Model> demoModel = buildDemoModel2014(model);

  ASSERT_TRUE(demoModel);

  SurfaceCounter counter;
  EXPECT_TRUE(counter.build(demoModel.get()));

  EXPECT_EQ(5, counter.interiorSurface);
  EXPECT_EQ(12, counter.exteriorSurface);
  EXPECT_EQ(3, counter.interiorSubSurface);
  EXPECT_EQ(28, counter.exteriorSubSurface);
}
