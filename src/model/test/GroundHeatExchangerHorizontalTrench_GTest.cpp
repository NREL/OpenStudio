/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../GroundHeatExchangerHorizontalTrench.hpp"
#include "../GroundHeatExchangerHorizontalTrench_Impl.hpp"
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

TEST_F(ModelFixture, GroundHeatExchangerHorizontalTrench_GroundHeatExchangerHorizontalTrench) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      GroundHeatExchangerHorizontalTrench gh(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  {
    Model m;
    GroundHeatExchangerHorizontalTrench gh(m);

    EXPECT_EQ(0.004, gh.designFlowRate());
    EXPECT_EQ(75, gh.trenchLengthinPipeAxialDirection());
    EXPECT_EQ(2, gh.numberofTrenches());
    EXPECT_EQ(2.0, gh.horizontalSpacingBetweenPipes());
    EXPECT_EQ(0.016, gh.pipeInnerDiameter());
    EXPECT_EQ(0.02667, gh.pipeOuterDiameter());
    EXPECT_EQ(1.25, gh.burialDepth());
    EXPECT_EQ(1.08, gh.soilThermalConductivity());
    EXPECT_EQ(962, gh.soilDensity());
    EXPECT_EQ(2576, gh.soilSpecificHeat());
    EXPECT_EQ(0.3895, gh.pipeThermalConductivity());
    EXPECT_EQ(641, gh.pipeDensity());
    EXPECT_EQ(2405, gh.pipeSpecificHeat());
    EXPECT_EQ(30, gh.soilMoistureContentPercent());
    EXPECT_EQ(50, gh.soilMoistureContentPercentatSaturation());
    EXPECT_EQ("KusudaAchenbach", gh.groundTemperatureModel());
    EXPECT_FALSE(gh.isGroundTemperatureModelDefaulted());
    EXPECT_EQ(15.5, gh.kusudaAchenbachAverageSurfaceTemperature());
    EXPECT_EQ(12.8, gh.kusudaAchenbachAverageAmplitudeofSurfaceTemperature());
    EXPECT_EQ(17.3, gh.kusudaAchenbachPhaseShiftofMinimumSurfaceTemperature());
    EXPECT_EQ(0.408, gh.evapotranspirationGroundCoverParameter());
    ModelObject undisturbedGroundTemperatureModel = gh.undisturbedGroundTemperatureModel();
    boost::optional<SiteGroundTemperatureUndisturbedKusudaAchenbach> uka =
      undisturbedGroundTemperatureModel.optionalCast<SiteGroundTemperatureUndisturbedKusudaAchenbach>();
    ASSERT_TRUE(uka);
  }

  {
    Model m;
    SiteGroundTemperatureUndisturbedKusudaAchenbach sgt(m);
    GroundHeatExchangerHorizontalTrench gh(m, sgt);

    EXPECT_EQ(0.004, gh.designFlowRate());
    EXPECT_EQ(75, gh.trenchLengthinPipeAxialDirection());
    EXPECT_EQ(2, gh.numberofTrenches());
    EXPECT_EQ(2.0, gh.horizontalSpacingBetweenPipes());
    EXPECT_EQ(0.016, gh.pipeInnerDiameter());
    EXPECT_EQ(0.02667, gh.pipeOuterDiameter());
    EXPECT_EQ(1.25, gh.burialDepth());
    EXPECT_EQ(1.08, gh.soilThermalConductivity());
    EXPECT_EQ(962, gh.soilDensity());
    EXPECT_EQ(2576, gh.soilSpecificHeat());
    EXPECT_EQ(0.3895, gh.pipeThermalConductivity());
    EXPECT_EQ(641, gh.pipeDensity());
    EXPECT_EQ(2405, gh.pipeSpecificHeat());
    EXPECT_EQ(30, gh.soilMoistureContentPercent());
    EXPECT_EQ(50, gh.soilMoistureContentPercentatSaturation());
    EXPECT_EQ("KusudaAchenbach", gh.groundTemperatureModel());
    EXPECT_FALSE(gh.isGroundTemperatureModelDefaulted());
    EXPECT_EQ(15.5, gh.kusudaAchenbachAverageSurfaceTemperature());
    EXPECT_EQ(12.8, gh.kusudaAchenbachAverageAmplitudeofSurfaceTemperature());
    EXPECT_EQ(17.3, gh.kusudaAchenbachPhaseShiftofMinimumSurfaceTemperature());
    EXPECT_EQ(0.408, gh.evapotranspirationGroundCoverParameter());
    ModelObject undisturbedGroundTemperatureModel = gh.undisturbedGroundTemperatureModel();
    boost::optional<SiteGroundTemperatureUndisturbedKusudaAchenbach> uka =
      undisturbedGroundTemperatureModel.optionalCast<SiteGroundTemperatureUndisturbedKusudaAchenbach>();
    ASSERT_TRUE(uka);
    EXPECT_EQ(sgt, uka.get());
  }
}

TEST_F(ModelFixture, GroundHeatExchangerHorizontalTrench_SetGetFields) {
  Model m;
  GroundHeatExchangerHorizontalTrench gh(m);

  EXPECT_TRUE(gh.setDesignFlowRate(0.005));
  EXPECT_TRUE(gh.setTrenchLengthinPipeAxialDirection(80));
  EXPECT_TRUE(gh.setNumberofTrenches(3));
  EXPECT_TRUE(gh.setHorizontalSpacingBetweenPipes(3.0));
  EXPECT_TRUE(gh.setPipeInnerDiameter(0.02));
  EXPECT_TRUE(gh.setPipeOuterDiameter(0.03));
  EXPECT_TRUE(gh.setBurialDepth(1.3));
  EXPECT_TRUE(gh.setSoilThermalConductivity(1.1));
  EXPECT_TRUE(gh.setSoilDensity(965));
  EXPECT_TRUE(gh.setSoilSpecificHeat(2600));
  EXPECT_TRUE(gh.setPipeThermalConductivity(0.4));
  EXPECT_TRUE(gh.setPipeDensity(650));
  EXPECT_TRUE(gh.setPipeSpecificHeat(2500));
  EXPECT_TRUE(gh.setSoilMoistureContentPercent(40));
  EXPECT_TRUE(gh.setSoilMoistureContentPercentatSaturation(60));
  EXPECT_FALSE(gh.setGroundTemperatureModel("SiteGroundTemperature"));
  gh.resetGroundTemperatureModel();
  EXPECT_TRUE(gh.setKusudaAchenbachAverageSurfaceTemperature(16.0));
  EXPECT_TRUE(gh.setKusudaAchenbachAverageAmplitudeofSurfaceTemperature(13.0));
  EXPECT_TRUE(gh.setKusudaAchenbachPhaseShiftofMinimumSurfaceTemperature(18.0));
  EXPECT_TRUE(gh.setEvapotranspirationGroundCoverParameter(0.5));

  SiteGroundTemperatureUndisturbedKusudaAchenbach sgt(m);
  EXPECT_TRUE(gh.setUndisturbedGroundTemperatureModel(sgt));

  EXPECT_EQ(0.005, gh.designFlowRate());
  EXPECT_EQ(80, gh.trenchLengthinPipeAxialDirection());
  EXPECT_EQ(3, gh.numberofTrenches());
  EXPECT_EQ(3.0, gh.horizontalSpacingBetweenPipes());
  EXPECT_EQ(0.02, gh.pipeInnerDiameter());
  EXPECT_EQ(0.03, gh.pipeOuterDiameter());
  EXPECT_EQ(1.3, gh.burialDepth());
  EXPECT_EQ(1.1, gh.soilThermalConductivity());
  EXPECT_EQ(965, gh.soilDensity());
  EXPECT_EQ(2600, gh.soilSpecificHeat());
  EXPECT_EQ(0.4, gh.pipeThermalConductivity());
  EXPECT_EQ(650, gh.pipeDensity());
  EXPECT_EQ(2500, gh.pipeSpecificHeat());
  EXPECT_EQ(40, gh.soilMoistureContentPercent());
  EXPECT_EQ(60, gh.soilMoistureContentPercentatSaturation());
  EXPECT_EQ("KusudaAchenbach", gh.groundTemperatureModel());
  EXPECT_FALSE(gh.isGroundTemperatureModelDefaulted());
  EXPECT_EQ(15.5, gh.kusudaAchenbachAverageSurfaceTemperature());
  EXPECT_EQ(12.8, gh.kusudaAchenbachAverageAmplitudeofSurfaceTemperature());
  EXPECT_EQ(17.3, gh.kusudaAchenbachPhaseShiftofMinimumSurfaceTemperature());
  EXPECT_EQ(0.5, gh.evapotranspirationGroundCoverParameter());
  ModelObject undisturbedGroundTemperatureModel = gh.undisturbedGroundTemperatureModel();
  boost::optional<SiteGroundTemperatureUndisturbedKusudaAchenbach> uka =
    undisturbedGroundTemperatureModel.optionalCast<SiteGroundTemperatureUndisturbedKusudaAchenbach>();
  ASSERT_TRUE(uka);
  EXPECT_EQ(sgt, uka.get());

  SiteGroundTemperatureUndisturbedXing sgt2(m);
  EXPECT_TRUE(gh.setUndisturbedGroundTemperatureModel(sgt2));

  EXPECT_EQ(0.005, gh.designFlowRate());
  EXPECT_EQ(80, gh.trenchLengthinPipeAxialDirection());
  EXPECT_EQ(3, gh.numberofTrenches());
  EXPECT_EQ(3.0, gh.horizontalSpacingBetweenPipes());
  EXPECT_EQ(0.02, gh.pipeInnerDiameter());
  EXPECT_EQ(0.03, gh.pipeOuterDiameter());
  EXPECT_EQ(1.3, gh.burialDepth());
  EXPECT_EQ(1.1, gh.soilThermalConductivity());
  EXPECT_EQ(965, gh.soilDensity());
  EXPECT_EQ(2600, gh.soilSpecificHeat());
  EXPECT_EQ(0.4, gh.pipeThermalConductivity());
  EXPECT_EQ(650, gh.pipeDensity());
  EXPECT_EQ(2500, gh.pipeSpecificHeat());
  EXPECT_EQ(40, gh.soilMoistureContentPercent());
  EXPECT_EQ(60, gh.soilMoistureContentPercentatSaturation());
  EXPECT_EQ("SiteGroundTemperature", gh.groundTemperatureModel());
  EXPECT_FALSE(gh.isGroundTemperatureModelDefaulted());
  EXPECT_THROW(gh.kusudaAchenbachAverageSurfaceTemperature(), openstudio::Exception);
  EXPECT_THROW(gh.kusudaAchenbachAverageAmplitudeofSurfaceTemperature(), openstudio::Exception);
  EXPECT_THROW(gh.kusudaAchenbachPhaseShiftofMinimumSurfaceTemperature(), openstudio::Exception);
  EXPECT_EQ(0.5, gh.evapotranspirationGroundCoverParameter());
  ModelObject undisturbedGroundTemperatureModel2 = gh.undisturbedGroundTemperatureModel();
  boost::optional<SiteGroundTemperatureUndisturbedXing> ux = undisturbedGroundTemperatureModel2.optionalCast<SiteGroundTemperatureUndisturbedXing>();
  ASSERT_TRUE(ux);
  EXPECT_EQ(sgt2, ux.get());
}

TEST_F(ModelFixture, GroundHeatExchangerHorizontalTrench_Clone) {
  {
    Model m;
    SiteGroundTemperatureUndisturbedKusudaAchenbach sgt(m);
    GroundHeatExchangerHorizontalTrench gh(m, sgt);

    EXPECT_EQ(1u, m.getConcreteModelObjects<GroundHeatExchangerHorizontalTrench>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedKusudaAchenbach>().size());

    auto ghClone = gh.clone(m).cast<GroundHeatExchangerHorizontalTrench>();
    EXPECT_EQ(2u, m.getConcreteModelObjects<GroundHeatExchangerHorizontalTrench>().size());
    EXPECT_EQ(2u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedKusudaAchenbach>().size());

    ModelObject undisturbedGroundTemperatureModel = gh.undisturbedGroundTemperatureModel();
    boost::optional<SiteGroundTemperatureUndisturbedKusudaAchenbach> uka =
      undisturbedGroundTemperatureModel.optionalCast<SiteGroundTemperatureUndisturbedKusudaAchenbach>();
    ASSERT_TRUE(uka);
    EXPECT_EQ(sgt, uka.get());

    ModelObject undisturbedGroundTemperatureModelClone = ghClone.undisturbedGroundTemperatureModel();
    boost::optional<SiteGroundTemperatureUndisturbedKusudaAchenbach> ukaClone =
      undisturbedGroundTemperatureModelClone.optionalCast<SiteGroundTemperatureUndisturbedKusudaAchenbach>();
    ASSERT_TRUE(ukaClone);
    EXPECT_NE(sgt, ukaClone.get());

    gh.remove();
    EXPECT_EQ(1u, m.getConcreteModelObjects<GroundHeatExchangerHorizontalTrench>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedKusudaAchenbach>().size());

    GroundHeatExchangerHorizontalTrench gh2(m, ukaClone.get());
    EXPECT_EQ(2u, m.getConcreteModelObjects<GroundHeatExchangerHorizontalTrench>().size());
    ghClone.remove();
    EXPECT_EQ(1u, m.getConcreteModelObjects<GroundHeatExchangerHorizontalTrench>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedKusudaAchenbach>().size());

    ModelObject undisturbedGroundTemperatureModel2 = gh2.undisturbedGroundTemperatureModel();
    boost::optional<SiteGroundTemperatureUndisturbedKusudaAchenbach> uka2 =
      undisturbedGroundTemperatureModel2.optionalCast<SiteGroundTemperatureUndisturbedKusudaAchenbach>();
    ASSERT_TRUE(uka2);
    EXPECT_EQ(ukaClone.get(), uka2.get());
  }

  {
    Model m;
    SiteGroundTemperatureUndisturbedXing sgt(m);
    GroundHeatExchangerHorizontalTrench gh(m, sgt);

    EXPECT_EQ(1u, m.getConcreteModelObjects<GroundHeatExchangerHorizontalTrench>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedXing>().size());

    auto ghClone = gh.clone(m).cast<GroundHeatExchangerHorizontalTrench>();
    EXPECT_EQ(2u, m.getConcreteModelObjects<GroundHeatExchangerHorizontalTrench>().size());
    EXPECT_EQ(2u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedXing>().size());

    ModelObject undisturbedGroundTemperatureModel = gh.undisturbedGroundTemperatureModel();
    boost::optional<SiteGroundTemperatureUndisturbedXing> ux = undisturbedGroundTemperatureModel.optionalCast<SiteGroundTemperatureUndisturbedXing>();
    ASSERT_TRUE(ux);
    EXPECT_EQ(sgt, ux.get());

    ModelObject undisturbedGroundTemperatureModelClone = ghClone.undisturbedGroundTemperatureModel();
    boost::optional<SiteGroundTemperatureUndisturbedXing> uxClone =
      undisturbedGroundTemperatureModelClone.optionalCast<SiteGroundTemperatureUndisturbedXing>();
    ASSERT_TRUE(uxClone);
    EXPECT_NE(sgt, uxClone.get());

    gh.remove();
    EXPECT_EQ(1u, m.getConcreteModelObjects<GroundHeatExchangerHorizontalTrench>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedXing>().size());

    GroundHeatExchangerHorizontalTrench gh2(m, uxClone.get());
    EXPECT_EQ(2u, m.getConcreteModelObjects<GroundHeatExchangerHorizontalTrench>().size());
    ghClone.remove();
    EXPECT_EQ(1u, m.getConcreteModelObjects<GroundHeatExchangerHorizontalTrench>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedXing>().size());

    ModelObject undisturbedGroundTemperatureModel2 = gh2.undisturbedGroundTemperatureModel();
    boost::optional<SiteGroundTemperatureUndisturbedXing> ux2 =
      undisturbedGroundTemperatureModel2.optionalCast<SiteGroundTemperatureUndisturbedXing>();
    ASSERT_TRUE(ux2);
    EXPECT_EQ(uxClone.get(), ux2.get());
  }

  //clone into another model
  {
    Model m;
    GroundHeatExchangerHorizontalTrench testObject(m);

    auto testObjectClone = testObject.clone(m).cast<GroundHeatExchangerHorizontalTrench>();

    Model m2;
    auto testObjectClone2 = testObject.clone(m2).cast<GroundHeatExchangerHorizontalTrench>();

    EXPECT_NE(testObjectClone2, testObjectClone);
    EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());

    ModelObject undisturbedGroundTemperatureModelClone = testObjectClone.undisturbedGroundTemperatureModel();
    boost::optional<SiteGroundTemperatureUndisturbedKusudaAchenbach> sgtClone =
      undisturbedGroundTemperatureModelClone.optionalCast<SiteGroundTemperatureUndisturbedKusudaAchenbach>();
    ASSERT_TRUE(sgtClone);
    EXPECT_EQ(2u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedKusudaAchenbach>().size());

    ModelObject undisturbedGroundTemperatureModelClone2 = testObjectClone2.undisturbedGroundTemperatureModel();
    boost::optional<SiteGroundTemperatureUndisturbedKusudaAchenbach> sgtClone2 =
      undisturbedGroundTemperatureModelClone2.optionalCast<SiteGroundTemperatureUndisturbedKusudaAchenbach>();
    ASSERT_TRUE(sgtClone2);
    EXPECT_EQ(1u, m2.getConcreteModelObjects<SiteGroundTemperatureUndisturbedKusudaAchenbach>().size());
  }
}

TEST_F(ModelFixture, GroundHeatExchangerHorizontalTrench_addToNode) {
  Model m;
  GroundHeatExchangerHorizontalTrench testObject(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  EXPECT_TRUE(plantLoop.setFluidType("PropyleneGlycol"));
  EXPECT_TRUE(plantLoop.setGlycolConcentration(50));

  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  auto testObjectClone = testObject.clone(m).cast<GroundHeatExchangerHorizontalTrench>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)9, plantLoop.supplyComponents().size());

  EXPECT_EQ(plantLoop.fluidType(), "PropyleneGlycol");
  EXPECT_EQ(plantLoop.glycolConcentration(), 50);
}
