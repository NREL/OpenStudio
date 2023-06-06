/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
  for (ThermalZone thermalZone : model.getConcreteModelObjects<ThermalZone>()) {

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
  DefaultConstructionSet defConSet = model.getConcreteModelObjects<DefaultConstructionSet>()[0];
  defConSet.defaultExteriorSurfaceConstructions()->setRoofCeilingConstruction(construction);

  //translate the model to EnergyPlus
  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  // TODO: this is wrong... it checks the model instead of the workspace (so it doesn't test the FT at all...)

  //loop through all zones and check the flow fraction for each surface in the surface group.  it should be 0.25
  for (const ThermalZone& thermalZone : model.getConcreteModelObjects<ThermalZone>()) {

    //get the radiant zone equipment
    for (const ModelObject& equipment : thermalZone.equipment()) {
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
        for (const IdfExtensibleGroup& extGrp : testRad.extensibleGroups()) {
          EXPECT_EQ(0.25, extGrp.getDouble(1, false).get());
        }
      }
    }
  }
}

TEST_F(EnergyPlusFixture, ZoneHVACLowTempRadiantConstFlow_Crash_no_constructions) {

  ForwardTranslator ft;

  Model m;

  // make a space with some surfaces
  Point3dVector points{
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
  };

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
