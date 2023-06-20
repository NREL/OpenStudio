/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/ZoneHVACLowTempRadiantVarFlow.hpp"
#include "../../model/ZoneHVACLowTempRadiantVarFlow_Impl.hpp"
#include "../../model/CoilCoolingLowTempRadiantVarFlow.hpp"
#include "../../model/CoilCoolingLowTempRadiantVarFlow_Impl.hpp"
#include "../../model/CoilHeatingLowTempRadiantVarFlow.hpp"
#include "../../model/CoilHeatingLowTempRadiantVarFlow_Impl.hpp"
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
#include "../../model/PlantLoop.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"

#include <utilities/idd/ZoneHVAC_LowTemperatureRadiant_VariableFlow_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_LowTemperatureRadiant_VariableFlow_Design_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ZoneHVACLowTempRadiantVarFlow_Set_Flow_Fractions) {
  //make the example model
  Model model = model::exampleModel();

  //loop through all zones and add a radiant system to each one
  for (ThermalZone thermalZone : model.getConcreteModelObjects<ThermalZone>()) {

    //make a variable flow radiant unit
    ScheduleConstant availabilitySched(model);
    ScheduleConstant coolingControlTemperatureSchedule(model);
    ScheduleConstant heatingControlTemperatureSchedule(model);

    availabilitySched.setValue(1.0);
    coolingControlTemperatureSchedule.setValue(15.0);
    heatingControlTemperatureSchedule.setValue(10.0);

    CoilCoolingLowTempRadiantVarFlow testCC(model, coolingControlTemperatureSchedule);
    CoilHeatingLowTempRadiantVarFlow testHC(model, heatingControlTemperatureSchedule);

    auto testCC1 = testCC.cast<HVACComponent>();
    auto testHC1 = testHC.cast<HVACComponent>();

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

  //set building's default ceiling construction to internal source construction
  DefaultConstructionSet defConSet = model.getConcreteModelObjects<DefaultConstructionSet>()[0];
  defConSet.defaultExteriorSurfaceConstructions()->setRoofCeilingConstruction(construction);

  //translate the model to EnergyPlus
  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  //loop through all zones and check the flow fraction for each surface in the surface group.  it should be 0.25
  for (const ThermalZone& thermalZone : model.getConcreteModelObjects<ThermalZone>()) {

    //get the radiant zone equipment
    for (const ModelObject& equipment : thermalZone.equipment()) {
      if (equipment.optionalCast<ZoneHVACLowTempRadiantVarFlow>()) {
        ZoneHVACLowTempRadiantVarFlow testRad = equipment.optionalCast<ZoneHVACLowTempRadiantVarFlow>().get();
        EXPECT_TRUE(testRad.isFluidtoRadiantSurfaceHeatTransferModelDefaulted());
        EXPECT_TRUE(testRad.isHydronicTubingInsideDiameterDefaulted());
        EXPECT_TRUE(testRad.isHydronicTubingOutsideDiameterDefaulted());
        EXPECT_TRUE(testRad.isHydronicTubingLengthDefaulted());
        EXPECT_TRUE(testRad.isHydronicTubingLengthAutosized());
        EXPECT_TRUE(testRad.isHydronicTubingConductivityDefaulted());
        EXPECT_TRUE(testRad.isTemperatureControlTypeDefaulted());
        EXPECT_TRUE(testRad.isSetpointControlTypeDefaulted());
        for (const IdfExtensibleGroup& extGrp : testRad.extensibleGroups()) {
          EXPECT_EQ(0.25, extGrp.getDouble(1, false).get());
        }
      }
    }
  }
}

// Test for #3866
TEST_F(EnergyPlusFixture, ZoneHVACLowTempRadiantVarFlow_Crash_no_constructions) {

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

  ScheduleConstant htg_sch(m);
  CoilHeatingLowTempRadiantVarFlow htg_coil(m, htg_sch);
  EXPECT_TRUE(htg_loop.addDemandBranchForComponent(htg_coil));

  ScheduleConstant clg_sch(m);
  CoilCoolingLowTempRadiantVarFlow clg_coil(m, clg_sch);
  EXPECT_TRUE(clg_loop.addDemandBranchForComponent(clg_coil));

  ZoneHVACLowTempRadiantVarFlow testRad(m, alwaysOn, htg_coil, clg_coil);
  EXPECT_TRUE(testRad.addToThermalZone(z));

  ASSERT_NO_THROW(ft.translateModel(m));

  // Test for #2797 - When no surfaces, it shouldn't even be translated or E+ will crash
  EXPECT_TRUE(testRad.surfaces().empty());
  Workspace w = ft.translateModel(m);
  WorkspaceObjectVector idf_rads(w.getObjectsByType(IddObjectType::ZoneHVAC_LowTemperatureRadiant_VariableFlow));
  EXPECT_EQ(0u, idf_rads.size());
}

TEST_F(EnergyPlusFixture, ZoneHVACLowTempRadiantVarFlow_NoCoils) {

  // Test for #3532 - Making the heating and cooling coils optional, we expect the FT to not translate it if it has neither coils
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

  ZoneHVACLowTempRadiantVarFlow testRad(m);

  // Create some materials and make an internal source construction
  StandardOpaqueMaterial exterior(m);
  StandardOpaqueMaterial interior(m);
  OpaqueMaterialVector layers;
  layers.push_back(exterior);
  layers.push_back(interior);
  ConstructionWithInternalSource construction(layers);

  // set building's default ceiling construction to internal source construction
  DefaultSurfaceConstructions defaultSurfaceConstructions(m);
  // EXPECT_TRUE(defaultSurfaceConstructions.setFloorConstruction(construction));
  // EXPECT_TRUE(defaultSurfaceConstructions.setWallConstruction(construction));
  EXPECT_TRUE(defaultSurfaceConstructions.setRoofCeilingConstruction(construction));

  DefaultConstructionSet defaultConstructionSet1(m);
  defaultConstructionSet1.setDefaultExteriorSurfaceConstructions(defaultSurfaceConstructions);

  Building b = m.getUniqueModelObject<Building>();
  b.setDefaultConstructionSet(defaultConstructionSet1);

  // add it to the thermal zone
  testRad.addToThermalZone(z);

  // attach to ceilings
  testRad.setRadiantSurfaceType("Ceilings");

  // When no coils, it shouldn't even be translated
  EXPECT_FALSE(testRad.heatingCoil());
  EXPECT_FALSE(testRad.coolingCoil());
  EXPECT_TRUE(!testRad.surfaces().empty());
  {
    Workspace w = ft.translateModel(m);
    WorkspaceObjectVector idf_rads(w.getObjectsByType(IddObjectType::ZoneHVAC_LowTemperatureRadiant_VariableFlow));
    EXPECT_EQ(0u, idf_rads.size());
  }

  // Assign a heating coil only
  ScheduleConstant htg_sch(m);
  CoilHeatingLowTempRadiantVarFlow htg_coil(m, htg_sch);
  EXPECT_TRUE(htg_loop.addDemandBranchForComponent(htg_coil));
  EXPECT_TRUE(testRad.setHeatingCoil(htg_coil));

  EXPECT_TRUE(testRad.heatingCoil());
  EXPECT_FALSE(testRad.coolingCoil());
  EXPECT_TRUE(!testRad.surfaces().empty());

  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idf_rads(w.getObjectsByType(IddObjectType::ZoneHVAC_LowTemperatureRadiant_VariableFlow));
    EXPECT_EQ(1u, idf_rads.size());
    WorkspaceObject idf_rad(idf_rads[0]);

    ASSERT_EQ(1u, w.getObjectsByType(IddObjectType::ZoneHVAC_LowTemperatureRadiant_VariableFlow_Design).size());

    ASSERT_TRUE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingDesignCapacity, false, true));
    EXPECT_EQ("Autosize", idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingDesignCapacity, false, true).get());
    EXPECT_TRUE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::MaximumHotWaterFlow, false, true));
    EXPECT_TRUE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingWaterInletNodeName, false, true));
    EXPECT_TRUE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingWaterOutletNodeName, false, true));
    EXPECT_FALSE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingDesignCapacity, false, true));
    EXPECT_FALSE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::MaximumColdWaterFlow, false, true));
    EXPECT_FALSE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingWaterInletNodeName, false, true));
    EXPECT_FALSE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingWaterOutletNodeName, false, true));

    // We check that it does have a design object assigned
    ASSERT_TRUE(idf_rad.getTarget(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::DesignObject));
    WorkspaceObject idfDesign = idf_rad.getTarget(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::DesignObject).get();

    ASSERT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::HeatingDesignCapacityMethod, false, true));
    EXPECT_EQ("HeatingDesignCapacity",
              idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::HeatingDesignCapacityMethod, false, true).get());
    EXPECT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::HeatingDesignCapacityPerFloorArea, false, true));
    EXPECT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::FractionofAutosizedHeatingDesignCapacity, false, true));
    EXPECT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::HeatingControlThrottlingRange, false, true));
    EXPECT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::HeatingControlTemperatureScheduleName, false, true));
    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CoolingDesignCapacityMethod, false, true));
    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CoolingDesignCapacityPerFloorArea, false, true));
    EXPECT_FALSE(
      idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::FractionofAutosizedCoolingDesignCapacity, false, true));
    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CoolingControlThrottlingRange, false, true));
    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CoolingControlTemperatureScheduleName, false, true));
    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CondensationControlType, false, true));
    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CondensationControlDewpointOffset, false, true));
  }

  EXPECT_TRUE(htg_coil.setHeatingDesignCapacityMethod("CapacityPerFloorArea"));
  EXPECT_EQ("CapacityPerFloorArea", htg_coil.heatingDesignCapacityMethod());

  EXPECT_TRUE(htg_coil.setHeatingDesignCapacity(1000.05));
  EXPECT_FALSE(htg_coil.isHeatingDesignCapacityAutosized());

  EXPECT_TRUE(htg_coil.setHeatingDesignCapacityPerFloorArea(11.05));
  EXPECT_EQ(11.05, htg_coil.heatingDesignCapacityPerFloorArea());

  EXPECT_TRUE(htg_coil.setFractionofAutosizedHeatingDesignCapacity(1.05));
  EXPECT_EQ(1.05, htg_coil.fractionofAutosizedHeatingDesignCapacity());

  {
    Workspace w = ft.translateModel(m);
    WorkspaceObjectVector idf_rads(w.getObjectsByType(IddObjectType::ZoneHVAC_LowTemperatureRadiant_VariableFlow));
    EXPECT_EQ(1u, idf_rads.size());
    WorkspaceObject idf_rad(idf_rads[0]);
    ASSERT_EQ(1u, w.getObjectsByType(IddObjectType::ZoneHVAC_LowTemperatureRadiant_VariableFlow_Design).size());

    ASSERT_TRUE(idf_rad.getDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingDesignCapacity, false));
    EXPECT_EQ(1000.05, idf_rad.getDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingDesignCapacity, false).get());
    EXPECT_TRUE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::MaximumHotWaterFlow, false, true));
    EXPECT_TRUE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingWaterInletNodeName, false, true));
    EXPECT_TRUE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingWaterOutletNodeName, false, true));

    EXPECT_FALSE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingDesignCapacity, false, true));
    EXPECT_FALSE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::MaximumColdWaterFlow, false, true));
    EXPECT_FALSE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingWaterInletNodeName, false, true));
    EXPECT_FALSE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingWaterOutletNodeName, false, true));

    // We check that it does have a design object assigned
    ASSERT_TRUE(idf_rad.getTarget(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::DesignObject));
    WorkspaceObject idfDesign = idf_rad.getTarget(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::DesignObject).get();

    ASSERT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::HeatingDesignCapacityMethod, false, true));
    EXPECT_EQ("CapacityPerFloorArea",
              idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::HeatingDesignCapacityMethod, false, true).get());
    ASSERT_TRUE(idfDesign.getDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::HeatingDesignCapacityPerFloorArea, false));
    EXPECT_EQ(11.05, idfDesign.getDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::HeatingDesignCapacityPerFloorArea, false).get());
    ASSERT_TRUE(idfDesign.getDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::FractionofAutosizedHeatingDesignCapacity, false));
    EXPECT_EQ(1.05,
              idfDesign.getDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::FractionofAutosizedHeatingDesignCapacity, false).get());
    EXPECT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::HeatingControlThrottlingRange, false, true));
    EXPECT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::HeatingControlTemperatureScheduleName, false, true));

    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CoolingDesignCapacityMethod, false, true));
    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CoolingDesignCapacityPerFloorArea, false, true));
    EXPECT_FALSE(
      idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::FractionofAutosizedCoolingDesignCapacity, false, true));
    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CoolingControlTemperatureScheduleName, false, true));
    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CoolingControlThrottlingRange, false, true));
    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CondensationControlType, false, true));
    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CondensationControlDewpointOffset, false, true));
  }

  // Assign a cooling coil only
  testRad.resetHeatingCoil();

  ScheduleConstant clg_sch(m);
  CoilCoolingLowTempRadiantVarFlow clg_coil(m, clg_sch);
  EXPECT_TRUE(clg_loop.addDemandBranchForComponent(clg_coil));
  EXPECT_TRUE(testRad.setCoolingCoil(clg_coil));

  EXPECT_FALSE(testRad.heatingCoil());
  EXPECT_TRUE(testRad.coolingCoil());
  EXPECT_TRUE(!testRad.surfaces().empty());
  {
    Workspace w = ft.translateModel(m);
    WorkspaceObjectVector idf_rads(w.getObjectsByType(IddObjectType::ZoneHVAC_LowTemperatureRadiant_VariableFlow));
    EXPECT_EQ(1u, idf_rads.size());
    WorkspaceObject idf_rad(idf_rads[0]);
    ASSERT_EQ(1u, w.getObjectsByType(IddObjectType::ZoneHVAC_LowTemperatureRadiant_VariableFlow_Design).size());

    EXPECT_FALSE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingDesignCapacity, false, true));
    EXPECT_FALSE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::MaximumHotWaterFlow, false, true));
    EXPECT_FALSE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingWaterInletNodeName, false, true));
    EXPECT_FALSE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingWaterOutletNodeName, false, true));

    ASSERT_TRUE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingDesignCapacity, false, true));
    EXPECT_EQ("Autosize", idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingDesignCapacity, false, true).get());
    EXPECT_TRUE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::MaximumColdWaterFlow, false, true));
    EXPECT_TRUE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingWaterInletNodeName, false, true));
    EXPECT_TRUE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingWaterOutletNodeName, false, true));

    // We check that it does have a design object assigned
    ASSERT_TRUE(idf_rad.getTarget(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::DesignObject));
    WorkspaceObject idfDesign = idf_rad.getTarget(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::DesignObject).get();

    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::HeatingDesignCapacityMethod, false, true));
    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::HeatingDesignCapacityPerFloorArea, false, true));
    EXPECT_FALSE(
      idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::FractionofAutosizedHeatingDesignCapacity, false, true));
    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::HeatingControlThrottlingRange, false, true));
    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::HeatingControlTemperatureScheduleName, false, true));

    ASSERT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CoolingDesignCapacityMethod, false, true));
    EXPECT_EQ("CoolingDesignCapacity",
              idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CoolingDesignCapacityMethod, false, true).get());
    EXPECT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CoolingDesignCapacityPerFloorArea, false, true));
    EXPECT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::FractionofAutosizedCoolingDesignCapacity, false, true));
    EXPECT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CoolingControlThrottlingRange, false, true));
    EXPECT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CoolingControlTemperatureScheduleName, false, true));
    EXPECT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CondensationControlType, false, true));
    EXPECT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CondensationControlDewpointOffset, false, true));
  }

  EXPECT_TRUE(clg_coil.setCoolingDesignCapacityMethod("FractionOfAutosizedCoolingCapacity"));
  EXPECT_EQ("FractionOfAutosizedCoolingCapacity", clg_coil.coolingDesignCapacityMethod());

  EXPECT_TRUE(clg_coil.setCoolingDesignCapacity(1002.05));
  EXPECT_FALSE(clg_coil.isCoolingDesignCapacityAutosized());

  EXPECT_TRUE(clg_coil.setCoolingDesignCapacityPerFloorArea(12.05));
  EXPECT_EQ(12.05, clg_coil.coolingDesignCapacityPerFloorArea());

  EXPECT_TRUE(clg_coil.setFractionofAutosizedCoolingDesignCapacity(2.05));
  EXPECT_EQ(2.05, clg_coil.fractionofAutosizedCoolingDesignCapacity());
  {
    Workspace w = ft.translateModel(m);
    WorkspaceObjectVector idf_rads(w.getObjectsByType(IddObjectType::ZoneHVAC_LowTemperatureRadiant_VariableFlow));
    EXPECT_EQ(1u, idf_rads.size());
    WorkspaceObject idf_rad(idf_rads[0]);

    ASSERT_EQ(1u, w.getObjectsByType(IddObjectType::ZoneHVAC_LowTemperatureRadiant_VariableFlow_Design).size());

    EXPECT_FALSE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingDesignCapacity, false, true));
    EXPECT_FALSE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::MaximumHotWaterFlow, false, true));
    EXPECT_FALSE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingWaterInletNodeName, false, true));
    EXPECT_FALSE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HeatingWaterOutletNodeName, false, true));

    ASSERT_TRUE(idf_rad.getDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingDesignCapacity, false));
    EXPECT_EQ(1002.05, idf_rad.getDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingDesignCapacity, false).get());
    EXPECT_TRUE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::MaximumColdWaterFlow, false, true));
    EXPECT_TRUE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingWaterInletNodeName, false, true));
    EXPECT_TRUE(idf_rad.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CoolingWaterOutletNodeName, false, true));

    // We check that it does have a design object assigned
    ASSERT_TRUE(idf_rad.getTarget(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::DesignObject));
    WorkspaceObject idfDesign = idf_rad.getTarget(ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::DesignObject).get();

    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::HeatingDesignCapacityMethod, false, true));
    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::HeatingDesignCapacityPerFloorArea, false, true));
    EXPECT_FALSE(
      idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::FractionofAutosizedHeatingDesignCapacity, false, true));
    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::HeatingControlThrottlingRange, false, true));
    EXPECT_FALSE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::HeatingControlTemperatureScheduleName, false, true));

    ASSERT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CoolingDesignCapacityMethod, false, true));
    EXPECT_EQ("FractionOfAutosizedCoolingCapacity",
              idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CoolingDesignCapacityMethod, false, true).get());
    ASSERT_TRUE(idfDesign.getDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CoolingDesignCapacityPerFloorArea, false));
    EXPECT_EQ(12.05, idfDesign.getDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CoolingDesignCapacityPerFloorArea, false).get());
    ASSERT_TRUE(idfDesign.getDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::FractionofAutosizedCoolingDesignCapacity, false));
    EXPECT_EQ(2.05,
              idfDesign.getDouble(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::FractionofAutosizedCoolingDesignCapacity, false).get());
    EXPECT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CoolingControlThrottlingRange, false, true));
    EXPECT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CoolingControlTemperatureScheduleName, false, true));
    EXPECT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CondensationControlType, false, true));
    EXPECT_TRUE(idfDesign.getString(ZoneHVAC_LowTemperatureRadiant_VariableFlow_DesignFields::CondensationControlDewpointOffset, false, true));
  }
}
