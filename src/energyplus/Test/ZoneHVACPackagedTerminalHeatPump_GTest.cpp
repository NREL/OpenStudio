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
#include "../../model/ZoneHVACPackagedTerminalHeatPump.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/CoilHeatingDXSingleSpeed.hpp"
#include "../../model/CoilHeatingDXVariableSpeed.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXVariableSpeed.hpp"
#include "../../model/CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/FanConstantVolume.hpp"

#include "../../model/ScheduleConstant.hpp"

#include <utilities/idd/ZoneHVAC_PackagedTerminalHeatPump_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneHVACPackagedTerminalHeatPump) {

  Model m;

  Schedule sch = m.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(m, sch);
  CoilHeatingDXSingleSpeed hc(m);
  CoilCoolingDXSingleSpeed cc(m);
  CoilHeatingElectric shc(m);

  ZoneHVACPackagedTerminalHeatPump pthp(m, sch, fan, hc, cc, shc);

  // Need to be in a thermal zone to be translated
  ThermalZone z(m);
  pthp.addToThermalZone(z);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(m);

  WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::ZoneHVAC_PackagedTerminalHeatPump));
  ASSERT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_pthp(idfObjs[0]);

  EXPECT_EQ("Coil:Heating:DX:SingleSpeed", idf_pthp.getString(ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingCoilObjectType).get());
  EXPECT_EQ(hc.nameString(), idf_pthp.getString(ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingCoilName).get());

  // Check that the DX coil ends up directly onto the object, and NOT a CoilSystem:Cooling:DX wrapper|
  EXPECT_EQ("Coil:Cooling:DX:SingleSpeed", idf_pthp.getString(ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingCoilObjectType).get());
  EXPECT_EQ(cc.nameString(), idf_pthp.getString(ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingCoilName).get());

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::CoilSystem_Cooling_DX).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneHVACPackagedTerminalHeatPump_CoilDXVariableSpeed) {

  Model m;

  Schedule sch = m.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(m, sch);
  CoilHeatingDXVariableSpeed hc(m);
  CoilCoolingDXVariableSpeed cc(m);
  CoilHeatingElectric shc(m);

  ZoneHVACPackagedTerminalHeatPump pthp(m, sch, fan, hc, cc, shc);

  // Need to be in a thermal zone to be translated
  ThermalZone z(m);
  pthp.addToThermalZone(z);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(m);

  WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::ZoneHVAC_PackagedTerminalHeatPump));
  ASSERT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_pthp(idfObjs[0]);

  EXPECT_EQ("Coil:Heating:DX:VariableSpeed", idf_pthp.getString(ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingCoilObjectType).get());
  EXPECT_EQ(hc.nameString(), idf_pthp.getString(ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingCoilName).get());

  // Check that the DX coil ends up directly onto the object, and NOT a CoilSystem:Cooling:DX wrapper|
  EXPECT_EQ("Coil:Cooling:DX:VariableSpeed", idf_pthp.getString(ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingCoilObjectType).get());
  EXPECT_EQ(cc.nameString(), idf_pthp.getString(ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingCoilName).get());

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::CoilSystem_Cooling_DX).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneHVACPackagedTerminalHeatPump_CoilSystemCoolingDXHeatExchangerAssisted) {

  Model m;

  Schedule sch = m.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(m, sch);
  CoilHeatingDXVariableSpeed hc(m);
  CoilSystemCoolingDXHeatExchangerAssisted cc(m);
  CoilHeatingElectric shc(m);

  ZoneHVACPackagedTerminalHeatPump pthp(m, sch, fan, hc, cc, shc);

  // Need to be in a thermal zone to be translated
  ThermalZone z(m);
  pthp.addToThermalZone(z);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(m);

  WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::ZoneHVAC_PackagedTerminalHeatPump));
  ASSERT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_pthp(idfObjs[0]);

  EXPECT_EQ("Coil:Heating:DX:VariableSpeed", idf_pthp.getString(ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingCoilObjectType).get());
  EXPECT_EQ(hc.nameString(), idf_pthp.getString(ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingCoilName).get());

  // Check that the DX coil ends up directly onto the object, and NOT a CoilSystem:Cooling:DX wrapper|
  EXPECT_EQ("CoilSystem:Cooling:DX:HeatExchangerAssisted", idf_pthp.getString(ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingCoilObjectType).get());
  EXPECT_EQ(cc.nameString(), idf_pthp.getString(ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingCoilName).get());

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::CoilSystem_Cooling_DX).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::CoilSystem_Cooling_DX_HeatExchangerAssisted).size());
}
