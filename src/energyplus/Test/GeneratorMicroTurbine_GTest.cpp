/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

// Objects of interest
#include "../../model/GeneratorMicroTurbine.hpp"
#include "../../model/GeneratorMicroTurbine_Impl.hpp"
#include "../../model/GeneratorMicroTurbineHeatRecovery.hpp"
#include "../../model/GeneratorMicroTurbineHeatRecovery_Impl.hpp"

// Needed resources
#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveBiquadratic_Impl.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"
#include "../../model/StraightComponent.hpp"
#include "../../model/StraightComponent_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
// For testing PlantEquipOperation
#include "../../model/WaterHeaterMixed.hpp"
#include "../../model/WaterHeaterMixed_Impl.hpp"
#include "../../model/PlantEquipmentOperationHeatingLoad.hpp"
#include "../../model/PlantEquipmentOperationHeatingLoad_Impl.hpp"
#include "../../model/ElectricLoadCenterDistribution.hpp"
#include "../../model/ElectricLoadCenterDistribution_Impl.hpp"

// IDF FieldEnums
#include <utilities/idd/Generator_MicroTurbine_FieldEnums.hxx>
// #include <utilities/idd/OS_Generator_MicroTurbine_HeatRecovery_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentList_FieldEnums.hxx>
#include <utilities/idd/ElectricLoadCenter_Distribution_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

// Misc
#include "../../model/Version.hpp"
#include "../../model/Version_Impl.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/UUID.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/IdfObject.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <boost/algorithm/string/predicate.hpp>

#include <resources.hxx>

#include <sstream>

#include <vector>

// Debug
#include "../../utilities/core/Logger.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

/**
 * Tests whether the ForwarTranslator will handle the name of the GeneratorMicroTurbine correctly in the PlantEquipmentOperationHeatingLoad
 **/
TEST_F(EnergyPlusFixture, ForwardTranslatorGeneratorMicroTurbine_ELCD_PlantLoop) {

  // TODO: Temporarily output the Log in the console with the Trace (-3) level
  // for debug
  // openstudio::Logger::instance().standardOutLogger().enable();
  // openstudio::Logger::instance().standardOutLogger().setLogLevel(Trace);

  // Create a model, a mchp, a mchpHR, a plantLoop and an electricalLoadCenter
  Model model;

  GeneratorMicroTurbine mchp = GeneratorMicroTurbine(model);
  GeneratorMicroTurbineHeatRecovery mchpHR = GeneratorMicroTurbineHeatRecovery(model, mchp);
  ASSERT_EQ(mchpHR, mchp.generatorMicroTurbineHeatRecovery().get());

  PlantLoop plantLoop(model);
  // Add a supply branch for the mchpHR
  ASSERT_TRUE(plantLoop.addSupplyBranchForComponent(mchpHR));

  // Create a WaterHeater:Mixed
  WaterHeaterMixed waterHeater(model);
  // Add it on the same branch as the chpHR, right after it
  Node mchpHROutletNode = mchpHR.outletModelObject()->cast<Node>();
  ASSERT_TRUE(waterHeater.addToNode(mchpHROutletNode));

  // Create a plantEquipmentOperationHeatingLoad
  PlantEquipmentOperationHeatingLoad operation(model);
  operation.setName(plantLoop.name().get() + " PlantEquipmentOperationHeatingLoad");
  ASSERT_TRUE(plantLoop.setPlantEquipmentOperationHeatingLoad(operation));
  ASSERT_TRUE(operation.addEquipment(mchpHR));
  ASSERT_TRUE(operation.addEquipment(waterHeater));

  // Create an ELCD
  ElectricLoadCenterDistribution elcd = ElectricLoadCenterDistribution(model);
  elcd.setName("Capstone C65 ELCD");
  elcd.setElectricalBussType("AlternatingCurrent");
  elcd.addGenerator(mchp);

  // Forward Translates
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, forwardTranslator.errors().size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::WaterHeater_Mixed).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Distribution).size());
  // The MicroTurbine should have been forward translated since there is an ELCD

  WorkspaceObjectVector microTurbineObjects(workspace.getObjectsByType(IddObjectType::Generator_MicroTurbine));
  EXPECT_EQ(1u, microTurbineObjects.size());
  // Check that the HR nodes have been set
  WorkspaceObject idf_mchp(microTurbineObjects[0]);
  EXPECT_EQ(mchpHR.inletModelObject()->name().get(), idf_mchp.getString(Generator_MicroTurbineFields::HeatRecoveryWaterInletNodeName).get());
  EXPECT_EQ(mchpHR.outletModelObject()->name().get(), idf_mchp.getString(Generator_MicroTurbineFields::HeatRecoveryWaterOutletNodeName).get());

  OptionalWorkspaceObject idf_operation(workspace.getObjectByTypeAndName(IddObjectType::PlantEquipmentOperation_HeatingLoad, *(operation.name())));
  ASSERT_TRUE(idf_operation);
  // Get the extensible
  ASSERT_EQ(1u, idf_operation->numExtensibleGroups());
  // IdfExtensibleGroup eg = idf_operation.getExtensibleGroup(0);
  // idf_operation.targets[0]
  ASSERT_EQ(1u, idf_operation->targets().size());
  WorkspaceObject plantEquipmentList(idf_operation->targets()[0]);
  ASSERT_EQ(2u, plantEquipmentList.extensibleGroups().size());

  IdfExtensibleGroup eg(plantEquipmentList.extensibleGroups()[0]);
  ASSERT_EQ("Generator:MicroTurbine", eg.getString(PlantEquipmentListExtensibleFields::EquipmentObjectType).get());
  // This fails
  EXPECT_EQ(mchp.name().get(), eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

  IdfExtensibleGroup eg2(plantEquipmentList.extensibleGroups()[1]);
  ASSERT_EQ("WaterHeater:Mixed", eg2.getString(PlantEquipmentListExtensibleFields::EquipmentObjectType).get());
  EXPECT_EQ(waterHeater.name().get(), eg2.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());

  // model.save(toPath("./ForwardTranslatorGeneratorMicroTurbine_ELCD_PlantLoop.osm"), true);
  // workspace.save(toPath("./ForwardTranslatorGeneratorMicroTurbine_ELCD_PlantLoop.idf"), true);
}
//test orphaning the generator before FT
TEST_F(EnergyPlusFixture, ForwardTranslatorGeneratorMicroTurbine_ELCD_Orphan) {
  // Create a model, a mchp, a mchpHR, a plantLoop and an electricalLoadCenter
  Model model;

  GeneratorMicroTurbine mchp = GeneratorMicroTurbine(model);
  GeneratorMicroTurbineHeatRecovery mchpHR = GeneratorMicroTurbineHeatRecovery(model, mchp);
  ASSERT_EQ(mchpHR, mchp.generatorMicroTurbineHeatRecovery().get());

  PlantLoop plantLoop(model);
  // Add a supply branch for the mchpHR
  ASSERT_TRUE(plantLoop.addSupplyBranchForComponent(mchpHR));

  // Create a WaterHeater:Mixed
  WaterHeaterMixed waterHeater(model);
  // Add it on the same branch as the chpHR, right after it
  Node mchpHROutletNode = mchpHR.outletModelObject()->cast<Node>();
  ASSERT_TRUE(waterHeater.addToNode(mchpHROutletNode));
  EXPECT_TRUE(waterHeater.plantLoop());

  // Create a plantEquipmentOperationHeatingLoad
  PlantEquipmentOperationHeatingLoad operation(model);
  operation.setName(plantLoop.name().get() + " PlantEquipmentOperationHeatingLoad");
  ASSERT_TRUE(plantLoop.setPlantEquipmentOperationHeatingLoad(operation));
  ASSERT_TRUE(operation.addEquipment(mchpHR));
  ASSERT_TRUE(operation.addEquipment(waterHeater));

  // Create an ELCD
  ElectricLoadCenterDistribution elcd = ElectricLoadCenterDistribution(model);
  elcd.setName("Capstone C65 ELCD");
  elcd.setElectricalBussType("AlternatingCurrent");
  elcd.addGenerator(mchp);

  // orphan the generator from the ELCD
  boost::optional<ElectricLoadCenterDistribution> elcd2 = mchp.electricLoadCenterDistribution();
  elcd2.get().remove();
  EXPECT_FALSE(mchp.electricLoadCenterDistribution());

  // Forward Translates
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, forwardTranslator.errors().size());
  //ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::WaterHeater_Mixed).size());
  ASSERT_EQ(0u, workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Distribution).size());

  // model.save(toPath("./ForwardTranslatorGeneratorMicroTurbine_ELCD_orhpan.osm"), true);
  // workspace.save(toPath("./ForwardTranslatorGeneratorMicroTurbine_ELCD_orphan.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslator_GeneratorMicroTurbine) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::Minimal, openstudio::IddFileType::EnergyPlus);

  // electric load center distribution
  openstudio::IdfObject idfObject1(openstudio::IddObjectType::ElectricLoadCenter_Distribution);
  idfObject1.setString(ElectricLoadCenter_DistributionFields::Name, "Electric Load Center Distribution 1");

  openstudio::WorkspaceObject epELCD = workspace.addObject(idfObject1).get();

  // generator micro turbine
  openstudio::IdfObject idfObject2(openstudio::IddObjectType::Generator_MicroTurbine);
  idfObject2.setString(Generator_MicroTurbineFields::Name, "Generator Micro Turbine 1");

  openstudio::WorkspaceObject epGenerator = workspace.addObject(idfObject2).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  /*   std::vector<ElectricLoadCenterDistribution> elcds = model.getConcreteModelObjects<ElectricLoadCenterDistribution>();
  ASSERT_EQ(1u, elcds.size());
  ElectricLoadCenterDistribution elcd = elcds[0];
  EXPECT_EQ("Electric Load Center Distribution 1", elcd.name().get());
  ASSERT_EQ(1u, elcd.generators().size()); */

  std::vector<GeneratorMicroTurbine> generators = model.getConcreteModelObjects<GeneratorMicroTurbine>();
  ASSERT_EQ(1u, generators.size());
  GeneratorMicroTurbine generator = generators[0];
  EXPECT_EQ("Generator Micro Turbine 1", generator.name().get());
  EXPECT_FALSE(generator.availabilitySchedule());

  /* EXPECT_EQ(generator.name().get(), elcd.generators()[0].name().get()); */
}
