/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/OutputVariable.hpp"
#include "../../model/OutputVariable_Impl.hpp"
#include "../../model/OutputMeter.hpp"
#include "../../model/OutputMeter_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/EnergyManagementSystemSensor.hpp"
#include "../../model/EnergyManagementSystemSensor_Impl.hpp"

#include "../../model/Version.hpp"
#include "../../model/Version_Impl.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/UUID.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/IdfObject.hpp"

#include <utilities/idd/OS_EnergyManagementSystem_Sensor_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_Sensor_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <boost/algorithm/string/predicate.hpp>

#include <QThread>

#include <resources.hxx>

#include <sstream>

#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;


TEST_F(EnergyPlusFixture,ForwardTranslator_EMS) {
  //Model model = exampleModel();

  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // add Site Outdoor Air Drybulb Temperature
  OutputVariable siteOutdoorAirDrybulbTemperature("Site Outdoor Air Drybulb Temperature", model);
  EXPECT_EQ("*", siteOutdoorAirDrybulbTemperature.keyValue());
  EXPECT_EQ("Site Outdoor Air Drybulb Temperature", siteOutdoorAirDrybulbTemperature.variableName());

  // add sensor
  EnergyManagementSystemSensor OATdbSensor(model);
  OATdbSensor.setName("OATdb Sensor");
  OATdbSensor.setOutputVariable(siteOutdoorAirDrybulbTemperature);

  EXPECT_EQ("OATdb Sensor", OATdbSensor.nameString());
  EXPECT_EQ(siteOutdoorAirDrybulbTemperature.handle(), OATdbSensor.outputVariable().get().handle());
  EXPECT_EQ(siteOutdoorAirDrybulbTemperature, OATdbSensor.outputVariable());
  EXPECT_EQ("", OATdbSensor.keyName().get());

  // add Zone Lights Electric Power to both zones
  OutputVariable lightsElectricPower("Zone Lights Electric Power", model);
  EXPECT_EQ("*", lightsElectricPower.keyValue());
  EXPECT_EQ("Zone Lights Electric Power", lightsElectricPower.variableName());

  // add light sensor on zone1
  EnergyManagementSystemSensor lights(model);
  lights.setName("Light Sensor");
  lights.setOutputVariable(lightsElectricPower);
  lights.setKeyName(zone1.name().get());

  EXPECT_EQ(zone1.name().get(), lights.keyName().get());
  EXPECT_EQ("Light Sensor", lights.nameString());


  //EXPECT_TRUE(model.getOptionalUniqueModelObject<Version>()) << "Example model does not include a Version object.";
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u,workspace.getObjectsByType(IddObjectType::Version).size());

  model.save(toPath("./EMS_example.osm"), true);
  workspace.save(toPath("./EMS_example.idf"), true);
}

/*
TEST_F(EnergyPlusFixture,ForwardTranslatorTest_TranslateAirLoopHVAC) {
  openstudio::model::Model model;
  EXPECT_TRUE(model.getOptionalUniqueModelObject<Version>()) << "Blank model does not include a Version object.";

  ThermalZone zone1(model);
  ThermalZone zone2(model);
  ThermalZone zone3(model);

  openstudio::model::AirLoopHVAC airLoopHVAC(model);
  airLoopHVAC.addBranchForZone(zone1, boost::none);
  airLoopHVAC.addBranchForZone(zone2, boost::none);
  airLoopHVAC.addBranchForZone(zone3, boost::none);

  ForwardTranslator trans;

  openstudio::Workspace workspace = trans.translateModel(model);
  EXPECT_EQ(1u,workspace.getObjectsByType(IddObjectType::Version).size());

  ASSERT_NE(unsigned(0),workspace.objects().size());

  openstudio::path outDir = resourcesPath() / openstudio::toPath("airLoopHVAC.idf");
  boost::filesystem::ofstream ofs(outDir);
  workspace.toIdfFile().print(ofs);
  ofs.close();
}
*/
