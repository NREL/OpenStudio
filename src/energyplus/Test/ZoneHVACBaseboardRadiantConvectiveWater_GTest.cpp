/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
