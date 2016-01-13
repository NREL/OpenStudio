/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "../Model.hpp"
//#include <model/Node.hpp>
//#include <model/Node_Impl.hpp>
#include "../ZoneHVACComponent.hpp"
#include "../ZoneHVACComponent_Impl.hpp"

#include "../CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../CoilCoolingWaterToAirHeatPumpEquationFit_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

//Test construction of the coil

TEST_F(ModelFixture,CoilCoolingWaterToAirHeatPumpEquationFit_Check_Constructor) {

  Model model;
  CoilCoolingWaterToAirHeatPumpEquationFit testCoilCoolingWaterToAirHeatPumpEquationFit(model);

  // Testing .idd object type

  EXPECT_EQ(openstudio::IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_EquationFit,testCoilCoolingWaterToAirHeatPumpEquationFit.iddObjectType().value());

}

// Test cloning the coil

TEST_F(ModelFixture,CoilCoolingWaterToAirHeatPumpEquationFit_Check_Clone) {

  // Make a coil
  Model model;
  CoilCoolingWaterToAirHeatPumpEquationFit coil(model);
  coil.setRatedAirFlowRate(100);

  // Clone into the same model
  CoilCoolingWaterToAirHeatPumpEquationFit cloneCoil = coil.clone(model).cast<model::CoilCoolingWaterToAirHeatPumpEquationFit>();
  ASSERT_EQ(coil.ratedAirFlowRate(), cloneCoil.ratedAirFlowRate());

  // Clone into another model
  Model model2;
  CoilCoolingWaterToAirHeatPumpEquationFit cloneCoil2 = coil.clone(model2).cast<model::CoilCoolingWaterToAirHeatPumpEquationFit>();
  ASSERT_EQ(coil.ratedAirFlowRate(), cloneCoil2.ratedAirFlowRate());
}

// Test add and remove from air and water nodes

TEST_F(ModelFixture,CoilCoolingWaterToAirHeatPumpEquationFit_addToNodes) {

  Model model;
  CoilCoolingWaterToAirHeatPumpEquationFit coilCoolingWaterToAirHeatPumpEquationFit(model);

  // Check inlet and outlet ports
  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.airInletPort());
  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.airOutletPort());
  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.waterInletPort());
  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.waterOutletPort());

}

// Test the methods that set and get all the fields

TEST_F(ModelFixture,CoilCoolingWaterToAirHeatPumpEquationFit_Check_Getters_and_Setters) {

  Model model;
  CoilCoolingWaterToAirHeatPumpEquationFit coilCoolingWaterToAirHeatPumpEquationFit(model);

  // Field N1 Rated Air Flow Rate. Test set and get Rated Air Flow Rate, and test default, autosize and reset functions

  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.setRatedAirFlowRate(100));
  boost::optional<double> testRatedAirFlowRate = coilCoolingWaterToAirHeatPumpEquationFit.ratedAirFlowRate();

  EXPECT_EQ((*testRatedAirFlowRate),100);
  EXPECT_FALSE(coilCoolingWaterToAirHeatPumpEquationFit.isRatedAirFlowRateDefaulted());
  EXPECT_FALSE(coilCoolingWaterToAirHeatPumpEquationFit.isRatedAirFlowRateAutosized());

  coilCoolingWaterToAirHeatPumpEquationFit.resetRatedAirFlowRate();
  EXPECT_FALSE(coilCoolingWaterToAirHeatPumpEquationFit.ratedAirFlowRate());
  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.isRatedAirFlowRateDefaulted());

  coilCoolingWaterToAirHeatPumpEquationFit.autosizeRatedAirFlowRate();
  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.isRatedAirFlowRateAutosized());

  // Field N2 Rated Water Flow Rate. Test set and get Rated Water Flow Rate, and test default, autosize and reset functions

  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.setRatedWaterFlowRate(100));
  boost::optional<double> testRatedWaterFlowRate = coilCoolingWaterToAirHeatPumpEquationFit.ratedWaterFlowRate();

  EXPECT_EQ((*testRatedWaterFlowRate),100);
  EXPECT_FALSE(coilCoolingWaterToAirHeatPumpEquationFit.isRatedWaterFlowRateDefaulted());
  EXPECT_FALSE(coilCoolingWaterToAirHeatPumpEquationFit.isRatedWaterFlowRateAutosized());

  coilCoolingWaterToAirHeatPumpEquationFit.resetRatedWaterFlowRate();
  EXPECT_FALSE(coilCoolingWaterToAirHeatPumpEquationFit.ratedWaterFlowRate());
  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.isRatedWaterFlowRateDefaulted());

  coilCoolingWaterToAirHeatPumpEquationFit.autosizeRatedWaterFlowRate();
  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.isRatedWaterFlowRateAutosized());

  // Field N3 Rated Total Cooling Capacity. Test set and get Total Cooling Capacity, and test default, autosize and reset functions

  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.setRatedTotalCoolingCapacity(100));
  boost::optional<double> testRatedTotalCoolingCapacity = coilCoolingWaterToAirHeatPumpEquationFit.ratedTotalCoolingCapacity();

  EXPECT_EQ((*testRatedTotalCoolingCapacity),100);
  EXPECT_FALSE(coilCoolingWaterToAirHeatPumpEquationFit.isRatedTotalCoolingCapacityDefaulted());
  EXPECT_FALSE(coilCoolingWaterToAirHeatPumpEquationFit.isRatedTotalCoolingCapacityAutosized());

  coilCoolingWaterToAirHeatPumpEquationFit.resetRatedTotalCoolingCapacity();
  EXPECT_FALSE(coilCoolingWaterToAirHeatPumpEquationFit.ratedTotalCoolingCapacity());
  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.isRatedTotalCoolingCapacityDefaulted());

  coilCoolingWaterToAirHeatPumpEquationFit.autosizeRatedTotalCoolingCapacity();
  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.isRatedTotalCoolingCapacityAutosized());

  // Field N4 Rated Sensible Cooling Capacity. Test set and get Rated Sensible Cooling Capacity, and test default, autosize and reset functions

  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.setRatedSensibleCoolingCapacity(100));
  boost::optional<double> testRatedSensibleCoolingCapacity = coilCoolingWaterToAirHeatPumpEquationFit.ratedSensibleCoolingCapacity();

  EXPECT_EQ((*testRatedSensibleCoolingCapacity),100);
  EXPECT_FALSE(coilCoolingWaterToAirHeatPumpEquationFit.isRatedSensibleCoolingCapacityDefaulted());
  EXPECT_FALSE(coilCoolingWaterToAirHeatPumpEquationFit.isRatedSensibleCoolingCapacityAutosized());

  coilCoolingWaterToAirHeatPumpEquationFit.resetRatedSensibleCoolingCapacity();
  EXPECT_FALSE(coilCoolingWaterToAirHeatPumpEquationFit.ratedSensibleCoolingCapacity());
  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.isRatedSensibleCoolingCapacityDefaulted());

  coilCoolingWaterToAirHeatPumpEquationFit.autosizeRatedSensibleCoolingCapacity();
  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.isRatedSensibleCoolingCapacityAutosized());

  // Field N5 Rated Cooling Coefficient of Performance. Test set and get Rated Cooling Coefficient of Performance

  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.setRatedCoolingCoefficientofPerformance(100));
  boost::optional<double> testRatedCoolingCoefficientofPerformance = coilCoolingWaterToAirHeatPumpEquationFit.ratedCoolingCoefficientofPerformance();
  EXPECT_EQ((*testRatedCoolingCoefficientofPerformance),100);

  // Fields N6 - N10 Check set Cooling Capacity Coefficients
  coilCoolingWaterToAirHeatPumpEquationFit.setTotalCoolingCapacityCoefficient1(1);
  double testTotalCoolingCapacityCoefficient1 = coilCoolingWaterToAirHeatPumpEquationFit.totalCoolingCapacityCoefficient1();
  coilCoolingWaterToAirHeatPumpEquationFit.setTotalCoolingCapacityCoefficient2(2);
  double testTotalCoolingCapacityCoefficient2 = coilCoolingWaterToAirHeatPumpEquationFit.totalCoolingCapacityCoefficient2();
  coilCoolingWaterToAirHeatPumpEquationFit.setTotalCoolingCapacityCoefficient3(3);
  double testTotalCoolingCapacityCoefficient3 = coilCoolingWaterToAirHeatPumpEquationFit.totalCoolingCapacityCoefficient3();
  coilCoolingWaterToAirHeatPumpEquationFit.setTotalCoolingCapacityCoefficient4(4);
  double testTotalCoolingCapacityCoefficient4 = coilCoolingWaterToAirHeatPumpEquationFit.totalCoolingCapacityCoefficient4();
  coilCoolingWaterToAirHeatPumpEquationFit.setTotalCoolingCapacityCoefficient5(5);
  double testTotalCoolingCapacityCoefficient5 = coilCoolingWaterToAirHeatPumpEquationFit.totalCoolingCapacityCoefficient5();

  EXPECT_EQ(testTotalCoolingCapacityCoefficient1,1);
  EXPECT_EQ(testTotalCoolingCapacityCoefficient2,2);
  EXPECT_EQ(testTotalCoolingCapacityCoefficient3,3);
  EXPECT_EQ(testTotalCoolingCapacityCoefficient4,4);
  EXPECT_EQ(testTotalCoolingCapacityCoefficient5,5);


  // Fields N11 - N16 Check set Sensible Capacity Coefficients
  coilCoolingWaterToAirHeatPumpEquationFit.setSensibleCoolingCapacityCoefficient1(1);
  double testSensibleCoolingCapacityCoefficient1 = coilCoolingWaterToAirHeatPumpEquationFit.sensibleCoolingCapacityCoefficient1();
  coilCoolingWaterToAirHeatPumpEquationFit.setSensibleCoolingCapacityCoefficient2(2);
  double testSensibleCoolingCapacityCoefficient2 = coilCoolingWaterToAirHeatPumpEquationFit.sensibleCoolingCapacityCoefficient2();
  coilCoolingWaterToAirHeatPumpEquationFit.setSensibleCoolingCapacityCoefficient3(3);
  double testSensibleCoolingCapacityCoefficient3 = coilCoolingWaterToAirHeatPumpEquationFit.sensibleCoolingCapacityCoefficient3();
  coilCoolingWaterToAirHeatPumpEquationFit.setSensibleCoolingCapacityCoefficient4(4);
  double testSensibleCoolingCapacityCoefficient4 = coilCoolingWaterToAirHeatPumpEquationFit.sensibleCoolingCapacityCoefficient4();
  coilCoolingWaterToAirHeatPumpEquationFit.setSensibleCoolingCapacityCoefficient5(5);
  double testSensibleCoolingCapacityCoefficient5 = coilCoolingWaterToAirHeatPumpEquationFit.sensibleCoolingCapacityCoefficient5();
  coilCoolingWaterToAirHeatPumpEquationFit.setSensibleCoolingCapacityCoefficient6(6);
  double testSensibleCoolingCapacityCoefficient6 = coilCoolingWaterToAirHeatPumpEquationFit.sensibleCoolingCapacityCoefficient6();

  EXPECT_EQ(testSensibleCoolingCapacityCoefficient1,1);
  EXPECT_EQ(testSensibleCoolingCapacityCoefficient2,2);
  EXPECT_EQ(testSensibleCoolingCapacityCoefficient3,3);
  EXPECT_EQ(testSensibleCoolingCapacityCoefficient4,4);
  EXPECT_EQ(testSensibleCoolingCapacityCoefficient5,5);
  EXPECT_EQ(testSensibleCoolingCapacityCoefficient6,6);

  // Fields N17 - N21 Check set Cooling Power Consumption Coefficients
  coilCoolingWaterToAirHeatPumpEquationFit.setCoolingPowerConsumptionCoefficient1(1);
  double testCoolingPowerConsumptionCoefficient1 = coilCoolingWaterToAirHeatPumpEquationFit.coolingPowerConsumptionCoefficient1();
  coilCoolingWaterToAirHeatPumpEquationFit.setCoolingPowerConsumptionCoefficient2(2);
  double testCoolingPowerConsumptionCoefficient2 = coilCoolingWaterToAirHeatPumpEquationFit.coolingPowerConsumptionCoefficient2();
  coilCoolingWaterToAirHeatPumpEquationFit.setCoolingPowerConsumptionCoefficient3(3);
  double testCoolingPowerConsumptionCoefficient3 = coilCoolingWaterToAirHeatPumpEquationFit.coolingPowerConsumptionCoefficient3();
  coilCoolingWaterToAirHeatPumpEquationFit.setCoolingPowerConsumptionCoefficient4(4);
  double testCoolingPowerConsumptionCoefficient4 = coilCoolingWaterToAirHeatPumpEquationFit.coolingPowerConsumptionCoefficient4();
  coilCoolingWaterToAirHeatPumpEquationFit.setCoolingPowerConsumptionCoefficient5(5);
  double testCoolingPowerConsumptionCoefficient5 = coilCoolingWaterToAirHeatPumpEquationFit.coolingPowerConsumptionCoefficient5();

  EXPECT_EQ(testCoolingPowerConsumptionCoefficient1,1);
  EXPECT_EQ(testCoolingPowerConsumptionCoefficient2,2);
  EXPECT_EQ(testCoolingPowerConsumptionCoefficient3,3);
  EXPECT_EQ(testCoolingPowerConsumptionCoefficient4,4);
  EXPECT_EQ(testCoolingPowerConsumptionCoefficient5,5);

  // Field N22 Nominal Time for Condensate Removal to Begin. Test set and get Nominal Time for Condensate Removal to Begin, and test default and reset functions

  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.setNominalTimeforCondensateRemovaltoBegin(100));
  boost::optional<double> testNominalTimeforCondensateRemovaltoBegin = coilCoolingWaterToAirHeatPumpEquationFit.nominalTimeforCondensateRemovaltoBegin();

  EXPECT_EQ((*testNominalTimeforCondensateRemovaltoBegin),100);
  EXPECT_FALSE(coilCoolingWaterToAirHeatPumpEquationFit.isNominalTimeforCondensateRemovaltoBeginDefaulted());

  coilCoolingWaterToAirHeatPumpEquationFit.resetNominalTimeforCondensateRemovaltoBegin();
  EXPECT_FALSE(coilCoolingWaterToAirHeatPumpEquationFit.nominalTimeforCondensateRemovaltoBegin());
  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.isNominalTimeforCondensateRemovaltoBeginDefaulted());

  // Field N23 Ratio of Initial Moisture Evaporation Rate and Steady State Latent Capacity. Test set and get Ratio of Initial Moisture Evaporation Rate and Steady State Latent Capacity, and test default and reset functions

  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(0.5));
  boost::optional<double> testRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity = coilCoolingWaterToAirHeatPumpEquationFit.ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity();

  EXPECT_EQ((*testRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity),0.5);
  EXPECT_FALSE(coilCoolingWaterToAirHeatPumpEquationFit.isRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacityDefaulted());

  coilCoolingWaterToAirHeatPumpEquationFit.resetRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity();
  EXPECT_FALSE(coilCoolingWaterToAirHeatPumpEquationFit.ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity());
  EXPECT_TRUE(coilCoolingWaterToAirHeatPumpEquationFit.isRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacityDefaulted());
}
