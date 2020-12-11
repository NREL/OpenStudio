/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/ZoneHVACLowTempRadiantConstFlow.hpp"
#include "../../model/ZoneHVACLowTempRadiantConstFlow_Impl.hpp"
#include "../../model/CoilCoolingLowTempRadiantConstFlow.hpp"
#include "../../model/CoilCoolingLowTempRadiantConstFlow_Impl.hpp"
#include "../../model/CoilHeatingLowTempRadiantConstFlow.hpp"
#include "../../model/CoilHeatingLowTempRadiantConstFlow_Impl.hpp"
#include "../../model/ConstructionWithInternalSource.hpp"
#include "../../model/ConstructionWithInternalSource_Impl.hpp"
#include "../../model/DefaultConstructionSet.hpp"
#include "../../model/DefaultConstructionSet_Impl.hpp"
#include "../../model/DefaultSurfaceConstructions.hpp"
#include "../../model/DefaultSurfaceConstructions_Impl.hpp"
#include "../../model/HVACComponent.hpp"
#include "../../model/HVACComponent_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"

#include <utilities/idd/ZoneHVAC_LowTemperatureRadiant_ConstantFlow_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ZoneHVACLowTempRadiantConstFlow_Set_Flow_Fractions) {
  //make the example model
  Model model = model::exampleModel();

  //loop through all zones and add a radiant system to each one
  for (ThermalZone thermalZone : model.getModelObjects<ThermalZone>()) {

    //make a constant flow radiant unit
    ScheduleConstant availabilitySched(model);
    ScheduleConstant coolingHighWaterTempSched(model);
    ScheduleConstant coolingLowWaterTempSched(model);
    ScheduleConstant coolingHighControlTempSched(model);
    ScheduleConstant coolingLowControlTempSched(model);
    ScheduleConstant heatingHighWaterTempSched(model);
    ScheduleConstant heatingLowWaterTempSched(model);
    ScheduleConstant heatingHighControlTempSched(model);
    ScheduleConstant heatingLowControlTempSched(model);

    availabilitySched.setValue(1.0);
    coolingHighWaterTempSched.setValue(15.0);
    coolingLowWaterTempSched.setValue(10.0);
    coolingHighControlTempSched.setValue(26.0);
    coolingLowControlTempSched.setValue(22.0);
    heatingHighWaterTempSched.setValue(50.0);
    heatingLowWaterTempSched.setValue(30.0);
    heatingHighControlTempSched.setValue(21.0);
    heatingLowControlTempSched.setValue(15.0);

    CoilCoolingLowTempRadiantConstFlow testCC(model, coolingHighWaterTempSched, coolingLowWaterTempSched, coolingHighControlTempSched,
                                              coolingLowControlTempSched);
    CoilHeatingLowTempRadiantConstFlow testHC(model, heatingHighWaterTempSched, heatingLowWaterTempSched, heatingHighControlTempSched,
                                              heatingLowControlTempSched);

    ZoneHVACLowTempRadiantConstFlow testRad(model, availabilitySched, testHC, testCC, 100.0);

    //set the coils
    testRad.setHeatingCoil(testHC);
    testRad.setCoolingCoil(testCC);

    //add it to the thermal zone
    testRad.addToThermalZone(thermalZone);

    //attach to ceilings
    testRad.setRadiantSurfaceType("Ceilings");

    //test that "surfaces" method returns 0 since no
    //ceilings have an internal source construction
    EXPECT_EQ(0, testRad.surfaces().size());
  }

  // Create some materials and make an internal source construction
  StandardOpaqueMaterial exterior(model);
  StandardOpaqueMaterial interior(model);
  OpaqueMaterialVector layers;
  layers.push_back(exterior);
  layers.push_back(interior);
  ConstructionWithInternalSource construction(layers);

  //set building's default ceiling construction to internal source construction
  DefaultConstructionSet defConSet = model.getModelObjects<DefaultConstructionSet>()[0];
  defConSet.defaultExteriorSurfaceConstructions()->setRoofCeilingConstruction(construction);

  //translate the model to EnergyPlus
  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  //loop through all zones and check the flow fraction for each surface in the surface group.  it should be 0.25
  for (ThermalZone thermalZone : model.getModelObjects<ThermalZone>()) {

    //get the radiant zone equipment
    for (ModelObject equipment : thermalZone.equipment()) {
      if (equipment.optionalCast<ZoneHVACLowTempRadiantConstFlow>()) {
        ZoneHVACLowTempRadiantConstFlow testRad = equipment.optionalCast<ZoneHVACLowTempRadiantConstFlow>().get();
        EXPECT_TRUE(testRad.isFluidtoRadiantSurfaceHeatTransferModelDefaulted());
        EXPECT_TRUE(testRad.isHydronicTubingInsideDiameterDefaulted());
        EXPECT_TRUE(testRad.isHydronicTubingOutsideDiameterDefaulted());
        EXPECT_FALSE(testRad.isHydronicTubingLengthAutosized());
        EXPECT_TRUE(testRad.isHydronicTubingConductivityDefaulted());
        EXPECT_TRUE(testRad.isTemperatureControlTypeDefaulted());
        EXPECT_TRUE(testRad.isRunningMeanOutdoorDryBulbTemperatureWeightingFactorDefaulted());
        EXPECT_TRUE(testRad.isFractionofMotorInefficienciestoFluidStreamDefaulted());
        for (IdfExtensibleGroup extGrp : testRad.extensibleGroups()) {
          EXPECT_EQ(0.25, extGrp.getDouble(1, false));
        }
      }
    }
  }
}

TEST_F(EnergyPlusFixture, ZoneHVACLowTempRadiantConstFlow_Crash_no_constructions) {

  ForwardTranslator ft;

  Model m;

  // make a space with some surfaces
  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

  boost::optional<Space> _space1 = Space::fromFloorPrint(points, 3, m);
  ASSERT_TRUE(_space1);

  // make a zone, add the space
  ThermalZone z(m);
  _space1->setThermalZone(z);

  // make some plant loops
  PlantLoop clg_loop(m);
  PlantLoop htg_loop(m);

  ASSERT_NO_THROW(ft.translateModel(m));

  // Make a radiant low temperature system
  Schedule alwaysOn = m.alwaysOnDiscreteSchedule();

  ScheduleConstant availabilitySched(m);
  ScheduleConstant coolingHighWaterTempSched(m);
  ScheduleConstant coolingLowWaterTempSched(m);
  ScheduleConstant coolingHighControlTempSched(m);
  ScheduleConstant coolingLowControlTempSched(m);
  ScheduleConstant heatingHighWaterTempSched(m);
  ScheduleConstant heatingLowWaterTempSched(m);
  ScheduleConstant heatingHighControlTempSched(m);
  ScheduleConstant heatingLowControlTempSched(m);

  availabilitySched.setValue(1.0);
  coolingHighWaterTempSched.setValue(15.0);
  coolingLowWaterTempSched.setValue(10.0);
  coolingHighControlTempSched.setValue(26.0);
  coolingLowControlTempSched.setValue(22.0);
  heatingHighWaterTempSched.setValue(50.0);
  heatingLowWaterTempSched.setValue(30.0);
  heatingHighControlTempSched.setValue(21.0);
  heatingLowControlTempSched.setValue(15.0);

  CoilCoolingLowTempRadiantConstFlow clg_coil(m, coolingHighWaterTempSched, coolingLowWaterTempSched, coolingHighControlTempSched,
                                              coolingLowControlTempSched);
  CoilHeatingLowTempRadiantConstFlow htg_coil(m, heatingHighWaterTempSched, heatingLowWaterTempSched, heatingHighControlTempSched,
                                              heatingLowControlTempSched);

  ZoneHVACLowTempRadiantConstFlow testRad(m, alwaysOn, htg_coil, clg_coil);
  EXPECT_TRUE(testRad.addToThermalZone(z));

  ASSERT_NO_THROW(ft.translateModel(m));

  // Test for #2797 - When no surfaces, it shouldn't even be translated or E+ will crash
  EXPECT_TRUE(testRad.surfaces().empty());
  Workspace w = ft.translateModel(m);
  WorkspaceObjectVector idf_rads(w.getObjectsByType(IddObjectType::ZoneHVAC_LowTemperatureRadiant_ConstantFlow));
  EXPECT_EQ(0u, idf_rads.size());
}
