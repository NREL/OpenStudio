/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/CoilWaterHeatingDesuperheater.hpp"
#include "../../model/CoilCoolingDX.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXVariableSpeed.hpp"
#include "../../model/CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/FanConstantVolume.hpp"

#include "../../model/ScheduleConstant.hpp"

#include <utilities/idd/ZoneHVAC_PackagedTerminalAirConditioner_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneHVACPackagedTerminalAirConditioner) {

  Model m;

  Schedule sch = m.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(m);
  CoilCoolingDXSingleSpeed cc(m);
  CoilHeatingElectric hc(m);

  ZoneHVACPackagedTerminalAirConditioner ptac(m, sch, fan, hc, cc);

  // Need to be in a thermal zone to be translated, and TZ needs at least one space
  ThermalZone z(m);
  ptac.addToThermalZone(z);
  Space s(m);
  s.setThermalZone(z);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(m);

  WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::ZoneHVAC_PackagedTerminalAirConditioner));
  ASSERT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_ptac(idfObjs[0]);

  // Check that the DX coil ends up directly onto the object, and NOT a CoilSystem:Cooling:DX wrapper
  EXPECT_EQ("Coil:Cooling:DX:SingleSpeed", idf_ptac.getString(ZoneHVAC_PackagedTerminalAirConditionerFields::CoolingCoilObjectType).get());
  EXPECT_EQ(cc.nameString(), idf_ptac.getString(ZoneHVAC_PackagedTerminalAirConditionerFields::CoolingCoilName).get());

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::CoilSystem_Cooling_DX).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneHVACPackagedTerminalAirConditioner_CoilCoolingDXVariableSpeed) {

  Model m;

  Schedule sch = m.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(m);
  CoilCoolingDXVariableSpeed cc(m);
  CoilHeatingElectric hc(m);

  ZoneHVACPackagedTerminalAirConditioner ptac(m, sch, fan, hc, cc);

  // Need to be in a thermal zone to be translated, and TZ needs at least one space
  ThermalZone z(m);
  ptac.addToThermalZone(z);
  Space s(m);
  s.setThermalZone(z);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(m);

  WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::ZoneHVAC_PackagedTerminalAirConditioner));
  ASSERT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_ptac(idfObjs[0]);

  // Check that the DX coil ends up directly onto the object, and NOT a CoilSystem:Cooling:DX wrapper
  EXPECT_EQ("Coil:Cooling:DX:VariableSpeed", idf_ptac.getString(ZoneHVAC_PackagedTerminalAirConditionerFields::CoolingCoilObjectType).get());
  EXPECT_EQ(cc.nameString(), idf_ptac.getString(ZoneHVAC_PackagedTerminalAirConditionerFields::CoolingCoilName).get());

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::CoilSystem_Cooling_DX).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneHVACPackagedTerminalAirConditioner_CoilSystemCoolingDXHeatExchangerAssisted) {

  Model m;

  Schedule sch = m.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(m);
  CoilSystemCoolingDXHeatExchangerAssisted cc(m);
  CoilHeatingElectric hc(m);

  ZoneHVACPackagedTerminalAirConditioner ptac(m, sch, fan, hc, cc);

  // Need to be in a thermal zone to be translated, and TZ needs at least one space
  ThermalZone z(m);
  ptac.addToThermalZone(z);
  Space s(m);
  s.setThermalZone(z);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(m);

  WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::ZoneHVAC_PackagedTerminalAirConditioner));
  ASSERT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_ptac(idfObjs[0]);

  // Check that the DX coil ends up directly onto the object, and NOT a CoilSystem:Cooling:DX wrapper|
  EXPECT_EQ("CoilSystem:Cooling:DX:HeatExchangerAssisted",
            idf_ptac.getString(ZoneHVAC_PackagedTerminalAirConditionerFields::CoolingCoilObjectType).get());
  EXPECT_EQ(cc.nameString(), idf_ptac.getString(ZoneHVAC_PackagedTerminalAirConditionerFields::CoolingCoilName).get());

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::CoilSystem_Cooling_DX).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::CoilSystem_Cooling_DX_HeatExchangerAssisted).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneHVACPackagedTerminalAirConditioner_CoilCoolingDX) {

  Model m;

  Schedule sch = m.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(m);
  CoilCoolingDXCurveFitOperatingMode operatingMode(m);
  CoilCoolingDXCurveFitPerformance performance(m, operatingMode);
  CoilCoolingDX cc(m, performance);
  CoilHeatingElectric hc(m);

  ZoneHVACPackagedTerminalAirConditioner ptac(m, sch, fan, hc, cc);

  // Need to be in a thermal zone to be translated, and TZ needs at least one space
  ThermalZone z(m);
  ptac.addToThermalZone(z);
  Space s(m);
  s.setThermalZone(z);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(m);

  WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::ZoneHVAC_PackagedTerminalAirConditioner));
  ASSERT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_ptac(idfObjs[0]);

  // Check that the DX coil ends up directly onto the object, and NOT a CoilSystem:Cooling:DX wrapper|
  EXPECT_EQ("Coil:Cooling:DX", idf_ptac.getString(ZoneHVAC_PackagedTerminalAirConditionerFields::CoolingCoilObjectType).get());
  EXPECT_EQ(cc.nameString(), idf_ptac.getString(ZoneHVAC_PackagedTerminalAirConditionerFields::CoolingCoilName).get());

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::CoilSystem_Cooling_DX).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX).size());
}
