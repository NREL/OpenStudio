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
#include "../CoilCoolingCooledBeam.hpp"
#include "../AirTerminalSingleDuctConstantVolumeCooledBeam.hpp"
#include "../AirTerminalSingleDuctConstantVolumeCooledBeam_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,AirTerminalSingleDuctConstantVolumeCooledBeam_Test_Constructor) {
  
  Model model;
  ScheduleConstant schedule(model);
  schedule.setValue(1.0); // Always on
  CoilCoolingCooledBeam coilCoolingCooledBeam(model);
  AirTerminalSingleDuctConstantVolumeCooledBeam cooledBeam(model,schedule,coilCoolingCooledBeam);
  
  // Test constructor
  EXPECT_EQ(openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeam,cooledBeam.iddObjectType().value());
  
  // Test cloning the Cooled Beam
  AirTerminalSingleDuctConstantVolumeCooledBeam cloneBeam = cooledBeam.clone(model).cast<model::AirTerminalSingleDuctConstantVolumeCooledBeam>();
  ASSERT_EQ(cooledBeam.supplyAirVolumetricFlowRate(), cloneBeam.supplyAirVolumetricFlowRate());
  Model model2;
  AirTerminalSingleDuctConstantVolumeCooledBeam cloneBeam2 = cooledBeam.clone(model2).cast<model::AirTerminalSingleDuctConstantVolumeCooledBeam>();
  ASSERT_EQ(cooledBeam.supplyAirVolumetricFlowRate(), cloneBeam2.supplyAirVolumetricFlowRate());
  
  // test set and get availibility schedule
  ScheduleConstant schedule1(model);
  schedule1.setValue(1.0);
  EXPECT_TRUE(cooledBeam.setAvailabilitySchedule(schedule1));
  EXPECT_EQ(cooledBeam.availabilitySchedule(),schedule1);
  
  // test set Cooling Coil
  CoilCoolingCooledBeam coilCoolingCooledBeam1(model);
  cooledBeam.setCoolingCoil(coilCoolingCooledBeam1);
  EXPECT_EQ(cooledBeam.coilCoolingCooledBeam(),coilCoolingCooledBeam1);
  }

TEST_F(ModelFixture,AirTerminalSingleDuctConstantVolumeCooledBeam_addToNode) {
  Model m;
  ScheduleConstant s(m);
  CoilCoolingCooledBeam coil(m);
  AirTerminalSingleDuctConstantVolumeCooledBeam testObject(m,s,coil);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)2, airLoop.supplyComponents().size() );

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_TRUE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.supplyComponents().size() );

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.demandComponents().size() );

  AirTerminalSingleDuctConstantVolumeCooledBeam testObjectClone = testObject.clone(m).cast<AirTerminalSingleDuctConstantVolumeCooledBeam>();
  inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObjectClone.addToNode(inletNode));
  EXPECT_TRUE(airLoop.addBranchForHVACComponent(testObjectClone));
  EXPECT_EQ( (unsigned)10, airLoop.demandComponents().size() );
}
 
 TEST_F(ModelFixture,AirTerminalSingleDuctConstantVolumeCooledBeam_Test_Fields) {
  
  Model model;
  ScheduleConstant schedule(model);
  schedule.setValue(1.0); // Always on
  CoilCoolingCooledBeam coilCoolingCooledBeam(model);
  AirTerminalSingleDuctConstantVolumeCooledBeam cooledBeam(model,schedule,coilCoolingCooledBeam);
  
  // Field A4 Cooled Beam Type
  EXPECT_TRUE(cooledBeam.setCooledBeamType("Active"));
  boost::optional<std::string> beamType = cooledBeam.cooledBeamType();
  EXPECT_EQ(*beamType,"Active");
  
  // Field N1 Supply Air Volumetric Flow Rate
  
  boost::optional<double> testSupplyAirVolumetricFlowRate = cooledBeam.supplyAirVolumetricFlowRate();
  cooledBeam.setSupplyAirVolumetricFlowRate(10);
  EXPECT_FALSE(cooledBeam.isSupplyAirVolumetricFlowRateDefaulted());
  EXPECT_FALSE(cooledBeam.isSupplyAirVolumetricFlowRateAutosized());
  boost::optional<double> testSupplyAirVolumetricFlowRate1 = cooledBeam.supplyAirVolumetricFlowRate();
  EXPECT_EQ(*testSupplyAirVolumetricFlowRate1,10);
  
  cooledBeam.resetSupplyAirVolumetricFlowRate();
  EXPECT_TRUE(cooledBeam.isSupplyAirVolumetricFlowRateDefaulted());
  EXPECT_TRUE(cooledBeam.isSupplyAirVolumetricFlowRateAutosized());
  
  // Field N2 Maximum Total Chilled Water Volumetric Flow Rate
  
  cooledBeam.setMaximumTotalChilledWaterVolumetricFlowRate(10);
  EXPECT_FALSE(cooledBeam.isMaximumTotalChilledWaterVolumetricFlowRateDefaulted());
  EXPECT_FALSE(cooledBeam.isMaximumTotalChilledWaterVolumetricFlowRateAutosized());
  boost::optional<double> testMaximumTotalChilledWaterVolumetricFlowRate = cooledBeam.maximumTotalChilledWaterVolumetricFlowRate();
  EXPECT_EQ(*testMaximumTotalChilledWaterVolumetricFlowRate,10);
  
  cooledBeam.resetMaximumTotalChilledWaterVolumetricFlowRate();
  EXPECT_TRUE(cooledBeam.isMaximumTotalChilledWaterVolumetricFlowRateDefaulted());
  EXPECT_TRUE(cooledBeam.isMaximumTotalChilledWaterVolumetricFlowRateAutosized());
  
  // Field N3 Number of Beams
  
  cooledBeam.setNumberofBeams(10);
  EXPECT_FALSE(cooledBeam.isNumberofBeamsDefaulted());
  EXPECT_FALSE(cooledBeam.isNumberofBeamsAutosized());
  boost::optional<int> testNumberofBeams = cooledBeam.numberofBeams();
  EXPECT_EQ(*testNumberofBeams,10);
  
  cooledBeam.resetNumberofBeams();
  EXPECT_TRUE(cooledBeam.isNumberofBeamsDefaulted());
  EXPECT_TRUE(cooledBeam.isNumberofBeamsAutosized());
  
  // Field N4 Beam Length
  
  cooledBeam.setBeamLength(10);
  EXPECT_FALSE(cooledBeam.isBeamLengthDefaulted());
  EXPECT_FALSE(cooledBeam.isBeamLengthAutosized());
  boost::optional<double> testBeamLength = cooledBeam.beamLength();
  EXPECT_EQ(*testBeamLength,10);
  
  cooledBeam.resetBeamLength();
  EXPECT_TRUE(cooledBeam.isBeamLengthDefaulted());
  EXPECT_TRUE(cooledBeam.isBeamLengthAutosized());

  // Field N5 Design Inlet Water Temperature
  
  cooledBeam.setDesignInletWaterTemperature(10);
  EXPECT_FALSE(cooledBeam.isDesignInletWaterTemperatureDefaulted());
  boost::optional<double> testDesignInletWaterTemperature = cooledBeam.designInletWaterTemperature();
  EXPECT_EQ(*testDesignInletWaterTemperature,10);
  
  cooledBeam.resetDesignInletWaterTemperature();
  EXPECT_TRUE(cooledBeam.isDesignInletWaterTemperatureDefaulted());
  
  // Field N6 Design Outlet Water Temperature
  
  cooledBeam.setDesignOutletWaterTemperature(10);
  EXPECT_FALSE(cooledBeam.isDesignOutletWaterTemperatureDefaulted());
  boost::optional<double> testDesignOutletWaterTemperature = cooledBeam.designOutletWaterTemperature();
  EXPECT_EQ(*testDesignOutletWaterTemperature,10);
  
  cooledBeam.resetDesignOutletWaterTemperature();
  EXPECT_TRUE(cooledBeam.isDesignOutletWaterTemperatureDefaulted());
  
  // Field N7 Coefficient of Induction Kin
  cooledBeam.setCoefficientofInductionKin(3);
  EXPECT_FALSE(cooledBeam.isCoefficientofInductionKinDefaulted());
  EXPECT_FALSE(cooledBeam.isCoefficientofInductionKinAutocalculated());
  boost::optional<double> testCoefficientofInductionKin = cooledBeam.coefficientofInductionKin();
  EXPECT_EQ(*testCoefficientofInductionKin,3);
  
  cooledBeam.resetCoefficientofInductionKin();
  EXPECT_TRUE(cooledBeam.isCoefficientofInductionKinDefaulted());
  EXPECT_TRUE(cooledBeam.isCoefficientofInductionKinAutocalculated());
  
  }
