/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "AirflowFixture.hpp"

#include "../contam/ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/Surface.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/BuildingStory.hpp"
#include "../../model/BuildingStory_Impl.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../model/ThermostatSetpointDualSetpoint.hpp"
#include "../../model/ThermostatSetpointDualSetpoint_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/SizingZone.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/HVACTemplates.hpp"
#include "../../model/SetpointManagerSingleZoneReheat.hpp"
#include "../../model/SetpointManagerSingleZoneReheat_Impl.hpp"
#include "../../osversion/VersionTranslator.hpp"
#include "../../utilities/idf/Handle.hpp"

#include "DemoModel.hpp"

#include <resources.hxx>

#include <sstream>
#include <QVector>

TEST_F(AirflowFixture, ForwardTranslator_exampleModel)
{
  openstudio::model::Model model = openstudio::model::exampleModel();

  openstudio::path p = openstudio::toPath("exampleModel.prj");

  bool test = openstudio::contam::ForwardTranslator::modelToPrj(model, p);

  EXPECT_TRUE(test);
}

TEST_F(AirflowFixture, ForwardTranslator_DemoModel_2012)
{
  openstudio::path modelPath = (resourcesPath() / openstudio::toPath("contam") / openstudio::toPath("CONTAMTemplate.osm"));
  openstudio::osversion::VersionTranslator vt;
  boost::optional<openstudio::model::Model> optionalModel = vt.loadModel(modelPath);
  ASSERT_TRUE(optionalModel);
  openstudio::model::Model model = optionalModel.get();

  boost::optional<openstudio::model::Model> demoModel = buildDemoModel2012(model);

  // Write out the model
  openstudio::path outPath("CONTAMDemo2012.osm");
  EXPECT_TRUE(demoModel->save(outPath, true));

  ASSERT_TRUE(demoModel);

  openstudio::contam::ForwardTranslator translator;

  boost::optional<openstudio::contam::IndexModel> prjModel = translator.translateModel(demoModel.get());

  ASSERT_TRUE(prjModel);

  // 6 Zones
  EXPECT_EQ(6,prjModel->zones().size());
  int systemZoneCount=0;
  int interiorZoneCount=0;
  for(const openstudio::contam::Zone& zone : prjModel->zones()) {
    if(zone.system()) {
      systemZoneCount++;
    } else if(zone.variablePressure() && zone.variableContaminants()) {
      interiorZoneCount++;
    }
  }
  EXPECT_EQ(2,systemZoneCount);
  EXPECT_EQ(4,interiorZoneCount);
  // 26 Paths
  std::vector<double> interiorMult({ 9, 21, 30 });
  std::vector<double> exteriorRoofMult({ 30, 70, 136 });
  std::vector<double> exteriorWallMult({ 9, 21, 24, 30, 51 });
  EXPECT_EQ(26,prjModel->airflowPaths().size());
  int exhaustPathCount=0;
  int systemPathCount=0;
  int windPressurePathCount=0;
  int outsideAirPathCount=0;
  int recirculationPathCount=0;
  int plainPathCount=0;
  for(openstudio::contam::AirflowPath afp : prjModel->airflowPaths()) {
    if(afp.system()) {
      systemPathCount++;
    } else if(afp.windPressure()) {
      // This should be an exterior flow path
      if(afp.pe() == 5) {
        EXPECT_FALSE(std::find(exteriorWallMult.begin(), exteriorWallMult.end(), afp.mult()) == exteriorWallMult.end());
      } else {
        EXPECT_FALSE(std::find(exteriorRoofMult.begin(), exteriorRoofMult.end(), afp.mult()) == exteriorRoofMult.end());
      }
      windPressurePathCount++;
    } else if(afp.exhaust()) {
      exhaustPathCount++;
    } else if(afp.outsideAir()) {
      outsideAirPathCount++;
    } else if(afp.recirculation()) {
      recirculationPathCount++;
    } else {
      // This is an interior flow path
      EXPECT_FALSE(std::find(interiorMult.begin(), interiorMult.end(), afp.mult()) == interiorMult.end());
      plainPathCount++;
    }
  }
  EXPECT_EQ(6,systemPathCount);
  EXPECT_EQ(12,windPressurePathCount);
  EXPECT_EQ(5,plainPathCount);
  EXPECT_EQ(1,recirculationPathCount);
  EXPECT_EQ(1,outsideAirPathCount);
  EXPECT_EQ(1,exhaustPathCount);

  // Check out the zones using the zone map
  std::vector<std::string> zoneNames({ "Library Zone", "Office 1 Zone", "Office 2 Zone", "Hallway Zone" });
  std::map<std::string,int> exteriorWallCount;
  exteriorWallCount["Library Zone"] = 4;
  exteriorWallCount["Office 1 Zone"] = 3;
  exteriorWallCount["Office 2 Zone"] = 3;
  exteriorWallCount["Hallway Zone"] = 2;

  std::map <openstudio::Handle, int> zoneMap = translator.zoneMap();
  EXPECT_EQ(4,zoneMap.size());

  std::vector<std::vector<int> > exteriorFlowPaths = prjModel->zoneExteriorFlowPaths();
  EXPECT_EQ(6,exteriorFlowPaths.size());

  for(const openstudio::model::ThermalZone& thermalZone : model.getConcreteModelObjects<openstudio::model::ThermalZone>()) {
    ASSERT_FALSE(std::find(zoneNames.begin(), zoneNames.end(), thermalZone.name().get()) == zoneNames.end());
    unsigned zoneNumber = zoneMap[thermalZone.handle()];
    ASSERT_TRUE(zoneNumber > 0);
    ASSERT_TRUE(zoneNumber <= prjModel->zones().size());
    int zoneExteriorWallCount = exteriorFlowPaths[zoneNumber-1].size();
    EXPECT_EQ(exteriorWallCount[thermalZone.name().get()],zoneExteriorWallCount);
  }

  // Try setting some values to make sure things work
  EXPECT_TRUE(prjModel->setDef_T(297.15));
  EXPECT_TRUE(prjModel->setDef_T("297.15"));
  EXPECT_FALSE(prjModel->setDef_T("twoninetysevenpointonefive"));
  EXPECT_EQ(297.15,prjModel->def_T());
}

TEST_F(AirflowFixture, ForwardTranslator_DemoModel_2012_BadStories)
{
  openstudio::path modelPath = (resourcesPath() / openstudio::toPath("contam") / openstudio::toPath("CONTAMTemplate.osm"));
  openstudio::osversion::VersionTranslator vt;
  boost::optional<openstudio::model::Model> optionalModel = vt.loadModel(modelPath);
  ASSERT_TRUE(optionalModel);
  openstudio::model::Model model = optionalModel.get();

  boost::optional<openstudio::model::Model> demoModel = buildDemoModel2012(model);

  ASSERT_TRUE(demoModel);

  auto stories = demoModel.get().getConcreteModelObjects<openstudio::model::BuildingStory>();

  EXPECT_EQ(1, stories.size());

  stories[0].resetNominalZCoordinate();

  openstudio::contam::ForwardTranslator translator;
  boost::optional<openstudio::contam::IndexModel> prjModel = translator.translateModel(demoModel.get());
  EXPECT_FALSE(prjModel);

  stories[0].setNominalZCoordinate(0.0);
  stories[0].resetNominalZCoordinate();

  prjModel = translator.translateModel(demoModel.get());
  EXPECT_FALSE(prjModel);

  stories[0].remove();
  prjModel = translator.translateModel(demoModel.get());
  EXPECT_FALSE(prjModel);

}

TEST_F(AirflowFixture, ForwardTranslator_DemoModel_2014)
{
  openstudio::path modelPath = (resourcesPath() / openstudio::toPath("contam") / openstudio::toPath("CONTAMTemplate.osm"));
  openstudio::osversion::VersionTranslator vt;
  boost::optional<openstudio::model::Model> optionalModel = vt.loadModel(modelPath);
  ASSERT_TRUE(optionalModel);
  openstudio::model::Model model = optionalModel.get();

  boost::optional<openstudio::model::Model> demoModel = buildDemoModel2014(model);

  ASSERT_TRUE(demoModel);

  // Write out the model
  openstudio::path outPath("CONTAMDemo2014.osm");
  EXPECT_TRUE(demoModel->save(outPath, true));

  openstudio::contam::PlrLeak2 window(OPNG, "WNOO6C_CMX", "Operable window, Building C, maximum (from C&IMisc.lb3)",
    "4.98716e-008", "0.00039745", "0.65", "1", "4", "0.000346");

  openstudio::contam::ForwardTranslator translator;

  boost::optional<openstudio::contam::IndexModel> prjModel = translator.translateModel(demoModel.get());

  ASSERT_TRUE(prjModel);

}