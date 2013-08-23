/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/test/ModelFixture.hpp>
#include <model/Model.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/HVACComponent.hpp>
#include <model/AirLoopHVAC.hpp>

#include <model/CoilCoolingCooledBeam.hpp>
#include <model/CoilCoolingCooledBeam_Impl.hpp>
#include <model/AirTerminalSingleDuctConstantVolumeCooledBeam.hpp>
#include <model/AirTerminalSingleDuctConstantVolumeCooledBeam_Impl.hpp>

#include <model/ScheduleConstant.hpp>
#include <model/ScheduleConstant_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>

// TEST
#include <model/AirTerminalSingleDuctVAVReheat.hpp>
#include <model/AirTerminalSingleDuctVAVReheat_Impl.hpp>
#include <model/CoilHeatingWater.hpp>
#include <model/CoilHeatingWater_Impl.hpp>
#include <model/AirLoopHVACZoneSplitter.hpp>
#include <model/AirLoopHVACZoneSplitter_Impl.hpp>
#include <model/AirLoopHVACZoneMixer.hpp>
#include <model/AirLoopHVACZoneMixer_Impl.hpp>
//

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
 
 TEST_F(ModelFixture,AirTerminalSingleDuctConstantVolumeCooledBeam_Test_AddandRemove) {
 
		Model model;
  ScheduleConstant schedule(model);
  schedule.setValue(1.0); // Always on
  CoilCoolingCooledBeam coilCoolingCooledBeam(model);
  CoilHeatingWater coil(model,schedule);
		AirTerminalSingleDuctConstantVolumeCooledBeam cooledBeam(model,schedule,coilCoolingCooledBeam);
		AirTerminalSingleDuctVAVReheat VAVreheat(model,schedule,coil);
		AirLoopHVAC airLoop(model);
		
		// Test addToNode
		//Attach to outlet node
		//Node outletNode = airLoop.supplyOutletNode();
		//cooledBeam.addToNode(outletNode);
		//ASSERT_EQ( (unsigned)3, airLoop.supplyComponents().size() );
		
		//ASSERT_EQ( (unsigned)2, airLoop.supplyComponents().size() );
		//Node outletNode = airLoop.supplyOutletNode();
		//VAVreheat.addToNode(outletNode);
		//ASSERT_EQ( (unsigned)2, airLoop.supplyComponents().size() );
		
		
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
  