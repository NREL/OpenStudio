/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/ZoneHVACBaseboardRadiantConvectiveSteam.hpp"
#include "../../model/ZoneHVACBaseboardRadiantConvectiveSteam_Impl.hpp"
#include "../../model/CoilHeatingSteamBaseboardRadiant.hpp"
#include "../../model/CoilHeatingSteamBaseboardRadiant_Impl.hpp"

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

#include <utilities/idd/ZoneHVAC_Baseboard_RadiantConvective_Steam_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_Baseboard_RadiantConvective_Steam_Design_FieldEnums.hxx>
#include <utilities/idd/Pipe_Adiabatic_FieldEnums.hxx>
#include <utilities/idd/Pipe_Adiabatic_Steam_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ZoneHVACBaseboardRadiantConvectiveSteam) {
  //make the example model
  Model m = model::exampleModel();

  ZoneHVACBaseboardRadiantConvectiveSteam baseboard(m);
  auto coil = baseboard.heatingCoil().cast<CoilHeatingSteamBaseboardRadiant>();

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
  EXPECT_TRUE(coil.setMaximumSteamFlowRate(1.0));

  // Translate
  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  ASSERT_EQ(1u, w.getObjectsByType(IddObjectType::ZoneHVAC_Baseboard_RadiantConvective_Steam_Design).size());
  ASSERT_EQ(0u, w.getObjectsByType(IddObjectType::Pipe_Adiabatic).size());
  ASSERT_EQ(6u, w.getObjectsByType(IddObjectType::Pipe_Adiabatic_Steam).size());
  ASSERT_EQ(1u, w.getObjectsByType(IddObjectType::ZoneHVAC_Baseboard_RadiantConvective_Steam_Design).size());
  WorkspaceObjectVector idfBaseboards = w.getObjectsByType(IddObjectType::ZoneHVAC_Baseboard_RadiantConvective_Steam);
  ASSERT_EQ(1u, idfBaseboards.size());
  WorkspaceObject idfBaseboard(idfBaseboards[0]);

  // Name
  EXPECT_EQ(baseboard.nameString(), idfBaseboard.getString(ZoneHVAC_Baseboard_RadiantConvective_SteamFields::Name).get());
  // Design Object: see below
  // Availability Schedule Name
  EXPECT_EQ(baseboard.availabilitySchedule().nameString(),
            idfBaseboard.getString(ZoneHVAC_Baseboard_RadiantConvective_SteamFields::AvailabilityScheduleName).get());
  // Inlet Node Name
  EXPECT_FALSE(idfBaseboard.getString(ZoneHVAC_Baseboard_RadiantConvective_SteamFields::InletNodeName).get().empty());
  // Outlet Node Name
  EXPECT_FALSE(idfBaseboard.getString(ZoneHVAC_Baseboard_RadiantConvective_SteamFields::OutletNodeName).get().empty());
  // Heating Design Capacity
  EXPECT_TRUE(
    openstudio::istringEqual("autosize", idfBaseboard.getString(ZoneHVAC_Baseboard_RadiantConvective_SteamFields::HeatingDesignCapacity).get()));
  // Degree of SubCooling
  EXPECT_EQ(coil.degreeofSubCooling(), idfBaseboard.getDouble(ZoneHVAC_Baseboard_RadiantConvective_SteamFields::DegreeofSubCooling).get());
  // Maximum Steam Flow Rate
  EXPECT_EQ(coil.maximumSteamFlowRate().get(), idfBaseboard.getDouble(ZoneHVAC_Baseboard_RadiantConvective_SteamFields::MaximumSteamFlowRate).get());

  // Surface 1 Name
  // Fraction of Radiant Energy to Surface 1
  EXPECT_EQ(surfaces.size(), idfBaseboard.numExtensibleGroups());

  // We check that it does have a design object assigned
  ASSERT_TRUE(idfBaseboard.getTarget(ZoneHVAC_Baseboard_RadiantConvective_SteamFields::DesignObject));
  WorkspaceObject idfDesign = idfBaseboard.getTarget(ZoneHVAC_Baseboard_RadiantConvective_SteamFields::DesignObject).get();
  // Name
  EXPECT_EQ("My Baseboard Design", idfDesign.nameString());
  // Heating Design Capacity Method
  EXPECT_EQ(coil.heatingDesignCapacityMethod(),
            idfDesign.getString(ZoneHVAC_Baseboard_RadiantConvective_Steam_DesignFields::HeatingDesignCapacityMethod).get());
  // Heating Design Capacity Per Floor Area
  EXPECT_EQ(coil.heatingDesignCapacityPerFloorArea(),
            idfDesign.getDouble(ZoneHVAC_Baseboard_RadiantConvective_Steam_DesignFields::HeatingDesignCapacityPerFloorArea).get());
  // Fraction of Autosized Heating Design Capacity
  EXPECT_EQ(coil.fractionofAutosizedHeatingDesignCapacity(),
            idfDesign.getDouble(ZoneHVAC_Baseboard_RadiantConvective_Steam_DesignFields::FractionofAutosizedHeatingDesignCapacity).get());
  // Convergence Tolerance
  EXPECT_EQ(coil.convergenceTolerance(), idfDesign.getDouble(ZoneHVAC_Baseboard_RadiantConvective_Steam_DesignFields::ConvergenceTolerance).get());
  // Fraction Radiant
  EXPECT_EQ(baseboard.fractionRadiant(), idfDesign.getDouble(ZoneHVAC_Baseboard_RadiantConvective_Steam_DesignFields::FractionRadiant).get());
  // Fraction of Radiant Energy Incident on People
  EXPECT_EQ(baseboard.fractionofRadiantEnergyIncidentonPeople(),
            idfDesign.getDouble(ZoneHVAC_Baseboard_RadiantConvective_Steam_DesignFields::FractionofRadiantEnergyIncidentonPeople).get());
}
