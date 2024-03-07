/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/ZoneHVACPackagedTerminalHeatPump.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/CoilHeatingDXSingleSpeed.hpp"
#include "../../model/CoilHeatingDXVariableSpeed.hpp"
#include "../../model/CoilCoolingDX.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance.hpp"
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

  // Need to be in a thermal zone to be translated, with at least one space
  ThermalZone z(m);
  pthp.addToThermalZone(z);
  Space s(m);
  s.setThermalZone(z);

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
  EXPECT_EQ("Yes", idf_pthp.getString(ZoneHVAC_PackagedTerminalHeatPumpFields::NoLoadSupplyAirFlowRateControlSetToLowSpeed).get());

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

  // Need to be in a thermal zone to be translated, with at least one space
  ThermalZone z(m);
  pthp.addToThermalZone(z);
  Space s(m);
  s.setThermalZone(z);

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

  // Need to be in a thermal zone to be translated, with at least one space
  ThermalZone z(m);
  pthp.addToThermalZone(z);
  Space s(m);
  s.setThermalZone(z);

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

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneHVACPackagedTerminalHeatPump_CoilCoolingDX) {

  Model m;

  Schedule sch = m.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(m, sch);
  CoilHeatingDXVariableSpeed hc(m);
  CoilCoolingDXCurveFitOperatingMode operatingMode(m);
  CoilCoolingDXCurveFitPerformance performance(m, operatingMode);
  CoilCoolingDX cc(m, performance);
  CoilHeatingElectric shc(m);

  ZoneHVACPackagedTerminalHeatPump pthp(m, sch, fan, hc, cc, shc);

  // Need to be in a thermal zone to be translated, with at least one space
  ThermalZone z(m);
  pthp.addToThermalZone(z);
  Space s(m);
  s.setThermalZone(z);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(m);

  WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::ZoneHVAC_PackagedTerminalHeatPump));
  ASSERT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_pthp(idfObjs[0]);

  EXPECT_EQ("Coil:Heating:DX:VariableSpeed", idf_pthp.getString(ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingCoilObjectType).get());
  EXPECT_EQ(hc.nameString(), idf_pthp.getString(ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingCoilName).get());

  // Check that the DX coil ends up directly onto the object, and NOT a CoilSystem:Cooling:DX wrapper|
  EXPECT_EQ("Coil:Cooling:DX", idf_pthp.getString(ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingCoilObjectType).get());
  EXPECT_EQ(cc.nameString(), idf_pthp.getString(ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingCoilName).get());

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::CoilSystem_Cooling_DX).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX).size());
}
