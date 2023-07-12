/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/CoilCoolingDXVariableSpeed.hpp"
#include "../../model/CoilCoolingDXVariableSpeed_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/Node.hpp"

#include <utilities/idd/Coil_Cooling_DX_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDXVariableSpeed_MinOATCompressor) {
  Model m;

  CoilCoolingDXVariableSpeed coil(m);
  coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-7.5);

  // Need to be used to be translated
  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  coil.addToNode(supplyOutletNode);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idf_coils(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableSpeed));
  ASSERT_EQ(1u, idf_coils.size());
  WorkspaceObject idf_coil(idf_coils[0]);

  auto _d = idf_coil.getDouble(Coil_Cooling_DX_VariableSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation);
  ASSERT_TRUE(_d);
  EXPECT_DOUBLE_EQ(-7.5, _d.get());
}
