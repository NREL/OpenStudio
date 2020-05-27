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
#include "../../model/CoilCoolingDXCurveFitSpeed_Impl.hpp"

#include <utilities/idd/Coil_Cooling_DX_CurveFit_Speed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDXCurveFitSpeed) {
  Model m;
  CoilCoolingDXCurveFitSpeed speed(m);

  ForwardTranslator ft;
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
  ASSERT_EQ(3u, idfCurveBiquadratics.size());

  WorkspaceObjectVector idfCurveQuadratics(w.getObjectsByType(IddObjectType::Curve_Quadratic));
  ASSERT_EQ(3u, idfCurveQuadratics.size());
}
