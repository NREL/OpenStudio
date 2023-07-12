/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "../../model/Node.hpp"

#include "../../model/CoilCoolingDXMultiSpeed.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/CoilHeatingGasMultiStage.hpp"
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/CoilHeatingElectricMultiStage.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/AirLoopHVAC_UnitarySystem_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Gas_MultiStage_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_MultiStage_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_CoilHeatingGasMultiStage) {
  Model m;

  CoilCoolingDXMultiSpeed c(m);
  CoilHeatingGasMultiStage h(m);
  FanConstantVolume f(m);
  CoilHeatingElectric s(m);

  AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed unitary(m, f, h, c, s);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(m);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Heating_Gas_MultiStage).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_CoilHeatingElectricMultiStage) {
  Model m;

  CoilCoolingDXMultiSpeed c(m);
  CoilHeatingElectricMultiStage h(m);
  FanConstantVolume f(m);
  CoilHeatingElectric s(m);

  AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed unitary(m, f, h, c, s);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(m);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Heating_Electric_MultiStage).size());
}
