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

#include "ModelFixture.hpp"

#include "../ZoneHVACLowTempRadiantVarFlow.hpp"
#include "../ZoneHVACLowTempRadiantVarFlow_Impl.hpp"
#include "../CoilCoolingLowTempRadiantVarFlow.hpp"
#include "../CoilCoolingLowTempRadiantVarFlow_Impl.hpp"
#include "../CoilHeatingLowTempRadiantVarFlow.hpp"
#include "../CoilHeatingLowTempRadiantVarFlow_Impl.hpp"
#include "../ConstructionWithInternalSource.hpp"
#include "../ConstructionWithInternalSource_Impl.hpp"
#include "../DefaultConstructionSet.hpp"
#include "../DefaultConstructionSet_Impl.hpp"
#include "../DefaultSurfaceConstructions.hpp"
#include "../DefaultSurfaceConstructions_Impl.hpp"
#include "../HVACComponent.hpp"
#include "../HVACComponent_Impl.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Space.hpp"
#include "../Construction.hpp"
#include "../ConstructionWithInternalSource.hpp"

#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneHVACLowTempRadiantVarFlow_Check_Constructor) {
  Model model;
  ScheduleConstant availabilitySched(model);
  ScheduleConstant coolingControlTemperatureSchedule(model);
  ScheduleConstant heatingControlTemperatureSchedule(model);

  availabilitySched.setValue(1.0);
  coolingControlTemperatureSchedule.setValue(15.0);
  heatingControlTemperatureSchedule.setValue(10.0);

  CoilCoolingLowTempRadiantVarFlow testCC(model, coolingControlTemperatureSchedule);
  CoilHeatingLowTempRadiantVarFlow testHC(model, heatingControlTemperatureSchedule);

  HVACComponent testCC1 = testCC.cast<HVACComponent>();
  HVACComponent testHC1 = testHC.cast<HVACComponent>();

  ZoneHVACLowTempRadiantVarFlow testRad(model, availabilitySched, testHC1, testCC1);

  // Test set and get heating coils
  testRad.setHeatingCoil(testHC1);
  ASSERT_TRUE(testRad.heatingCoil());
  EXPECT_EQ(testRad.heatingCoil().get(), testHC1);

  testRad.setCoolingCoil(testCC1);
  ASSERT_TRUE(testRad.coolingCoil());
  EXPECT_EQ(testRad.coolingCoil().get(), testCC1);

  // Test clone
  testRad.setHydronicTubingInsideDiameter(5);

  // Clone into the same model
  ZoneHVACLowTempRadiantVarFlow cloneRad = testRad.clone(model).cast<model::ZoneHVACLowTempRadiantVarFlow>();

  boost::optional<HVACComponent> cloneCC1 = cloneRad.coolingCoil();
  boost::optional<HVACComponent> cloneHC1 = cloneRad.heatingCoil();
  EXPECT_TRUE(cloneHC1);
  EXPECT_TRUE(cloneCC1);
  ASSERT_EQ(testRad.hydronicTubingInsideDiameter(), cloneRad.hydronicTubingInsideDiameter());

  // Clone into another model
  Model model2;
  ZoneHVACLowTempRadiantVarFlow cloneRad2 = cloneRad.clone(model2).cast<model::ZoneHVACLowTempRadiantVarFlow>();
  ASSERT_EQ(cloneRad.hydronicTubingInsideDiameter(), cloneRad2.hydronicTubingInsideDiameter());

  //test add to and remove from Thermal zone
  ThermalZone thermalZone(model);
  EXPECT_TRUE(testRad.addToThermalZone(thermalZone));
  boost::optional<ThermalZone> testThermalZone = testRad.thermalZone();
  //EXPECT_EQ(*(testThermalZone),testRad.thermalZone());

  testRad.removeFromThermalZone();
  EXPECT_FALSE(testRad.thermalZone());

  // Test set and get radiant surface type

  testRad.setRadiantSurfaceType("Floors");
  boost::optional<std::string> str1 = testRad.radiantSurfaceType();
  EXPECT_EQ(*str1, "Floors");

  testRad.resetRadiantSurfaceType();
  str1 = testRad.radiantSurfaceType();
  EXPECT_EQ(*str1, "Ceilings");

  // Test set and get Fluid to Radiant Surface Heat Transfer Model
  testRad.setFluidtoRadiantSurfaceHeatTransferModel("ISOStandard");
  boost::optional<std::string> str2 = testRad.fluidtoRadiantSurfaceHeatTransferModel();
  EXPECT_EQ(*str2, "ISOStandard");
  EXPECT_FALSE(testRad.isFluidtoRadiantSurfaceHeatTransferModelDefaulted());
  testRad.resetFluidtoRadiantSurfaceHeatTransferModel();
  EXPECT_TRUE(testRad.isFluidtoRadiantSurfaceHeatTransferModelDefaulted());
  boost::optional<std::string> str3 = testRad.fluidtoRadiantSurfaceHeatTransferModel();
  EXPECT_EQ(*str3, "ConvectionOnly");

  // Test set and get Hydronic Tubing Inside Diameter
  testRad.setHydronicTubingInsideDiameter(0.01);
  double inDia = testRad.hydronicTubingInsideDiameter();
  EXPECT_EQ(inDia, 0.01);
  EXPECT_FALSE(testRad.isHydronicTubingInsideDiameterDefaulted());
  testRad.resetHydronicTubingInsideDiameter();
  EXPECT_TRUE(testRad.isHydronicTubingInsideDiameterDefaulted());
  double inDia1 = testRad.hydronicTubingInsideDiameter();
  EXPECT_EQ(inDia1, 0.013);

  // Test set and get Hydronic Tubing Outside Diameter
  testRad.setHydronicTubingOutsideDiameter(0.01);
  double outDia = testRad.hydronicTubingOutsideDiameter();
  EXPECT_EQ(outDia, 0.01);
  EXPECT_FALSE(testRad.isHydronicTubingOutsideDiameterDefaulted());
  testRad.resetHydronicTubingOutsideDiameter();
  EXPECT_TRUE(testRad.isHydronicTubingOutsideDiameterDefaulted());
  double outDia1 = testRad.hydronicTubingOutsideDiameter();
  EXPECT_EQ(outDia1, 0.016);

  // Test set and get Hydronic Tubing Length
  testRad.setHydronicTubingLength(200);
  boost::optional<double> length = testRad.hydronicTubingLength();
  EXPECT_EQ(*length, 200);
  EXPECT_FALSE(testRad.isHydronicTubingLengthDefaulted());
  EXPECT_FALSE(testRad.isHydronicTubingLengthAutosized());
  testRad.resetHydronicTubingLength();
  EXPECT_TRUE(testRad.isHydronicTubingLengthDefaulted());
  testRad.autosizeHydronicTubingLength();
  EXPECT_TRUE(testRad.isHydronicTubingLengthAutosized());

  // Test set and get Hydronic Tubing Conductivity
  testRad.setHydronicTubingConductivity(0.01);
  double cond = testRad.hydronicTubingConductivity();
  EXPECT_EQ(cond, 0.01);
  EXPECT_FALSE(testRad.isHydronicTubingConductivityDefaulted());
  testRad.resetHydronicTubingConductivity();
  EXPECT_TRUE(testRad.isHydronicTubingConductivityDefaulted());
  double cond1 = testRad.hydronicTubingConductivity();
  EXPECT_EQ(cond1, 0.35);

  // Test set and get Temperature Control Type
  testRad.setTemperatureControlType("OutdoorDryBulbTemperature");
  str2 = testRad.temperatureControlType();
  EXPECT_EQ(*str2, "OutdoorDryBulbTemperature");
  EXPECT_FALSE(testRad.isTemperatureControlTypeDefaulted());
  testRad.resetTemperatureControlType();
  EXPECT_TRUE(testRad.isTemperatureControlTypeDefaulted());
  str3 = testRad.temperatureControlType();
  EXPECT_EQ(*str3, "MeanAirTemperature");

  // Test set and get Setpoint Control Type
  EXPECT_TRUE(testRad.setSetpointControlType("ZeroFlowPower"));
  std::string testSetpointControlType = testRad.setpointControlType();
  EXPECT_EQ(testSetpointControlType, "ZeroFlowPower");
  EXPECT_FALSE(testRad.isSetpointControlTypeDefaulted());
  testRad.resetSetpointControlType();
  testSetpointControlType = testRad.setpointControlType();
  EXPECT_EQ(testSetpointControlType, "HalfFlowPower");
  EXPECT_TRUE(testRad.isSetpointControlTypeDefaulted());

  //test number of circuits
  testRad.setNumberofCircuits("CalculateFromCircuitLength");
  std::string numCirc = testRad.numberofCircuits();
  EXPECT_EQ(numCirc, "CalculateFromCircuitLength");

  //test circuit length
  testRad.setCircuitLength(200.0);
  double circLength = testRad.circuitLength();
  EXPECT_EQ(circLength, 200.0);

  // Test set and get Changeover Delay Time Period Schedule
  ScheduleConstant sch(model);
  EXPECT_TRUE(testRad.setChangeoverDelayTimePeriodSchedule(sch));
  ASSERT_TRUE(testRad.changeoverDelayTimePeriodSchedule());
  EXPECT_EQ(sch, testRad.changeoverDelayTimePeriodSchedule().get());
  testRad.resetChangeoverDelayTimePeriodSchedule();
  EXPECT_FALSE(testRad.changeoverDelayTimePeriodSchedule());
}

TEST_F(ModelFixture, ZoneHVACLowTempRadiantVarFlow_Set_Flow_Fractions) {
  //make the example model
  Model model = model::exampleModel();

  //loop through all zones and add a radiant system to each one
  for (ThermalZone thermalZone : model.getModelObjects<ThermalZone>()) {
    //make a variable flow radiant unit
    ScheduleConstant availabilitySched(model);
    ScheduleConstant coolingControlTemperatureSchedule(model);
    ScheduleConstant heatingControlTemperatureSchedule(model);

    availabilitySched.setValue(1.0);
    coolingControlTemperatureSchedule.setValue(15.0);
    heatingControlTemperatureSchedule.setValue(10.0);

    CoilCoolingLowTempRadiantVarFlow testCC(model, coolingControlTemperatureSchedule);
    CoilHeatingLowTempRadiantVarFlow testHC(model, heatingControlTemperatureSchedule);

    HVACComponent testCC1 = testCC.cast<HVACComponent>();
    HVACComponent testHC1 = testHC.cast<HVACComponent>();

    ZoneHVACLowTempRadiantVarFlow testRad(model, availabilitySched, testHC1, testCC1);

    //set the coils
    testRad.setHeatingCoil(testHC1);
    testRad.setCoolingCoil(testCC1);

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
  //construction.setLayers(layers);

  //set building's default ceiling construction to internal source construction
  DefaultConstructionSet defConSet = model.getModelObjects<DefaultConstructionSet>()[0];
  defConSet.defaultExteriorSurfaceConstructions()->setRoofCeilingConstruction(construction);

  //loop through all zones and check the flow fraction for each surface in the surface group.  it should be 0.25
  for (ThermalZone thermalZone : model.getModelObjects<ThermalZone>()) {

    //get the radiant zone equipment
    for (const ModelObject& equipment : thermalZone.equipment()) {
      if (equipment.optionalCast<ZoneHVACLowTempRadiantVarFlow>()) {
        ZoneHVACLowTempRadiantVarFlow testRad = equipment.optionalCast<ZoneHVACLowTempRadiantVarFlow>().get();
        EXPECT_EQ(4, testRad.surfaces().size());
      }
    }
  }
}

TEST_F(ModelFixture, ZoneHVACLowTempRadiantVarFlow_surfaces) {

  Model m;

  // make a space with some surfaces
  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

  boost::optional<Space> _space1 = Space::fromFloorPrint(points, 3, m);
  ASSERT_TRUE(_space1);
  EXPECT_EQ(6u, _space1->surfaces().size());

  // make a zone, add the space
  ThermalZone z(m);
  _space1->setThermalZone(z);

  // Make a radiant low temperature system
  ScheduleConstant availabilitySched(m);
  ScheduleConstant coolingControlTemperatureSchedule(m);
  ScheduleConstant heatingControlTemperatureSchedule(m);

  availabilitySched.setValue(1.0);
  coolingControlTemperatureSchedule.setValue(15.0);
  heatingControlTemperatureSchedule.setValue(10.0);

  CoilCoolingLowTempRadiantVarFlow clg_coil(m, coolingControlTemperatureSchedule);
  CoilHeatingLowTempRadiantVarFlow htg_coil(m, heatingControlTemperatureSchedule);

  ZoneHVACLowTempRadiantVarFlow testRad(m, availabilitySched, htg_coil, clg_coil);
  EXPECT_TRUE(testRad.setRadiantSurfaceType("AllSurfaces"));
  EXPECT_TRUE(testRad.addToThermalZone(z));

  // The surfaces don't have any constructions assigned
  ASSERT_NO_THROW(testRad.surfaces());
  EXPECT_EQ(0u, testRad.surfaces().size());

  Construction c(m);
  for (auto& s : _space1->surfaces()) {
    s.setConstruction(c);
  }

  // The surfaces have constructions, but not internal source
  ASSERT_NO_THROW(testRad.surfaces());
  EXPECT_EQ(0u, testRad.surfaces().size());

  ConstructionWithInternalSource cInternalSource(m);
  for (auto& s : _space1->surfaces()) {
    s.setConstruction(cInternalSource);
  }

  // The surfaces have constructions, with  internal source
  ASSERT_NO_THROW(testRad.surfaces());
  EXPECT_EQ(6u, testRad.surfaces().size());
}

TEST_F(ModelFixture, ZoneHVACLowTempRadiantVarFlow_MinimalCtor) {

  Model m;

  auto alwaysOn = m.alwaysOnDiscreteSchedule();

  ZoneHVACLowTempRadiantVarFlow testRad(m);
  EXPECT_EQ(alwaysOn, testRad.availabilitySchedule());

  EXPECT_FALSE(testRad.heatingCoil());
  EXPECT_FALSE(testRad.coolingCoil());
}
