/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/Schedule.hpp"

#include "../../model/EvaporativeCoolerDirectResearchSpecial.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/EvaporativeCooler_Direct_ResearchSpecial_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture,ForwardTranslator_EvaporativeCoolerDirectResearchSpecial_LimitFields)
{
  Model m;

  Schedule sch =  m.alwaysOnDiscreteSchedule();
  EvaporativeCoolerDirectResearchSpecial e(m, sch);

  EXPECT_TRUE(e.setPrimaryAirDesignFlowRate(1.05));
  EXPECT_TRUE(e.setEvaporativeOperationMinimumDrybulbTemperature(16));
  EXPECT_TRUE(e.setEvaporativeOperationMaximumLimitWetbulbTemperature(24));
  EXPECT_TRUE(e.setEvaporativeOperationMaximumLimitDrybulbTemperature(28));

  ASSERT_TRUE(e.primaryAirDesignFlowRate());
  EXPECT_DOUBLE_EQ(1.05, e.primaryAirDesignFlowRate().get());
  EXPECT_DOUBLE_EQ(16, e.evaporativeOperationMinimumDrybulbTemperature());
  EXPECT_DOUBLE_EQ(24, e.evaporativeOperationMaximumLimitWetbulbTemperature());
  EXPECT_DOUBLE_EQ(28, e.evaporativeOperationMaximumLimitDrybulbTemperature());

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::EvaporativeCooler_Direct_ResearchSpecial);
  ASSERT_EQ(1u, idfObjs.size());

  WorkspaceObject idfObj(idfObjs[0]);

  EXPECT_DOUBLE_EQ(e.primaryAirDesignFlowRate().get(),
            idfObj.getDouble(EvaporativeCooler_Direct_ResearchSpecialFields::PrimaryAirDesignFlowRate).get());
  EXPECT_DOUBLE_EQ(e.evaporativeOperationMinimumDrybulbTemperature(),
            idfObj.getDouble(EvaporativeCooler_Direct_ResearchSpecialFields::EvaporativeOperationMinimumDrybulbTemperature).get());
  EXPECT_DOUBLE_EQ(e.evaporativeOperationMaximumLimitWetbulbTemperature(),
            idfObj.getDouble(EvaporativeCooler_Direct_ResearchSpecialFields::EvaporativeOperationMaximumLimitWetbulbTemperature).get());
  EXPECT_DOUBLE_EQ(e.evaporativeOperationMaximumLimitDrybulbTemperature(),
            idfObj.getDouble(EvaporativeCooler_Direct_ResearchSpecialFields::EvaporativeOperationMaximumLimitDrybulbTemperature).get());

}


