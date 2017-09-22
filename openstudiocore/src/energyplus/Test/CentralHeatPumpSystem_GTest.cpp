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
//#include "../../model/Building.hpp"
//#include "../../model/Building_Impl.hpp"
//#include "../../model/Site.hpp"
//#include "../../model/Site_Impl.hpp"
//#include "../../model/WeatherFile.hpp"
//#include "../../model/WeatherFile_Impl.hpp"
//#include "../../model/ThermalZone.hpp"
//#include "../../model/Component.hpp"
//#include "../../model/Curve.hpp"
//#include "../../model/Curve_Impl.hpp"
//#include "../../model/CurveQuadratic.hpp"
//#include "../../model/CurveQuadratic_Impl.hpp"

#include "../../model/CentralHeatPumpSystem.hpp"
#include "../../model/CentralHeatPumpSystemModule.hpp"
#include "../../model/ChillerHeaterPerformanceElectricEIR.hpp"

#include "../../model/PlantLoop.hpp"
#include "../../model/Node.hpp"
// I think I'll need to cast to a node
#include "../../model/Node_Impl.hpp"

#include "../../model/Schedule.hpp"


#include "../../model/Version.hpp"
#include "../../model/Version_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/UUID.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/IdfObject.hpp"

// OS FieldEnums
#include <utilities/idd/OS_CentralHeatPumpSystem_FieldEnums.hxx>
#include <utilities/idd/OS_CentralHeatPumpSystem_Module_FieldEnums.hxx>
#include <utilities/idd/OS_ChillerHeaterPerformance_Electric_EIR_FieldEnums.hxx>


// E+ FieldEnums
#include <utilities/idd/CentralHeatPumpSystem_FieldEnums.hxx>

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


TEST_F(EnergyPlusFixture,ForwardTranslatorCentralHeatPumpSystem) {

  Model model;

  CentralHeatPumpSystem central_hp(model);
  ASSERT_TRUE(central_hp.setAncillaryPower(0.7));
  EXPECT_FALSE(central_hp.ancillaryOperationSchedule());
  Schedule schedule = model.alwaysOnDiscreteSchedule();

  // Return type: bool
  ASSERT_TRUE(central_hp.setAncillaryOperationSchedule(schedule));
  EXPECT_TRUE(central_hp.ancillaryOperationSchedule());

  // First module has one
  CentralHeatPumpSystemModule central_hp_module(model);
  central_hp.addModule(central_hp_module);
  EXPECT_EQ(1, central_hp_module.numberofChillerHeaterModules());

  // Second has 2
  CentralHeatPumpSystemModule central_hp_module2(model);
  central_hp.addModule(central_hp_module2);
  ASSERT_TRUE(central_hp_module2.setNumberofChillerHeaterModules(2));


  ASSERT_EQ( (unsigned)2, central_hp.modules().size() );


  // Connect tthe CentralHP to three plant loops
  // CoolingLoop: on the supply side
  {
    PlantLoop coolingPlant(model);
    auto node = coolingPlant.supplyOutletNode();
    EXPECT_TRUE(central_hp.addToNode(node));
  }

  // SourceLoop: on the demand side
  {
    PlantLoop sourcePlant(model);
    auto node = sourcePlant.demandInletNode();
    EXPECT_TRUE(central_hp.addToNode(node));
  }
  // HeatingLoop: on the supply side
  {
    PlantLoop heatingPlant(model);
    auto node = heatingPlant.supplyOutletNode();
    EXPECT_TRUE(central_hp.addToTertiaryNode(node));
  }

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::CentralHeatPumpSystem).size());

  IdfObject i_central_hp = workspace.getObjectsByType(IddObjectType::CentralHeatPumpSystem)[0];

  ASSERT_EQ(OS_CentralHeatPumpSystemFields::CoolingLoopInletNodeName, CentralHeatPumpSystemFields::CoolingLoopInletNodeName);
  ASSERT_EQ(OS_CentralHeatPumpSystemFields::CoolingLoopOutletNodeName, CentralHeatPumpSystemFields::CoolingLoopOutletNodeName);

  ASSERT_EQ(OS_CentralHeatPumpSystemFields::SourceLoopInletNodeName, CentralHeatPumpSystemFields::SourceLoopInletNodeName);
  ASSERT_EQ(OS_CentralHeatPumpSystemFields::SourceLoopOutletNodeName, CentralHeatPumpSystemFields::SourceLoopOutletNodeName);

  ASSERT_EQ(OS_CentralHeatPumpSystemFields::HeatingLoopInletNodeName, CentralHeatPumpSystemFields::HeatingLoopInletNodeName);
  ASSERT_EQ(OS_CentralHeatPumpSystemFields::HeatingLoopOutletNodeName, CentralHeatPumpSystemFields::HeatingLoopOutletNodeName);


  model.save(toPath("./ft_central_hp.osm"), true);
  workspace.save(toPath("./ft_central_hp.idf"), true);

}

