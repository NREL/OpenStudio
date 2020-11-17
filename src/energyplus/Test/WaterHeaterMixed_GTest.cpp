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
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/WaterHeaterMixed.hpp"
// #include "../../model/WaterHeaterMixed_Impl.hpp"
#include "../../model/Schedule.hpp"

#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/WaterHeater_Mixed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

// Test for #1675
TEST_F(EnergyPlusFixture, ForwardTranslatorWaterHeaterMixed_Condition) {
  ForwardTranslator ft;
  Model m;

  WaterHeaterMixed wh(m);
  EXPECT_FALSE(wh.peakUseFlowRate());
  EXPECT_FALSE(wh.useFlowRateFractionSchedule());

  // Not on a PlantLoop, and no Peak use Flow rate => Not translated
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::WaterHeater_Mixed).size());
  }

  // Has a peak flow rate, but still missing the Flow Rate schedule
  EXPECT_TRUE(wh.setPeakUseFlowRate(0.1));
  EXPECT_TRUE(wh.peakUseFlowRate());
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::WaterHeater_Mixed).size());
  }

  // Has both required conditions for standalone
  auto alwaysOnContinuous = m.alwaysOnContinuousSchedule();
  EXPECT_TRUE(wh.setUseFlowRateFractionSchedule(alwaysOnContinuous));
  EXPECT_TRUE(wh.useFlowRateFractionSchedule());
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::WaterHeater_Mixed).size());
  }

  // Now it has the schedule, but not the peak use flow rate
  wh.resetPeakUseFlowRate();
  EXPECT_FALSE(wh.peakUseFlowRate());
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::WaterHeater_Mixed).size());
  }

  // Now it has neither
  wh.resetUseFlowRateFractionSchedule();
  EXPECT_FALSE(wh.useFlowRateFractionSchedule());
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::WaterHeater_Mixed).size());
  }

  PlantLoop p(m);
  // Add a supply branch for the wh
  EXPECT_TRUE(p.addSupplyBranchForComponent(wh));
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::WaterHeater_Mixed).size());
  }
}
