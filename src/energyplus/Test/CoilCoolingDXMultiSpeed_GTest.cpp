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
// #include "../ReverseTranslator.hpp"

#include "../../model/CoilCoolingDXMultiSpeed.hpp"
#include "../../model/CoilCoolingDXMultiSpeedStageData.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/UnitarySystemPerformanceMultispeed.hpp"

#include "../../model/Node.hpp"
#include "../../model/Curve.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Coil_Cooling_DX_MultiSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>
#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

/* It should allow for up to 4 stages. Tests that issue #2812 is fixed and doesn't come back */
TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDXMultiSpeed_4Stages) {

  Model m;
  CoilCoolingDXMultiSpeed coil(m);

  CoilCoolingDXMultiSpeedStageData stage1(m);
  coil.addStage(stage1);

  CoilCoolingDXMultiSpeedStageData stage2(m);
  coil.addStage(stage2);

  CoilCoolingDXMultiSpeedStageData stage3(m);
  coil.addStage(stage3);

  CoilCoolingDXMultiSpeedStageData stage4(m);
  coil.addStage(stage4);

  ASSERT_EQ(4u, coil.stages().size());

  // The coil can only be wrapped in E+ in AirLoopHVAC:UnitaryHeatPump:AirToAir:Multispeed
  // In OpenStudio, you can use UnitarySystem, the FT does the job
  AirLoopHVACUnitarySystem unitary(m);
  unitary.setCoolingCoil(coil);

  UnitarySystemPerformanceMultispeed perf(m);
  unitary.setDesignSpecificationMultispeedObject(perf);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  //translate the model to EnergyPlus
  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(m);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_MultiSpeed).size());
  WorkspaceObject idf_coil = workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_MultiSpeed)[0];

  // Check that there are indeed 4 stages, both the NumberofSpeeds field (int) and the number of extensible groups
  ASSERT_EQ(4u, idf_coil.getInt(Coil_Cooling_DX_MultiSpeedFields::NumberofSpeeds).get());
  ASSERT_EQ(4u, idf_coil.extensibleGroups().size());

  // WorkspaceExtensibleGroup eg = idf_coil.extensibleGroups()[0] or getExtensibleGroup(0);

  ASSERT_EQ(idf_coil.getExtensibleGroup(0)
              .getString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofTemperatureCurveName)
              .get(),
            stage1.totalCoolingCapacityFunctionofTemperatureCurve().name().get());

  ASSERT_EQ(idf_coil.getExtensibleGroup(1)
              .getString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofTemperatureCurveName)
              .get(),
            stage2.totalCoolingCapacityFunctionofTemperatureCurve().name().get());

  ASSERT_EQ(idf_coil.getExtensibleGroup(2)
              .getString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofTemperatureCurveName)
              .get(),
            stage3.totalCoolingCapacityFunctionofTemperatureCurve().name().get());

  ASSERT_EQ(idf_coil.getExtensibleGroup(3)
              .getString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofTemperatureCurveName)
              .get(),
            stage4.totalCoolingCapacityFunctionofTemperatureCurve().name().get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDXMultiSpeed_MinOATCompressor) {
  Model m;

  CoilCoolingDXMultiSpeed coil(m);
  coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-7.5);

  // Won't be translated unless assigned
  AirLoopHVACUnitarySystem unitary(m);
  unitary.setCoolingCoil(coil);

  UnitarySystemPerformanceMultispeed perf(m);
  unitary.setDesignSpecificationMultispeedObject(perf);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idf_coils(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_MultiSpeed));
  ASSERT_EQ(1u, idf_coils.size());
  WorkspaceObject idf_coil(idf_coils[0]);

  auto _d = idf_coil.getDouble(Coil_Cooling_DX_MultiSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation);
  ASSERT_TRUE(_d);
  EXPECT_DOUBLE_EQ(-7.5, _d.get());
}
