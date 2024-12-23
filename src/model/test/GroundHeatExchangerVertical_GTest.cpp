/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../GroundHeatExchangerVertical.hpp"
#include "../GroundHeatExchangerVertical_Impl.hpp"
#include "../SiteGroundTemperatureUndisturbedKusudaAchenbach.hpp"
#include "../SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl.hpp"
#include "../SiteGroundTemperatureUndisturbedXing.hpp"
#include "../SiteGroundTemperatureUndisturbedXing_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, GroundHeatExchangerVertical_GroundHeatExchangerVertical) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;
      GroundHeatExchangerVertical testObject = GroundHeatExchangerVertical(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  {
    Model m;
    GroundHeatExchangerVertical gh(m);

    ASSERT_TRUE(gh.designFlowRate());
    EXPECT_EQ(0.0033, gh.designFlowRate().get());
    ASSERT_TRUE(gh.numberofBoreHoles());
    EXPECT_EQ(120, gh.numberofBoreHoles().get());
    EXPECT_EQ(1.0, gh.boreHoleTopDepth());
    ASSERT_TRUE(gh.boreHoleLength());
    EXPECT_EQ(76.2, gh.boreHoleLength().get());
    ASSERT_TRUE(gh.boreHoleRadius());
    EXPECT_EQ(0.635080E-01, gh.boreHoleRadius().get());
    ASSERT_TRUE(gh.groundThermalConductivity());
    EXPECT_EQ(0.692626, gh.groundThermalConductivity().get());
    ASSERT_TRUE(gh.groundThermalHeatCapacity());
    EXPECT_EQ(0.234700E+07, gh.groundThermalHeatCapacity().get());
    ASSERT_TRUE(gh.groundTemperature());
    EXPECT_EQ(13.375, gh.groundTemperature().get());
    ASSERT_TRUE(gh.groutThermalConductivity());
    EXPECT_EQ(0.692626, gh.groutThermalConductivity().get());
    ASSERT_TRUE(gh.pipeThermalConductivity());
    EXPECT_EQ(0.391312, gh.pipeThermalConductivity().get());
    ASSERT_TRUE(gh.pipeOutDiameter());
    EXPECT_EQ(2.66667E-02, gh.pipeOutDiameter().get());
    ASSERT_TRUE(gh.uTubeDistance());
    EXPECT_EQ(2.53977E-02, gh.uTubeDistance().get());
    ASSERT_TRUE(gh.pipeThickness());
    EXPECT_EQ(2.41285E-03, gh.pipeThickness().get());
    ASSERT_TRUE(gh.maximumLengthofSimulation());
    EXPECT_EQ(2, gh.maximumLengthofSimulation().get());
    EXPECT_EQ(0.0005, gh.gFunctionReferenceRatio());
    EXPECT_FALSE(gh.isGFunctionReferenceRatioDefaulted());
    ModelObject undisturbedGroundTemperatureModel = gh.undisturbedGroundTemperatureModel();
    boost::optional<SiteGroundTemperatureUndisturbedKusudaAchenbach> uka =
      undisturbedGroundTemperatureModel.optionalCast<SiteGroundTemperatureUndisturbedKusudaAchenbach>();
    ASSERT_TRUE(uka);
  }

  {
    Model m;
    SiteGroundTemperatureUndisturbedKusudaAchenbach sgt(m);
    GroundHeatExchangerVertical gh(m, sgt);

    ASSERT_TRUE(gh.designFlowRate());
    EXPECT_EQ(0.0033, gh.designFlowRate().get());
    ASSERT_TRUE(gh.numberofBoreHoles());
    EXPECT_EQ(120, gh.numberofBoreHoles().get());
    EXPECT_EQ(1.0, gh.boreHoleTopDepth());
    ASSERT_TRUE(gh.boreHoleLength());
    EXPECT_EQ(76.2, gh.boreHoleLength().get());
    ASSERT_TRUE(gh.boreHoleRadius());
    EXPECT_EQ(0.635080E-01, gh.boreHoleRadius().get());
    ASSERT_TRUE(gh.groundThermalConductivity());
    EXPECT_EQ(0.692626, gh.groundThermalConductivity().get());
    ASSERT_TRUE(gh.groundThermalHeatCapacity());
    EXPECT_EQ(0.234700E+07, gh.groundThermalHeatCapacity().get());
    ASSERT_TRUE(gh.groundTemperature());
    EXPECT_EQ(13.375, gh.groundTemperature().get());
    ASSERT_TRUE(gh.groutThermalConductivity());
    EXPECT_EQ(0.692626, gh.groutThermalConductivity().get());
    ASSERT_TRUE(gh.pipeThermalConductivity());
    EXPECT_EQ(0.391312, gh.pipeThermalConductivity().get());
    ASSERT_TRUE(gh.pipeOutDiameter());
    EXPECT_EQ(2.66667E-02, gh.pipeOutDiameter().get());
    ASSERT_TRUE(gh.uTubeDistance());
    EXPECT_EQ(2.53977E-02, gh.uTubeDistance().get());
    ASSERT_TRUE(gh.pipeThickness());
    EXPECT_EQ(2.41285E-03, gh.pipeThickness().get());
    ASSERT_TRUE(gh.maximumLengthofSimulation());
    EXPECT_EQ(2, gh.maximumLengthofSimulation().get());
    EXPECT_EQ(0.0005, gh.gFunctionReferenceRatio());
    EXPECT_FALSE(gh.isGFunctionReferenceRatioDefaulted());
    ModelObject undisturbedGroundTemperatureModel = gh.undisturbedGroundTemperatureModel();
    boost::optional<SiteGroundTemperatureUndisturbedKusudaAchenbach> uka =
      undisturbedGroundTemperatureModel.optionalCast<SiteGroundTemperatureUndisturbedKusudaAchenbach>();
    ASSERT_TRUE(uka);
    EXPECT_EQ(sgt, uka.get());
  }
}

TEST_F(ModelFixture, GroundHeatExchangerVertical_SetGetFields) {
  Model m;
  GroundHeatExchangerVertical gh(m);

  EXPECT_TRUE(gh.setDesignFlowRate(0.004));
  EXPECT_TRUE(gh.setNumberofBoreHoles(100));
  EXPECT_TRUE(gh.setBoreHoleTopDepth(1.25));
  EXPECT_TRUE(gh.setBoreHoleLength(80.0));
  EXPECT_TRUE(gh.setBoreHoleRadius(0.7E-01));
  EXPECT_TRUE(gh.setGroundThermalConductivity(0.7));
  EXPECT_TRUE(gh.setGroundThermalHeatCapacity(0.3E+07));
  EXPECT_TRUE(gh.setGroundTemperature(14.0));
  EXPECT_TRUE(gh.setGroutThermalConductivity(0.7));
  EXPECT_TRUE(gh.setPipeThermalConductivity(0.4));
  EXPECT_TRUE(gh.setPipeOutDiameter(2.7E-02));
  EXPECT_TRUE(gh.setUTubeDistance(2.6E-02));
  EXPECT_TRUE(gh.setPipeThickness(2.5E-03));
  EXPECT_TRUE(gh.setMaximumLengthofSimulation(3));
  EXPECT_TRUE(gh.setGFunctionReferenceRatio(0.001));

  SiteGroundTemperatureUndisturbedKusudaAchenbach sgt(m);
  EXPECT_TRUE(gh.setUndisturbedGroundTemperatureModel(sgt));

  ASSERT_TRUE(gh.designFlowRate());
  EXPECT_EQ(0.004, gh.designFlowRate().get());
  ASSERT_TRUE(gh.numberofBoreHoles());
  EXPECT_EQ(100, gh.numberofBoreHoles().get());
  EXPECT_EQ(1.25, gh.boreHoleTopDepth());
  ASSERT_TRUE(gh.boreHoleLength());
  EXPECT_EQ(80.0, gh.boreHoleLength().get());
  ASSERT_TRUE(gh.boreHoleRadius());
  EXPECT_EQ(0.7E-01, gh.boreHoleRadius().get());
  ASSERT_TRUE(gh.groundThermalConductivity());
  EXPECT_EQ(0.7, gh.groundThermalConductivity().get());
  ASSERT_TRUE(gh.groundThermalHeatCapacity());
  EXPECT_EQ(0.3E+07, gh.groundThermalHeatCapacity().get());
  ASSERT_TRUE(gh.groundTemperature());
  EXPECT_EQ(14.0, gh.groundTemperature().get());
  ASSERT_TRUE(gh.groutThermalConductivity());
  EXPECT_EQ(0.7, gh.groutThermalConductivity().get());
  ASSERT_TRUE(gh.pipeThermalConductivity());
  EXPECT_EQ(0.4, gh.pipeThermalConductivity().get());
  ASSERT_TRUE(gh.pipeOutDiameter());
  EXPECT_EQ(2.7E-02, gh.pipeOutDiameter().get());
  ASSERT_TRUE(gh.uTubeDistance());
  EXPECT_EQ(2.6E-02, gh.uTubeDistance().get());
  ASSERT_TRUE(gh.pipeThickness());
  EXPECT_EQ(2.5E-03, gh.pipeThickness().get());
  ASSERT_TRUE(gh.maximumLengthofSimulation());
  EXPECT_EQ(3, gh.maximumLengthofSimulation().get());
  EXPECT_EQ(0.001, gh.gFunctionReferenceRatio());
  EXPECT_FALSE(gh.isGFunctionReferenceRatioDefaulted());
  ModelObject undisturbedGroundTemperatureModel = gh.undisturbedGroundTemperatureModel();
  boost::optional<SiteGroundTemperatureUndisturbedKusudaAchenbach> uka =
    undisturbedGroundTemperatureModel.optionalCast<SiteGroundTemperatureUndisturbedKusudaAchenbach>();
  ASSERT_TRUE(uka);
  EXPECT_EQ(sgt, uka.get());

  SiteGroundTemperatureUndisturbedXing sgt2(m);
  EXPECT_TRUE(gh.setUndisturbedGroundTemperatureModel(sgt2));

  ASSERT_TRUE(gh.designFlowRate());
  EXPECT_EQ(0.004, gh.designFlowRate().get());
  ASSERT_TRUE(gh.numberofBoreHoles());
  EXPECT_EQ(100, gh.numberofBoreHoles().get());
  EXPECT_EQ(1.25, gh.boreHoleTopDepth());
  ASSERT_TRUE(gh.boreHoleLength());
  EXPECT_EQ(80.0, gh.boreHoleLength().get());
  ASSERT_TRUE(gh.boreHoleRadius());
  EXPECT_EQ(0.7E-01, gh.boreHoleRadius().get());
  ASSERT_TRUE(gh.groundThermalConductivity());
  EXPECT_EQ(0.7, gh.groundThermalConductivity().get());
  ASSERT_TRUE(gh.groundThermalHeatCapacity());
  EXPECT_EQ(0.3E+07, gh.groundThermalHeatCapacity().get());
  ASSERT_TRUE(gh.groundTemperature());
  EXPECT_EQ(14.0, gh.groundTemperature().get());
  ASSERT_TRUE(gh.groutThermalConductivity());
  EXPECT_EQ(0.7, gh.groutThermalConductivity().get());
  ASSERT_TRUE(gh.pipeThermalConductivity());
  EXPECT_EQ(0.4, gh.pipeThermalConductivity().get());
  ASSERT_TRUE(gh.pipeOutDiameter());
  EXPECT_EQ(2.7E-02, gh.pipeOutDiameter().get());
  ASSERT_TRUE(gh.uTubeDistance());
  EXPECT_EQ(2.6E-02, gh.uTubeDistance().get());
  ASSERT_TRUE(gh.pipeThickness());
  EXPECT_EQ(2.5E-03, gh.pipeThickness().get());
  ASSERT_TRUE(gh.maximumLengthofSimulation());
  EXPECT_EQ(3, gh.maximumLengthofSimulation().get());
  EXPECT_EQ(0.001, gh.gFunctionReferenceRatio());
  EXPECT_FALSE(gh.isGFunctionReferenceRatioDefaulted());
  ModelObject undisturbedGroundTemperatureModel2 = gh.undisturbedGroundTemperatureModel();
  boost::optional<SiteGroundTemperatureUndisturbedXing> ux = undisturbedGroundTemperatureModel2.optionalCast<SiteGroundTemperatureUndisturbedXing>();
  ASSERT_TRUE(ux);
  EXPECT_EQ(sgt2, ux.get());
}

TEST_F(ModelFixture, GroundHeatExchangerVertical_Connections) {
  Model m;
  GroundHeatExchangerVertical testObject(m);

  Node inletNode(m);
  Node outletNode(m);

  m.connect(inletNode, inletNode.outletPort(), testObject, testObject.inletPort());
  m.connect(testObject, testObject.outletPort(), outletNode, outletNode.inletPort());

  ASSERT_TRUE(testObject.inletModelObject());
  ASSERT_TRUE(testObject.outletModelObject());

  EXPECT_EQ(inletNode.handle(), testObject.inletModelObject()->handle());
  EXPECT_EQ(outletNode.handle(), testObject.outletModelObject()->handle());
}

TEST_F(ModelFixture, GroundHeatExchangerVertical_addToNode) {
  Model m;
  GroundHeatExchangerVertical testObject(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  auto testObjectClone = testObject.clone(m).cast<GroundHeatExchangerVertical>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)9, plantLoop.supplyComponents().size());
}

TEST_F(ModelFixture, GroundHeatExchangerVertical_AddRemoveSupplyBranchForComponent) {
  Model model;
  GroundHeatExchangerVertical testObject(model);

  PlantLoop plantLoop(model);

  EXPECT_TRUE(plantLoop.addSupplyBranchForComponent(testObject));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());
  EXPECT_NE((unsigned)9, plantLoop.supplyComponents().size());

  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());

  EXPECT_TRUE(plantLoop.removeSupplyBranchWithComponent(testObject));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());
  EXPECT_NE((unsigned)7, plantLoop.supplyComponents().size());
}

TEST_F(ModelFixture, GroundHeatExchangerVertical_AddDemandBranchForComponent) {
  Model model;
  GroundHeatExchangerVertical testObject(model);

  PlantLoop plantLoop(model);

  EXPECT_FALSE(plantLoop.addDemandBranchForComponent(testObject));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());
  EXPECT_NE((unsigned)7, plantLoop.demandComponents().size());
}

TEST_F(ModelFixture, GroundHeatExchangerVertical_AddToNodeTwoSameObjects) {
  Model model;
  GroundHeatExchangerVertical testObject(model);

  PlantLoop plantLoop(model);

  Node supplyOutletNode = plantLoop.supplyOutletNode();
  testObject.addToNode(supplyOutletNode);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
}

TEST_F(ModelFixture, GroundHeatExchangerVertical_Remove) {
  Model model;
  GroundHeatExchangerVertical testObject(model);

  PlantLoop plantLoop(model);

  Node supplyOutletNode = plantLoop.supplyOutletNode();
  testObject.addToNode(supplyOutletNode);

  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());

  testObject.remove();
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());
}

//test cloning the object
TEST_F(ModelFixture, GroundHeatExchangerVertical_Clone) {
  Model m;
  //make an object to clone, and edit some property to make sure the clone worked
  GroundHeatExchangerVertical testObject(m);
  testObject.setDesignFlowRate(3.14);

  //clone into the same model
  auto testObjectClone = testObject.clone(m).cast<GroundHeatExchangerVertical>();
  EXPECT_EQ(3.14, testObjectClone.designFlowRate().get());

  //clone into another model
  Model m2;
  auto testObjectClone2 = testObject.clone(m2).cast<GroundHeatExchangerVertical>();
  EXPECT_EQ(3.14, testObjectClone2.designFlowRate().get());

  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());
}

TEST_F(ModelFixture, GroundHeatExchangerVertical_GFunctions) {
  Model model;
  GroundHeatExchangerVertical testObject(model);

  std::vector<GFunction> gFunctions = testObject.gFunctions();
  EXPECT_EQ(35, gFunctions.size());

  testObject.removeAllGFunctions();

  gFunctions = testObject.gFunctions();
  EXPECT_EQ(0, gFunctions.size());

  EXPECT_TRUE(testObject.addGFunction(1.0, 1.5));
  gFunctions = testObject.gFunctions();
  EXPECT_EQ(1, gFunctions.size());

  testObject.addGFunction(2.0, 2.5);

  testObject.removeGFunction(0);
  gFunctions = testObject.gFunctions();
  EXPECT_EQ(1, gFunctions.size());

  EXPECT_DOUBLE_EQ(2.0, gFunctions[0].lnValue());
  EXPECT_DOUBLE_EQ(2.5, gFunctions[0].gValue());

  testObject.removeAllGFunctions();
  for (int i = 0; i < 100; i++) {
    testObject.addGFunction(i, i + 0.5);
  }
  gFunctions = testObject.gFunctions();
  EXPECT_EQ(100, gFunctions.size());
  EXPECT_THROW(testObject.addGFunction(1.0, 1.5), openstudio::Exception);
}
