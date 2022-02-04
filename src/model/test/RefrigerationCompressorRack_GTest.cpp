/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../RefrigerationCompressorRack.hpp"
#include "../RefrigerationCompressorRack_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../ScheduleCompact_Impl.hpp"
#include "../RefrigerationAirChiller.hpp"
#include "../RefrigerationAirChiller_Impl.hpp"
#include "../RefrigerationCase.hpp"
#include "../RefrigerationCase_Impl.hpp"
#include "../RefrigerationWalkIn.hpp"
#include "../RefrigerationWalkIn_Impl.hpp"
#include "../ModelObjectList.hpp"
#include "../ModelObjectList_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../AirLoopHVACZoneSplitter_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationCompressorRack) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;

      RefrigerationCompressorRack rack(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model model;

  RefrigerationCompressorRack rack(model);

  EXPECT_EQ("Outdoors", rack.heatRejectionLocation());
  EXPECT_EQ(2.0, rack.designCompressorRackCOP());
  boost::optional<Curve> compressorRackCOPFunctionofTemperatureCurve = rack.compressorRackCOPFunctionofTemperatureCurve().optionalCast<Curve>();
  EXPECT_TRUE(compressorRackCOPFunctionofTemperatureCurve);
  EXPECT_EQ(250.0, rack.designCondenserFanPower());
  EXPECT_FALSE(rack.condenserFanPowerFunctionofTemperatureCurve());
  EXPECT_EQ("AirCooled", rack.condenserType());
  EXPECT_EQ("VariableFlow", rack.waterCooledLoopFlowType());
  EXPECT_FALSE(rack.waterCooledCondenserOutletTemperatureSchedule());
  EXPECT_FALSE(rack.waterCooledCondenserDesignFlowRate());
  EXPECT_FALSE(rack.waterCooledCondenserMaximumFlowRate());
  EXPECT_EQ(55.0, rack.waterCooledCondenserMaximumWaterOutletTemperature());
  EXPECT_EQ(10.0, rack.waterCooledCondenserMinimumWaterInletTemperature());
  EXPECT_FALSE(rack.evaporativeCondenserAvailabilitySchedule());
  EXPECT_EQ(0.9, rack.evaporativeCondenserEffectiveness());
  EXPECT_FALSE(rack.evaporativeCondenserAirFlowRate());
  EXPECT_TRUE(rack.isEvaporativeCondenserAirFlowRateAutocalculated());
  EXPECT_EQ(200.0, rack.basinHeaterCapacity());
  EXPECT_EQ(2.0, rack.basinHeaterSetpointTemperature());
  EXPECT_TRUE(rack.designEvaporativeCondenserWaterPumpPower());
  EXPECT_EQ(1000.0, rack.designEvaporativeCondenserWaterPumpPower().get());
  EXPECT_FALSE(rack.isDesignEvaporativeCondenserWaterPumpPowerAutocalculated());
  EXPECT_EQ("General", rack.endUseSubcategory());
  EXPECT_TRUE(rack.isEndUseSubcategoryDefaulted());
  std::vector<RefrigerationCase> cases = rack.cases();
  EXPECT_TRUE(cases.empty());
  std::vector<RefrigerationWalkIn> walkins = rack.walkins();
  EXPECT_TRUE(walkins.empty());
  std::vector<RefrigerationAirChiller> airChillers = rack.airChillers();
  EXPECT_TRUE(airChillers.empty());
  EXPECT_FALSE(rack.heatRejectionZone());
}

TEST_F(ModelFixture, RefrigerationCompressorRack_addToNode) {
  Model model;

  RefrigerationCompressorRack rack = RefrigerationCompressorRack(model);
  AirLoopHVAC airLoop(model);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(rack.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(rack.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(model);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(rack.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_TRUE(rack.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.demandComponents().size());

  auto rackClone = rack.clone(model).cast<RefrigerationCompressorRack>();
  demandOutletNode = plantLoop.demandOutletNode();

  EXPECT_TRUE(rackClone.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)9, plantLoop.demandComponents().size());
}

TEST_F(ModelFixture, RefrigerationCompressorRack_SetGetFields) {
  Model model;

  RefrigerationCompressorRack rack = RefrigerationCompressorRack(model);

  EXPECT_TRUE(rack.setHeatRejectionLocation("Zone"));
  EXPECT_TRUE(rack.setDesignCompressorRackCOP(2.5));
  CurveQuadratic curve1(model);
  EXPECT_TRUE(rack.setCompressorRackCOPFunctionofTemperatureCurve(curve1));
  EXPECT_TRUE(rack.setDesignCondenserFanPower(255.0));
  CurveQuadratic curve2(model);
  EXPECT_TRUE(rack.setCondenserFanPowerFunctionofTemperatureCurve(curve2));
  EXPECT_TRUE(rack.setCondenserType("EvaporativelyCooled"));
  EXPECT_TRUE(rack.setWaterCooledLoopFlowType("ConstantFlow"));
  ScheduleConstant schedule1(model);
  EXPECT_TRUE(rack.setWaterCooledCondenserOutletTemperatureSchedule(schedule1));
  EXPECT_TRUE(rack.setWaterCooledCondenserDesignFlowRate(1.0));
  EXPECT_TRUE(rack.setWaterCooledCondenserMaximumFlowRate(2.0));
  EXPECT_TRUE(rack.setWaterCooledCondenserMaximumWaterOutletTemperature(60.0));
  EXPECT_TRUE(rack.setWaterCooledCondenserMinimumWaterInletTemperature(15.0));
  ScheduleConstant schedule2(model);
  EXPECT_TRUE(rack.setEvaporativeCondenserAvailabilitySchedule(schedule2));
  EXPECT_TRUE(rack.setEvaporativeCondenserEffectiveness(1.0));
  EXPECT_TRUE(rack.setEvaporativeCondenserAirFlowRate(5.0));
  EXPECT_TRUE(rack.setBasinHeaterCapacity(205.0));
  EXPECT_TRUE(rack.setBasinHeaterSetpointTemperature(2.5));
  EXPECT_TRUE(rack.autocalculateDesignEvaporativeCondenserWaterPumpPower());
  EXPECT_TRUE(rack.setEndUseSubcategory("Test"));
  ScheduleCompact s1(model);
  RefrigerationCase case1(model, s1);
  RefrigerationWalkIn walkin1(model, s1);
  RefrigerationAirChiller airChiller1(model, s1);
  EXPECT_TRUE(rack.addCase(case1));
  EXPECT_TRUE(rack.addWalkin(walkin1));
  EXPECT_FALSE(rack.addAirChiller(airChiller1));
  ThermalZone zone1(model);
  EXPECT_TRUE(rack.setHeatRejectionZone(zone1));

  EXPECT_EQ("Zone", rack.heatRejectionLocation());
  EXPECT_EQ(2.5, rack.designCompressorRackCOP());
  boost::optional<Curve> compressorRackCOPFunctionofTemperatureCurve = rack.compressorRackCOPFunctionofTemperatureCurve().optionalCast<Curve>();
  EXPECT_TRUE(compressorRackCOPFunctionofTemperatureCurve);
  EXPECT_EQ(255.0, rack.designCondenserFanPower());
  EXPECT_TRUE(rack.condenserFanPowerFunctionofTemperatureCurve());
  EXPECT_EQ("EvaporativelyCooled", rack.condenserType());
  EXPECT_EQ("ConstantFlow", rack.waterCooledLoopFlowType());
  EXPECT_TRUE(rack.waterCooledCondenserOutletTemperatureSchedule());
  ASSERT_TRUE(rack.waterCooledCondenserDesignFlowRate());
  EXPECT_EQ(1.0, rack.waterCooledCondenserDesignFlowRate().get());
  ASSERT_TRUE(rack.waterCooledCondenserMaximumFlowRate());
  EXPECT_EQ(2.0, rack.waterCooledCondenserMaximumFlowRate().get());
  EXPECT_EQ(60.0, rack.waterCooledCondenserMaximumWaterOutletTemperature());
  EXPECT_EQ(15.0, rack.waterCooledCondenserMinimumWaterInletTemperature());
  EXPECT_TRUE(rack.evaporativeCondenserAvailabilitySchedule());
  EXPECT_EQ(1.0, rack.evaporativeCondenserEffectiveness());
  ASSERT_TRUE(rack.evaporativeCondenserAirFlowRate());
  EXPECT_EQ(5.0, rack.evaporativeCondenserAirFlowRate().get());
  EXPECT_FALSE(rack.isEvaporativeCondenserAirFlowRateAutocalculated());
  EXPECT_EQ(205.0, rack.basinHeaterCapacity());
  EXPECT_EQ(2.5, rack.basinHeaterSetpointTemperature());
  EXPECT_FALSE(rack.designEvaporativeCondenserWaterPumpPower());
  EXPECT_TRUE(rack.isDesignEvaporativeCondenserWaterPumpPowerAutocalculated());
  EXPECT_EQ("Test", rack.endUseSubcategory());
  EXPECT_FALSE(rack.isEndUseSubcategoryDefaulted());
  std::vector<RefrigerationCase> cases = rack.cases();
  EXPECT_FALSE(cases.empty());
  std::vector<RefrigerationWalkIn> walkins = rack.walkins();
  EXPECT_FALSE(walkins.empty());
  std::vector<RefrigerationAirChiller> airChillers = rack.airChillers();
  EXPECT_TRUE(airChillers.empty());
  EXPECT_TRUE(rack.heatRejectionZone());

  rack.resetCondenserFanPowerFunctionofTemperatureCurve();
  rack.resetWaterCooledCondenserOutletTemperatureSchedule();
  rack.resetWaterCooledCondenserDesignFlowRate();
  rack.resetWaterCooledCondenserMaximumFlowRate();
  rack.resetEvaporativeCondenserAvailabilitySchedule();
  rack.autocalculateEvaporativeCondenserAirFlowRate();
  rack.autocalculateDesignEvaporativeCondenserWaterPumpPower();
  rack.resetEndUseSubcategory();
  rack.removeAllCases();
  rack.removeAllWalkins();
  rack.removeAllAirChillers();
  rack.resetHeatRejectionZone();

  EXPECT_FALSE(rack.condenserFanPowerFunctionofTemperatureCurve());
  EXPECT_FALSE(rack.waterCooledCondenserOutletTemperatureSchedule());
  EXPECT_FALSE(rack.waterCooledCondenserDesignFlowRate());
  EXPECT_FALSE(rack.waterCooledCondenserMaximumFlowRate());
  EXPECT_FALSE(rack.evaporativeCondenserAvailabilitySchedule());
  EXPECT_TRUE(rack.isEvaporativeCondenserAirFlowRateAutocalculated());
  EXPECT_TRUE(rack.isDesignEvaporativeCondenserWaterPumpPowerAutocalculated());
  EXPECT_TRUE(rack.isEndUseSubcategoryDefaulted());
  cases = rack.cases();
  EXPECT_TRUE(cases.empty());
  walkins = rack.walkins();
  EXPECT_TRUE(walkins.empty());
  airChillers = rack.airChillers();
  EXPECT_TRUE(airChillers.empty());
  EXPECT_FALSE(rack.heatRejectionZone());
}

TEST_F(ModelFixture, RefrigerationCompressorRack_Clone) {
  Model model;

  RefrigerationCompressorRack rack = RefrigerationCompressorRack(model);
  rack.setDesignCompressorRackCOP(3.0);

  auto rackClone = rack.clone(model).cast<RefrigerationCompressorRack>();
  EXPECT_EQ(3.0, rackClone.designCompressorRackCOP());

  Model model2;
  auto rackClone2 = rack.clone(model2).cast<RefrigerationCompressorRack>();
  EXPECT_EQ(3.0, rackClone2.designCompressorRackCOP());
}

TEST_F(ModelFixture, RefrigerationCompressorRack_Remove) {
  Model model;
  auto size = model.modelObjects().size();
  RefrigerationCompressorRack rack(model);

  std::vector<RefrigerationCompressorRack> refrigerationCompressorRacks = model.getConcreteModelObjects<RefrigerationCompressorRack>();
  EXPECT_EQ(1, refrigerationCompressorRacks.size());

  std::vector<ModelObjectList> refrigerationModelObjectLists = model.getConcreteModelObjects<ModelObjectList>();
  EXPECT_EQ(1, refrigerationModelObjectLists.size());

  std::vector<CurveQuadratic> refrigerationCurveQuadratics = model.getConcreteModelObjects<CurveQuadratic>();
  EXPECT_EQ(1, refrigerationCurveQuadratics.size());

  EXPECT_FALSE(rack.remove().empty());

  refrigerationCompressorRacks = model.getConcreteModelObjects<RefrigerationCompressorRack>();
  EXPECT_EQ(0, refrigerationCompressorRacks.size());

  refrigerationModelObjectLists = model.getConcreteModelObjects<ModelObjectList>();
  EXPECT_EQ(0, refrigerationModelObjectLists.size());

  refrigerationCurveQuadratics = model.getConcreteModelObjects<CurveQuadratic>();
  EXPECT_EQ(0, refrigerationCurveQuadratics.size());

  EXPECT_EQ(size, model.modelObjects().size());
}
