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
#include "../../model/CoilCoolingDXCurveFitSpeed.hpp"

#include "../../model/CoilCoolingDXCurveFitPerformance.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode.hpp"
#include "../../model/CoilCoolingDX.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/Node.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveQuadratic.hpp"

#include <utilities/idd/Coil_Cooling_DX_CurveFit_Speed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDXCurveFitSpeed) {
  Model m;
  CoilCoolingDXCurveFitSpeed speed(m);

  CoilCoolingDXCurveFitOperatingMode operatingMode(m);
  CoilCoolingDXCurveFitPerformance performance(m, operatingMode);
  CoilCoolingDX dx(m, performance);

  // put it inside a Unitary, and put Unitary on an AirLoopHVAC so it gets translated
  AirLoopHVACUnitarySystem unitary(m);
  unitary.setCoolingCoil(dx);
  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  EXPECT_TRUE(operatingMode.addSpeed(speed));

  ForwardTranslator ft;
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfSpeeds(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_CurveFit_Speed));
    ASSERT_EQ(1u, idfSpeeds.size());
    WorkspaceObject idfSpeed(idfSpeeds[0]);

    EXPECT_EQ(1.0, idfSpeed.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::GrossTotalCoolingCapacityFraction, false).get());
    EXPECT_EQ(1.0, idfSpeed.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::EvaporatorAirFlowRateFraction, false).get());
    EXPECT_EQ(1.0, idfSpeed.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::CondenserAirFlowRateFraction, false).get());
    EXPECT_EQ("Autosize", idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::GrossSensibleHeatRatio, false).get());
    EXPECT_EQ(3.0, idfSpeed.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::GrossCoolingCOP, false).get());
    EXPECT_EQ(1.0, idfSpeed.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::ActiveFractionofCoilFaceArea, false).get());
    EXPECT_EQ(773.3, idfSpeed.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate, false).get());
    EXPECT_EQ(1.0, idfSpeed.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::EvaporativeCondenserPumpPowerFraction, false).get());
    EXPECT_EQ(0.9, idfSpeed.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::EvaporativeCondenserEffectiveness, false).get());
    EXPECT_EQ(0.2, idfSpeed.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::RatedWasteHeatFractionofPowerInput, false).get());

    WorkspaceObjectVector idfCurveBiquadratics(w.getObjectsByType(IddObjectType::Curve_Biquadratic));
    ASSERT_EQ(0u, idfCurveBiquadratics.size());

    WorkspaceObjectVector idfCurveQuadratics(w.getObjectsByType(IddObjectType::Curve_Quadratic));
    ASSERT_EQ(0u, idfCurveQuadratics.size());
  }

  CurveBiquadratic totalCoolingCapacityModifierFunctionofTemperatureCurve(m);
  totalCoolingCapacityModifierFunctionofTemperatureCurve.setCoefficient1Constant(0.766956);
  totalCoolingCapacityModifierFunctionofTemperatureCurve.setCoefficient2x(0.0107756);
  totalCoolingCapacityModifierFunctionofTemperatureCurve.setCoefficient3xPOW2(-0.0000414703);
  totalCoolingCapacityModifierFunctionofTemperatureCurve.setCoefficient4y(0.00134961);
  totalCoolingCapacityModifierFunctionofTemperatureCurve.setCoefficient5yPOW2(-0.000261144);
  totalCoolingCapacityModifierFunctionofTemperatureCurve.setCoefficient6xTIMESY(0.000457488);
  totalCoolingCapacityModifierFunctionofTemperatureCurve.setMinimumValueofx(17.0);
  totalCoolingCapacityModifierFunctionofTemperatureCurve.setMaximumValueofx(22.0);
  totalCoolingCapacityModifierFunctionofTemperatureCurve.setMinimumValueofy(13.0);
  totalCoolingCapacityModifierFunctionofTemperatureCurve.setMaximumValueofy(46.0);
  EXPECT_TRUE(speed.setTotalCoolingCapacityModifierFunctionofTemperatureCurve(totalCoolingCapacityModifierFunctionofTemperatureCurve));
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfSpeeds(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_CurveFit_Speed));
    ASSERT_EQ(1u, idfSpeeds.size());
    WorkspaceObject idfSpeed(idfSpeeds[0]);

    auto _totModOfTempCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofTemperatureCurveName, false, true);
    ASSERT_TRUE(_totModOfTempCurve);
    EXPECT_EQ(totalCoolingCapacityModifierFunctionofTemperatureCurve.nameString(), _totModOfTempCurve.get());

    WorkspaceObjectVector idfCurveBiquadratics(w.getObjectsByType(IddObjectType::Curve_Biquadratic));
    ASSERT_EQ(1u, idfCurveBiquadratics.size());

    WorkspaceObjectVector idfCurveQuadratics(w.getObjectsByType(IddObjectType::Curve_Quadratic));
    ASSERT_EQ(0u, idfCurveQuadratics.size());
  }

  CurveQuadratic totalCoolingCapacityModifierFunctionofAirFlowFractionCurve(m);
  totalCoolingCapacityModifierFunctionofAirFlowFractionCurve.setCoefficient1Constant(0.8);
  totalCoolingCapacityModifierFunctionofAirFlowFractionCurve.setCoefficient2x(0.2);
  totalCoolingCapacityModifierFunctionofAirFlowFractionCurve.setCoefficient3xPOW2(0.0);
  totalCoolingCapacityModifierFunctionofAirFlowFractionCurve.setMinimumValueofx(0.5);
  totalCoolingCapacityModifierFunctionofAirFlowFractionCurve.setMaximumValueofx(1.5);
  EXPECT_TRUE(speed.setTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve(totalCoolingCapacityModifierFunctionofAirFlowFractionCurve));
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfSpeeds(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_CurveFit_Speed));
    ASSERT_EQ(1u, idfSpeeds.size());
    WorkspaceObject idfSpeed(idfSpeeds[0]);

    WorkspaceObjectVector idfCurveBiquadratics(w.getObjectsByType(IddObjectType::Curve_Biquadratic));
    ASSERT_EQ(1u, idfCurveBiquadratics.size());

    WorkspaceObjectVector idfCurveQuadratics(w.getObjectsByType(IddObjectType::Curve_Quadratic));
    ASSERT_EQ(1u, idfCurveQuadratics.size());

    auto _totModOfTempCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofTemperatureCurveName, false, true);
    ASSERT_TRUE(_totModOfTempCurve);
    EXPECT_EQ(totalCoolingCapacityModifierFunctionofTemperatureCurve.nameString(), _totModOfTempCurve.get());

    auto _totModOfFlowCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofAirFlowFractionCurveName, false, true);
    ASSERT_TRUE(_totModOfFlowCurve);
    EXPECT_EQ(totalCoolingCapacityModifierFunctionofAirFlowFractionCurve.nameString(), _totModOfFlowCurve.get());
  }

  CurveBiquadratic energyInputRatioModifierFunctionofTemperatureCurve(m);
  energyInputRatioModifierFunctionofTemperatureCurve.setCoefficient1Constant(0.297145);
  energyInputRatioModifierFunctionofTemperatureCurve.setCoefficient2x(0.0430933);
  energyInputRatioModifierFunctionofTemperatureCurve.setCoefficient3xPOW2(-0.000748766);
  energyInputRatioModifierFunctionofTemperatureCurve.setCoefficient4y(0.00597727);
  energyInputRatioModifierFunctionofTemperatureCurve.setCoefficient5yPOW2(0.000482112);
  energyInputRatioModifierFunctionofTemperatureCurve.setCoefficient6xTIMESY(-0.000956448);
  energyInputRatioModifierFunctionofTemperatureCurve.setMinimumValueofx(17.0);
  energyInputRatioModifierFunctionofTemperatureCurve.setMaximumValueofx(22.0);
  energyInputRatioModifierFunctionofTemperatureCurve.setMinimumValueofy(13.0);
  energyInputRatioModifierFunctionofTemperatureCurve.setMaximumValueofy(46.0);
  EXPECT_TRUE(speed.setEnergyInputRatioModifierFunctionofTemperatureCurve(energyInputRatioModifierFunctionofTemperatureCurve));
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfSpeeds(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_CurveFit_Speed));
    ASSERT_EQ(1u, idfSpeeds.size());
    WorkspaceObject idfSpeed(idfSpeeds[0]);

    WorkspaceObjectVector idfCurveBiquadratics(w.getObjectsByType(IddObjectType::Curve_Biquadratic));
    ASSERT_EQ(2u, idfCurveBiquadratics.size());

    WorkspaceObjectVector idfCurveQuadratics(w.getObjectsByType(IddObjectType::Curve_Quadratic));
    ASSERT_EQ(1u, idfCurveQuadratics.size());

    auto _totModOfTempCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofTemperatureCurveName, false, true);
    ASSERT_TRUE(_totModOfTempCurve);
    EXPECT_EQ(totalCoolingCapacityModifierFunctionofTemperatureCurve.nameString(), _totModOfTempCurve.get());

    auto _totModOfFlowCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofAirFlowFractionCurveName, false, true);
    ASSERT_TRUE(_totModOfFlowCurve);
    EXPECT_EQ(totalCoolingCapacityModifierFunctionofAirFlowFractionCurve.nameString(), _totModOfFlowCurve.get());

    auto _eirFTempCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofTemperatureCurveName, false, true);
    ASSERT_TRUE(_eirFTempCurve);
    EXPECT_EQ(energyInputRatioModifierFunctionofTemperatureCurve.nameString(), _eirFTempCurve.get());
  }

  CurveQuadratic energyInputRatioModifierFunctionofAirFlowFractionCurve(m);
  energyInputRatioModifierFunctionofAirFlowFractionCurve.setCoefficient1Constant(1.156);
  energyInputRatioModifierFunctionofAirFlowFractionCurve.setCoefficient2x(-0.1816);
  energyInputRatioModifierFunctionofAirFlowFractionCurve.setCoefficient3xPOW2(0.0256);
  energyInputRatioModifierFunctionofAirFlowFractionCurve.setMinimumValueofx(0.5);
  energyInputRatioModifierFunctionofAirFlowFractionCurve.setMaximumValueofx(1.5);
  EXPECT_TRUE(speed.setEnergyInputRatioModifierFunctionofAirFlowFractionCurve(energyInputRatioModifierFunctionofAirFlowFractionCurve));
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfSpeeds(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_CurveFit_Speed));
    ASSERT_EQ(1u, idfSpeeds.size());
    WorkspaceObject idfSpeed(idfSpeeds[0]);

    WorkspaceObjectVector idfCurveBiquadratics(w.getObjectsByType(IddObjectType::Curve_Biquadratic));
    ASSERT_EQ(2u, idfCurveBiquadratics.size());

    WorkspaceObjectVector idfCurveQuadratics(w.getObjectsByType(IddObjectType::Curve_Quadratic));
    ASSERT_EQ(2u, idfCurveQuadratics.size());

    auto _totModOfTempCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofTemperatureCurveName, false, true);
    ASSERT_TRUE(_totModOfTempCurve);
    EXPECT_EQ(totalCoolingCapacityModifierFunctionofTemperatureCurve.nameString(), _totModOfTempCurve.get());

    auto _totModOfFlowCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofAirFlowFractionCurveName, false, true);
    ASSERT_TRUE(_totModOfFlowCurve);
    EXPECT_EQ(totalCoolingCapacityModifierFunctionofAirFlowFractionCurve.nameString(), _totModOfFlowCurve.get());

    auto _eirFTempCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofTemperatureCurveName, false, true);
    ASSERT_TRUE(_eirFTempCurve);
    EXPECT_EQ(energyInputRatioModifierFunctionofTemperatureCurve.nameString(), _eirFTempCurve.get());

    auto _eirFFlowCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofAirFlowFractionCurveName, false, true);
    ASSERT_TRUE(_eirFFlowCurve);
    EXPECT_EQ(energyInputRatioModifierFunctionofAirFlowFractionCurve.nameString(), _eirFFlowCurve.get());
  }

  CurveQuadratic partLoadFractionCorrelationCurve(m);
  partLoadFractionCorrelationCurve.setCoefficient1Constant(0.75);
  partLoadFractionCorrelationCurve.setCoefficient2x(0.25);
  partLoadFractionCorrelationCurve.setCoefficient3xPOW2(0.0);
  partLoadFractionCorrelationCurve.setMinimumValueofx(0.0);
  partLoadFractionCorrelationCurve.setMaximumValueofx(1.0);
  EXPECT_TRUE(speed.setPartLoadFractionCorrelationCurve(partLoadFractionCorrelationCurve));
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfSpeeds(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_CurveFit_Speed));
    ASSERT_EQ(1u, idfSpeeds.size());
    WorkspaceObject idfSpeed(idfSpeeds[0]);

    WorkspaceObjectVector idfCurveBiquadratics(w.getObjectsByType(IddObjectType::Curve_Biquadratic));
    ASSERT_EQ(2u, idfCurveBiquadratics.size());

    WorkspaceObjectVector idfCurveQuadratics(w.getObjectsByType(IddObjectType::Curve_Quadratic));
    ASSERT_EQ(3u, idfCurveQuadratics.size());

    auto _totModOfTempCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofTemperatureCurveName, false, true);
    ASSERT_TRUE(_totModOfTempCurve);
    EXPECT_EQ(totalCoolingCapacityModifierFunctionofTemperatureCurve.nameString(), _totModOfTempCurve.get());

    auto _totModOfFlowCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofAirFlowFractionCurveName, false, true);
    ASSERT_TRUE(_totModOfFlowCurve);
    EXPECT_EQ(totalCoolingCapacityModifierFunctionofAirFlowFractionCurve.nameString(), _totModOfFlowCurve.get());

    auto _eirFTempCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofTemperatureCurveName, false, true);
    ASSERT_TRUE(_eirFTempCurve);
    EXPECT_EQ(energyInputRatioModifierFunctionofTemperatureCurve.nameString(), _eirFTempCurve.get());

    auto _eirFFlowCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofAirFlowFractionCurveName, false, true);
    ASSERT_TRUE(_eirFFlowCurve);
    EXPECT_EQ(energyInputRatioModifierFunctionofAirFlowFractionCurve.nameString(), _eirFFlowCurve.get());

    auto _plrFracCurve = idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::PartLoadFractionCorrelationCurveName, false, true);
    ASSERT_TRUE(_plrFracCurve);
    EXPECT_EQ(partLoadFractionCorrelationCurve.nameString(), _plrFracCurve.get());
  }

  CurveBiquadratic wasteHeatModifierFunctionofTemperatureCurve(m);
  wasteHeatModifierFunctionofTemperatureCurve.setCoefficient1Constant(1);
  wasteHeatModifierFunctionofTemperatureCurve.setCoefficient2x(0.0);
  wasteHeatModifierFunctionofTemperatureCurve.setCoefficient3xPOW2(0.0);
  wasteHeatModifierFunctionofTemperatureCurve.setCoefficient4y(0.0);
  wasteHeatModifierFunctionofTemperatureCurve.setCoefficient5yPOW2(0.0);
  wasteHeatModifierFunctionofTemperatureCurve.setCoefficient6xTIMESY(0.0);
  wasteHeatModifierFunctionofTemperatureCurve.setMinimumValueofx(0.0);
  wasteHeatModifierFunctionofTemperatureCurve.setMaximumValueofx(0.0);
  wasteHeatModifierFunctionofTemperatureCurve.setMinimumValueofy(0.0);
  wasteHeatModifierFunctionofTemperatureCurve.setMaximumValueofy(0.0);
  EXPECT_TRUE(speed.setWasteHeatModifierFunctionofTemperatureCurve(wasteHeatModifierFunctionofTemperatureCurve));
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfSpeeds(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_CurveFit_Speed));
    ASSERT_EQ(1u, idfSpeeds.size());
    WorkspaceObject idfSpeed(idfSpeeds[0]);

    WorkspaceObjectVector idfCurveBiquadratics(w.getObjectsByType(IddObjectType::Curve_Biquadratic));
    ASSERT_EQ(3u, idfCurveBiquadratics.size());

    WorkspaceObjectVector idfCurveQuadratics(w.getObjectsByType(IddObjectType::Curve_Quadratic));
    ASSERT_EQ(3u, idfCurveQuadratics.size());

    auto _totModOfTempCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofTemperatureCurveName, false, true);
    ASSERT_TRUE(_totModOfTempCurve);
    EXPECT_EQ(totalCoolingCapacityModifierFunctionofTemperatureCurve.nameString(), _totModOfTempCurve.get());

    auto _totModOfFlowCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofAirFlowFractionCurveName, false, true);
    ASSERT_TRUE(_totModOfFlowCurve);
    EXPECT_EQ(totalCoolingCapacityModifierFunctionofAirFlowFractionCurve.nameString(), _totModOfFlowCurve.get());

    auto _eirFTempCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofTemperatureCurveName, false, true);
    ASSERT_TRUE(_eirFTempCurve);
    EXPECT_EQ(energyInputRatioModifierFunctionofTemperatureCurve.nameString(), _eirFTempCurve.get());

    auto _eirFFlowCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofAirFlowFractionCurveName, false, true);
    ASSERT_TRUE(_eirFFlowCurve);
    EXPECT_EQ(energyInputRatioModifierFunctionofAirFlowFractionCurve.nameString(), _eirFFlowCurve.get());

    auto _plrFracCurve = idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::PartLoadFractionCorrelationCurveName, false, true);
    ASSERT_TRUE(_plrFracCurve);
    EXPECT_EQ(partLoadFractionCorrelationCurve.nameString(), _plrFracCurve.get());

    auto _wasteHeatCurve = idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::WasteHeatModifierFunctionofTemperatureCurveName, false, true);
    ASSERT_TRUE(_wasteHeatCurve);
    EXPECT_EQ(wasteHeatModifierFunctionofTemperatureCurve.nameString(), _wasteHeatCurve.get());
  }

  CurveBiquadratic sensibleHeatRatioModifierFunctionofTemperatureCurve(m);
  EXPECT_TRUE(speed.setSensibleHeatRatioModifierFunctionofTemperatureCurve(sensibleHeatRatioModifierFunctionofTemperatureCurve));
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfSpeeds(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_CurveFit_Speed));
    ASSERT_EQ(1u, idfSpeeds.size());
    WorkspaceObject idfSpeed(idfSpeeds[0]);

    WorkspaceObjectVector idfCurveBiquadratics(w.getObjectsByType(IddObjectType::Curve_Biquadratic));
    ASSERT_EQ(4u, idfCurveBiquadratics.size());

    WorkspaceObjectVector idfCurveQuadratics(w.getObjectsByType(IddObjectType::Curve_Quadratic));
    ASSERT_EQ(3u, idfCurveQuadratics.size());

    auto _totModOfTempCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofTemperatureCurveName, false, true);
    ASSERT_TRUE(_totModOfTempCurve);
    EXPECT_EQ(totalCoolingCapacityModifierFunctionofTemperatureCurve.nameString(), _totModOfTempCurve.get());

    auto _totModOfFlowCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofAirFlowFractionCurveName, false, true);
    ASSERT_TRUE(_totModOfFlowCurve);
    EXPECT_EQ(totalCoolingCapacityModifierFunctionofAirFlowFractionCurve.nameString(), _totModOfFlowCurve.get());

    auto _eirFTempCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofTemperatureCurveName, false, true);
    ASSERT_TRUE(_eirFTempCurve);
    EXPECT_EQ(energyInputRatioModifierFunctionofTemperatureCurve.nameString(), _eirFTempCurve.get());

    auto _eirFFlowCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofAirFlowFractionCurveName, false, true);
    ASSERT_TRUE(_eirFFlowCurve);
    EXPECT_EQ(energyInputRatioModifierFunctionofAirFlowFractionCurve.nameString(), _eirFFlowCurve.get());

    auto _plrFracCurve = idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::PartLoadFractionCorrelationCurveName, false, true);
    ASSERT_TRUE(_plrFracCurve);
    EXPECT_EQ(partLoadFractionCorrelationCurve.nameString(), _plrFracCurve.get());

    auto _wasteHeatCurve = idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::WasteHeatModifierFunctionofTemperatureCurveName, false, true);
    ASSERT_TRUE(_wasteHeatCurve);
    EXPECT_EQ(wasteHeatModifierFunctionofTemperatureCurve.nameString(), _wasteHeatCurve.get());

    auto _shrFTempCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::SensibleHeatRatioModifierFunctionofTemperatureCurveName, false, true);
    ASSERT_TRUE(_shrFTempCurve);
    EXPECT_EQ(sensibleHeatRatioModifierFunctionofTemperatureCurve.nameString(), _shrFTempCurve.get());
  }

  CurveQuadratic sensibleHeatRatioModifierFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(speed.setSensibleHeatRatioModifierFunctionofFlowFractionCurve(sensibleHeatRatioModifierFunctionofFlowFractionCurve));
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfSpeeds(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_CurveFit_Speed));
    ASSERT_EQ(1u, idfSpeeds.size());
    WorkspaceObject idfSpeed(idfSpeeds[0]);

    WorkspaceObjectVector idfCurveBiquadratics(w.getObjectsByType(IddObjectType::Curve_Biquadratic));
    ASSERT_EQ(4u, idfCurveBiquadratics.size());

    WorkspaceObjectVector idfCurveQuadratics(w.getObjectsByType(IddObjectType::Curve_Quadratic));
    ASSERT_EQ(4u, idfCurveQuadratics.size());

    auto _totModOfTempCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofTemperatureCurveName, false, true);
    ASSERT_TRUE(_totModOfTempCurve);
    EXPECT_EQ(totalCoolingCapacityModifierFunctionofTemperatureCurve.nameString(), _totModOfTempCurve.get());

    auto _totModOfFlowCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofAirFlowFractionCurveName, false, true);
    ASSERT_TRUE(_totModOfFlowCurve);
    EXPECT_EQ(totalCoolingCapacityModifierFunctionofAirFlowFractionCurve.nameString(), _totModOfFlowCurve.get());

    auto _eirFTempCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofTemperatureCurveName, false, true);
    ASSERT_TRUE(_eirFTempCurve);
    EXPECT_EQ(energyInputRatioModifierFunctionofTemperatureCurve.nameString(), _eirFTempCurve.get());

    auto _eirFFlowCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofAirFlowFractionCurveName, false, true);
    ASSERT_TRUE(_eirFFlowCurve);
    EXPECT_EQ(energyInputRatioModifierFunctionofAirFlowFractionCurve.nameString(), _eirFFlowCurve.get());

    auto _plrFracCurve = idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::PartLoadFractionCorrelationCurveName, false, true);
    ASSERT_TRUE(_plrFracCurve);
    EXPECT_EQ(partLoadFractionCorrelationCurve.nameString(), _plrFracCurve.get());

    auto _wasteHeatCurve = idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::WasteHeatModifierFunctionofTemperatureCurveName, false, true);
    ASSERT_TRUE(_wasteHeatCurve);
    EXPECT_EQ(wasteHeatModifierFunctionofTemperatureCurve.nameString(), _wasteHeatCurve.get());

    auto _shrFTempCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::SensibleHeatRatioModifierFunctionofTemperatureCurveName, false, true);
    ASSERT_TRUE(_shrFTempCurve);
    EXPECT_EQ(sensibleHeatRatioModifierFunctionofTemperatureCurve.nameString(), _shrFTempCurve.get());

    auto _shrFFlowCurve =
      idfSpeed.getString(Coil_Cooling_DX_CurveFit_SpeedFields::SensibleHeatRatioModifierFunctionofFlowFractionCurveName, false, true);
    ASSERT_TRUE(_shrFFlowCurve);
    EXPECT_EQ(sensibleHeatRatioModifierFunctionofFlowFractionCurve.nameString(), _shrFFlowCurve.get());
  }
}
