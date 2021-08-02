/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/AirTerminalSingleDuctVAVHeatAndCoolReheat.hpp"
#include "../../model/AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/CoilHeatingGas.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/AirLoopHVAC.hpp"

#include <utilities/idd/AirTerminal_SingleDuct_VAV_HeatAndCool_Reheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AirTerminalSingleDuctVAVHeatAndCoolReheat) {
  Model m;

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);

  Schedule sch = m.alwaysOnDiscreteSchedule();
  CoilHeatingGas coil(m, sch);
  AirTerminalSingleDuctVAVHeatAndCoolReheat aterm(m, coil);
  aterm.setMinimumAirFlowTurndownSchedule(sch);
  // TODO

  AirLoopHVAC a(m);
  a.addBranchForZone(z, aterm);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfAirTerms(w.getObjectsByType(IddObjectType::AirTerminal_SingleDuct_VAV_HeatAndCool_Reheat));
  ASSERT_EQ(1u, idfAirTerms.size());
  WorkspaceObject idfAirTerm(idfAirTerms[0]);

  boost::optional<WorkspaceObject> woAvailabilitySchedule(
    idfAirTerm.getTarget(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AvailabilityScheduleName));
  EXPECT_TRUE(woAvailabilitySchedule);
  EXPECT_EQ(woAvailabilitySchedule->iddObject().type(), IddObjectType::Schedule_Constant);
  // TODO
  boost::optional<WorkspaceObject> woTurndownSchedule(
    idfAirTerm.getTarget(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MinimumAirFlowTurndownScheduleName));
  EXPECT_TRUE(woTurndownSchedule);
  EXPECT_EQ(woTurndownSchedule->iddObject().type(), IddObjectType::Schedule_Constant);
}
