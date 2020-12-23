/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/CoilChillerAirSourceVariableSpeed.hpp"
#include "../../model/CoilChillerAirSourceVariableSpeed_Impl.hpp"
#include "../../model/CoilChillerAirSourceVariableSpeedSpeedData.hpp"
#include "../../model/CoilSystemIntegratedHeatPumpAirSource.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/Node.hpp"
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/CoilHeatingElectric.hpp"

#include <utilities/idd/Coil_Chiller_AirSource_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilChillerAirSourceVariableSpeed) {
  Model m;

  CoilChillerAirSourceVariableSpeed chillingCoil(m);

  CurveQuadratic curve(m);
  ScheduleConstant schedule(m);
  CoilChillerAirSourceVariableSpeedSpeedData speed(m);

  chillingCoil.setPartLoadFractionCorrelationCurve(curve);
  chillingCoil.setGridSignalSchedule(schedule);
  chillingCoil.addSpeed(speed);

  CoilSystemIntegratedHeatPumpAirSource coilSystem(m);
  coilSystem.setChillingCoil(chillingCoil);

  Schedule s = m.alwaysOnDiscreteSchedule();
  FanConstantVolume supplyFan(m, s);
  CoilHeatingElectric coilHeatingElectric(m, s);

  AirLoopHVACUnitaryHeatPumpAirToAir coil(m, s, supplyFan, coilSystem, coilSystem, coilHeatingElectric);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_TRUE(coil.addToNode(supplyOutletNode));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::CoilSystem_IntegratedHeatPump_AirSource).size());
  EXPECT_EQ(4u, w.getObjectsByType(IddObjectType::Curve_Quadratic).size());
  EXPECT_EQ(2u, w.getObjectsByType(IddObjectType::Curve_Biquadratic).size());
  EXPECT_EQ(4u, w.getObjectsByType(IddObjectType::Schedule_Constant).size());

  WorkspaceObjectVector idf_coils(w.getObjectsByType(IddObjectType::Coil_Chiller_AirSource_VariableSpeed));
  EXPECT_EQ(1u, idf_coils.size());
  WorkspaceObject idf_coil(idf_coils[0]);

  EXPECT_EQ(1, idf_coil.getInt(Coil_Chiller_AirSource_VariableSpeedFields::NominalSpeedLevel, false).get());
  EXPECT_EQ("autosize", idf_coil.getString(Coil_Chiller_AirSource_VariableSpeedFields::RatedChilledWaterCapacity, false).get());
  EXPECT_EQ(8.0, idf_coil.getDouble(Coil_Chiller_AirSource_VariableSpeedFields::RatedEvaporatorInletWaterTemperature, false).get());
  EXPECT_EQ(35.0, idf_coil.getDouble(Coil_Chiller_AirSource_VariableSpeedFields::RatedCondenserInletAirTemperature, false).get());
  EXPECT_EQ("autocalculate", idf_coil.getString(Coil_Chiller_AirSource_VariableSpeedFields::RatedEvaporatorWaterFlowRate, false).get());
  EXPECT_EQ("No", idf_coil.getString(Coil_Chiller_AirSource_VariableSpeedFields::EvaporatorPumpPowerIncludedinRatedCOP, false).get());
  EXPECT_EQ("No",
            idf_coil.getString(Coil_Chiller_AirSource_VariableSpeedFields::EvaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP, false).get());
  EXPECT_EQ(0.2, idf_coil.getDouble(Coil_Chiller_AirSource_VariableSpeedFields::FractionofEvaporatorPumpHeattoWater, false).get());
  EXPECT_EQ("", idf_coil.getString(Coil_Chiller_AirSource_VariableSpeedFields::EvaporatorWaterInletNodeName, false).get());
  EXPECT_EQ("", idf_coil.getString(Coil_Chiller_AirSource_VariableSpeedFields::EvaporatorWaterOutletNodeName, false).get());
  EXPECT_EQ(0, idf_coil.getDouble(Coil_Chiller_AirSource_VariableSpeedFields::CrankcaseHeaterCapacity, false).get());
  EXPECT_EQ(10, idf_coil.getDouble(Coil_Chiller_AirSource_VariableSpeedFields::MaximumAmbientTemperatureforCrankcaseHeaterOperation, false).get());
  boost::optional<WorkspaceObject> idf_curve(idf_coil.getTarget(Coil_Chiller_AirSource_VariableSpeedFields::PartLoadFractionCorrelationCurveName));
  EXPECT_TRUE(idf_curve);
  EXPECT_EQ(idf_curve->iddObject().type(), IddObjectType::Curve_Quadratic);
  boost::optional<WorkspaceObject> idf_sch(idf_coil.getTarget(Coil_Chiller_AirSource_VariableSpeedFields::GridSignalScheduleName));
  EXPECT_TRUE(idf_sch);
  EXPECT_EQ(idf_sch->iddObject().type(), IddObjectType::Schedule_Constant);
  EXPECT_EQ(100.0, idf_coil.getDouble(Coil_Chiller_AirSource_VariableSpeedFields::LowerBoundtoApplyGridResponsiveControl, false).get());
  EXPECT_EQ(-100.0, idf_coil.getDouble(Coil_Chiller_AirSource_VariableSpeedFields::UpperBoundtoApplyGridResponsiveControl, false).get());
  EXPECT_EQ(10, idf_coil.getDouble(Coil_Chiller_AirSource_VariableSpeedFields::MaxSpeedLevelDuringGridResponsiveControl, false).get());
  EXPECT_EQ(1, idf_coil.getInt(Coil_Chiller_AirSource_VariableSpeedFields::NumberofSpeeds, false).get());

  EXPECT_EQ(1u, idf_coil.numExtensibleGroups());

  WorkspaceExtensibleGroup w_eg = idf_coil.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  EXPECT_EQ(9555.0, w_eg.getDouble(Coil_Chiller_AirSource_VariableSpeedExtensibleFields::RatedWaterCoolingCapacityatSpeed, false).get());
  EXPECT_EQ(3.2, w_eg.getDouble(Coil_Chiller_AirSource_VariableSpeedExtensibleFields::RatedWaterCoolingCOPatSpeed, false).get());
  EXPECT_EQ(1.58, w_eg.getDouble(Coil_Chiller_AirSource_VariableSpeedExtensibleFields::SpeedReferenceUnitRatedAirFlowRate, false).get());
  EXPECT_EQ(0.00063, w_eg.getDouble(Coil_Chiller_AirSource_VariableSpeedExtensibleFields::SpeedReferenceUnitRatedWaterFlowRate, false).get());
  EXPECT_EQ(
    15.0, w_eg.getDouble(Coil_Chiller_AirSource_VariableSpeedExtensibleFields::SpeedReferenceUnitWaterPumpInputPowerAtRatedConditions, false).get());
  boost::optional<WorkspaceObject> idf_curve1(
    w_eg.getTarget(Coil_Chiller_AirSource_VariableSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofTemperatureCurveName));
  EXPECT_TRUE(idf_curve1);
  EXPECT_EQ(idf_curve1->iddObject().type(), IddObjectType::Curve_Biquadratic);
  boost::optional<WorkspaceObject> idf_curve2(
    w_eg.getTarget(Coil_Chiller_AirSource_VariableSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofWaterFlowFractionCurveName));
  EXPECT_TRUE(idf_curve2);
  EXPECT_EQ(idf_curve2->iddObject().type(), IddObjectType::Curve_Quadratic);
  boost::optional<WorkspaceObject> idf_curve3(
    w_eg.getTarget(Coil_Chiller_AirSource_VariableSpeedExtensibleFields::SpeedEIRFunctionofTemperatureCurveName));
  EXPECT_TRUE(idf_curve3);
  EXPECT_EQ(idf_curve3->iddObject().type(), IddObjectType::Curve_Biquadratic);
  boost::optional<WorkspaceObject> idf_curve4(
    w_eg.getTarget(Coil_Chiller_AirSource_VariableSpeedExtensibleFields::SpeedEIRFunctionofWaterFlowFractionCurveName));
  EXPECT_TRUE(idf_curve4);
  EXPECT_EQ(idf_curve4->iddObject().type(), IddObjectType::Curve_Quadratic);
}
