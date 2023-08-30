/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/ZoneHVACBaseboardRadiantConvectiveWater.hpp"
#include "../../model/ZoneHVACBaseboardRadiantConvectiveWater_Impl.hpp"
#include "../../model/CoilHeatingWaterBaseboardRadiant.hpp"
#include "../../model/CoilHeatingWaterBaseboardRadiant_Impl.hpp"

#include "../../model/Model.hpp"
#include "../../model/HVACComponent.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Schedule.hpp"
#include "../../utilities/geometry/Point3d.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/geometry/Point3d.hpp"

#include <utilities/idd/ZoneHVAC_Baseboard_RadiantConvective_Water_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_Baseboard_RadiantConvective_Water_Design_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ZoneHVACBaseboardRadiantConvectiveWater) {
  //make the example model
  Model m = model::exampleModel();

  ZoneHVACBaseboardRadiantConvectiveWater baseboard(m);
  auto coil = baseboard.heatingCoil().cast<CoilHeatingWaterBaseboardRadiant>();

  PlantLoop p(m);
  EXPECT_TRUE(p.addDemandBranchForComponent(coil));

  Point3dVector floorPrint{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };
  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, m);
  ASSERT_TRUE(space1);
  auto surfaces = space1->surfaces();
  EXPECT_EQ(6u, surfaces.size());

  // Space needs to be in a ThermalZone or it's not translated
  ThermalZone z(m);
  EXPECT_TRUE(space1->setThermalZone(z));

  EXPECT_TRUE(baseboard.addToThermalZone(z));

  // Some tweaks to disambiguate the ft tests later
  baseboard.setName("My Baseboard");
  EXPECT_TRUE(baseboard.setFractionRadiant(0.4));
  EXPECT_TRUE(baseboard.setFractionofRadiantEnergyIncidentonPeople(0.3));
  EXPECT_TRUE(coil.setMaximumWaterFlowRate(1.0));

  // Translate
  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  ASSERT_EQ(1u, w.getObjectsByType(IddObjectType::ZoneHVAC_Baseboard_RadiantConvective_Water_Design).size());
  WorkspaceObjectVector idfBaseboards = w.getObjectsByType(IddObjectType::ZoneHVAC_Baseboard_RadiantConvective_Water);
  ASSERT_EQ(1u, idfBaseboards.size());
  WorkspaceObject idfBaseboard(idfBaseboards[0]);

  // Name
  EXPECT_EQ(baseboard.nameString(), idfBaseboard.getString(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::Name).get());
  // Design Object: see below
  // Availability Schedule Name
  EXPECT_EQ(baseboard.availabilitySchedule().nameString(),
            idfBaseboard.getString(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::AvailabilityScheduleName).get());
  // Inlet Node Name
  EXPECT_FALSE(idfBaseboard.getString(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::InletNodeName).get().empty());
  // Outlet Node Name
  EXPECT_FALSE(idfBaseboard.getString(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::OutletNodeName).get().empty());
  // Rated Average Water Temperature
  EXPECT_EQ(coil.ratedAverageWaterTemperature(),
            idfBaseboard.getDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::RatedAverageWaterTemperature).get());
  // Rated Water Mass Flow Rate
  EXPECT_EQ(coil.ratedWaterMassFlowRate(), idfBaseboard.getDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::RatedWaterMassFlowRate).get());
  // Heating Design Capacity
  EXPECT_TRUE(
    openstudio::istringEqual("autosize", idfBaseboard.getString(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::HeatingDesignCapacity).get()));
  // Maximum Water Flow Rate
  EXPECT_EQ(coil.maximumWaterFlowRate().get(), idfBaseboard.getDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::MaximumWaterFlowRate).get());

  // Surface 1 Name
  // Fraction of Radiant Energy to Surface 1
  EXPECT_EQ(surfaces.size(), idfBaseboard.numExtensibleGroups());

  // We check that it does have a design object assigned
  ASSERT_TRUE(idfBaseboard.getTarget(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::DesignObject));
  WorkspaceObject idfDesign = idfBaseboard.getTarget(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::DesignObject).get();
  // Name
  EXPECT_EQ("My Baseboard Design", idfDesign.nameString());
  // Heating Design Capacity Method
  EXPECT_EQ(coil.heatingDesignCapacityMethod(),
            idfDesign.getString(ZoneHVAC_Baseboard_RadiantConvective_Water_DesignFields::HeatingDesignCapacityMethod).get());
  // Heating Design Capacity Per Floor Area
  EXPECT_EQ(coil.heatingDesignCapacityPerFloorArea(),
            idfDesign.getDouble(ZoneHVAC_Baseboard_RadiantConvective_Water_DesignFields::HeatingDesignCapacityPerFloorArea).get());
  // Fraction of Autosized Heating Design Capacity
  EXPECT_EQ(coil.fractionofAutosizedHeatingDesignCapacity(),
            idfDesign.getDouble(ZoneHVAC_Baseboard_RadiantConvective_Water_DesignFields::FractionofAutosizedHeatingDesignCapacity).get());
  // Convergence Tolerance
  EXPECT_EQ(coil.convergenceTolerance(), idfDesign.getDouble(ZoneHVAC_Baseboard_RadiantConvective_Water_DesignFields::ConvergenceTolerance).get());
  // Fraction Radiant
  EXPECT_EQ(baseboard.fractionRadiant(), idfDesign.getDouble(ZoneHVAC_Baseboard_RadiantConvective_Water_DesignFields::FractionRadiant).get());
  // Fraction of Radiant Energy Incident on People
  EXPECT_EQ(baseboard.fractionofRadiantEnergyIncidentonPeople(),
            idfDesign.getDouble(ZoneHVAC_Baseboard_RadiantConvective_Water_DesignFields::FractionofRadiantEnergyIncidentonPeople).get());
}
