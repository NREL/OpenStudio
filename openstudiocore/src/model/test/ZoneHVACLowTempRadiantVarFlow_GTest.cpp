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

#include "ModelFixture.hpp"

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
#include "../ZoneHVACLowTempRadiantVarFlow.hpp"
#include "../ZoneHVACLowTempRadiantVarFlow_Impl.hpp"

#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,ZoneHVACLowTempRadiantVarFlow_Check_Constructor)
{
  Model model;
  ScheduleConstant availabilitySched(model);
  ScheduleConstant coolingControlTemperatureSchedule(model);
  ScheduleConstant heatingControlTemperatureSchedule(model);

  availabilitySched.setValue(1.0);
  coolingControlTemperatureSchedule.setValue(15.0);
  heatingControlTemperatureSchedule.setValue(10.0);

  CoilCoolingLowTempRadiantVarFlow testCC(model,coolingControlTemperatureSchedule);
  CoilHeatingLowTempRadiantVarFlow testHC(model,heatingControlTemperatureSchedule);

  HVACComponent testCC1 = testCC.cast<HVACComponent>();
  HVACComponent testHC1 = testHC.cast<HVACComponent>();

  ZoneHVACLowTempRadiantVarFlow testRad(model,availabilitySched,testHC1,testCC1);

  // Test set and get heating coils
  testRad.setHeatingCoil(testHC1);
  EXPECT_EQ(testRad.heatingCoil(),testHC1);

  testRad.setCoolingCoil(testCC1);
  EXPECT_EQ(testRad.coolingCoil(),testCC1);

  // Test clone
  testRad.setHydronicTubingInsideDiameter(5);

  // Clone into the same model
  ZoneHVACLowTempRadiantVarFlow cloneRad = testRad.clone(model).cast<model::ZoneHVACLowTempRadiantVarFlow>();

  HVACComponent cloneCC1 = cloneRad.coolingCoil();
  HVACComponent cloneHC1 = cloneRad.heatingCoil();

  ASSERT_EQ(testRad.hydronicTubingInsideDiameter(), cloneRad.hydronicTubingInsideDiameter());

  // Clone into another model
  Model model2;
  ZoneHVACLowTempRadiantVarFlow cloneRad2 =   cloneRad.clone(model2).cast<model::ZoneHVACLowTempRadiantVarFlow>();
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
  EXPECT_EQ(*str1,"Floors");

  testRad.resetRadiantSurfaceType();
  str1 = testRad.radiantSurfaceType();
  EXPECT_EQ(*str1,"Ceilings");

  // Test set and get Hydronic Tubing Inside Diameter
  testRad.setHydronicTubingInsideDiameter(0.01);
  double inDia = testRad.hydronicTubingInsideDiameter();
  EXPECT_EQ(inDia, 0.01);
  EXPECT_FALSE(testRad.isHydronicTubingInsideDiameterDefaulted());

  testRad.resetHydronicTubingInsideDiameter();
  EXPECT_TRUE(testRad.isHydronicTubingInsideDiameterDefaulted());
  double inDia1 = testRad.hydronicTubingInsideDiameter();
  EXPECT_EQ(inDia1,0.013);

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

  // Test set and get Temperature Control Type
  testRad.setTemperatureControlType("OutdoorDryBulbTemperature");
  boost::optional<std::string> str2 = testRad.temperatureControlType();
  EXPECT_EQ(*str2,"OutdoorDryBulbTemperature");
  EXPECT_FALSE(testRad.isTemperatureControlTypeDefaulted());

  testRad.resetTemperatureControlType();
  EXPECT_TRUE(testRad.isTemperatureControlTypeDefaulted());
  boost::optional<std::string> str3 = testRad.temperatureControlType();
  EXPECT_EQ(*str3,"MeanAirTemperature");

  //test number of circuits
  testRad.setNumberofCircuits("CalculateFromCircuitLength");
  std::string numCirc = testRad.numberofCircuits();
  EXPECT_EQ(numCirc,"CalculateFromCircuitLength");

  //test circuit length
  testRad.setCircuitLength(200.0);
  double circLength = testRad.circuitLength();
  EXPECT_EQ(circLength,200.0);

}

TEST_F(ModelFixture,ZoneHVACLowTempRadiantVarFlow_Set_Flow_Fractions)
{
  //make the example model
  Model model = model::exampleModel();

  //loop through all zones and add a radiant system to each one
  for (ThermalZone thermalZone : model.getModelObjects<ThermalZone>()){
    //make a variable flow radiant unit
    ScheduleConstant availabilitySched(model);
    ScheduleConstant coolingControlTemperatureSchedule(model);
    ScheduleConstant heatingControlTemperatureSchedule(model);

    availabilitySched.setValue(1.0);
    coolingControlTemperatureSchedule.setValue(15.0);
    heatingControlTemperatureSchedule.setValue(10.0);

    CoilCoolingLowTempRadiantVarFlow testCC(model,coolingControlTemperatureSchedule);
    CoilHeatingLowTempRadiantVarFlow testHC(model,heatingControlTemperatureSchedule);

    HVACComponent testCC1 = testCC.cast<HVACComponent>();
    HVACComponent testHC1 = testHC.cast<HVACComponent>();

    ZoneHVACLowTempRadiantVarFlow testRad(model,availabilitySched,testHC1,testCC1);

    //set the coils
    testRad.setHeatingCoil(testHC1);
    testRad.setCoolingCoil(testCC1);

    //add it to the thermal zone
    testRad.addToThermalZone(thermalZone);

    //attach to ceilings
    testRad.setRadiantSurfaceType("Ceilings");

    //test that "surfaces" method returns 0 since no
    //ceilings have an internal source construction
    EXPECT_EQ(0,testRad.surfaces().size());

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
  for (ThermalZone thermalZone : model.getModelObjects<ThermalZone>()){

    //get the radiant zone equipment
    for (const ModelObject& equipment : thermalZone.equipment()){
      if (equipment.optionalCast<ZoneHVACLowTempRadiantVarFlow>()){
        ZoneHVACLowTempRadiantVarFlow testRad = equipment.optionalCast<ZoneHVACLowTempRadiantVarFlow>().get();
        EXPECT_EQ(4,testRad.surfaces().size());
      }
    }
  }

}
  
