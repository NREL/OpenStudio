/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXSingleSpeed_Impl.hpp"

#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDXSingleSpeed_MinOATCompressor) {
  Model m;

  CoilCoolingDXSingleSpeed coil(m);
  coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-7.5);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  // Note JM 2020-05-13: I find it weird that the coil gets translated when it's not even used by anything, but trying to fix #3976 only here
  WorkspaceObjectVector idf_coils(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_SingleSpeed));
  ASSERT_EQ(1u, idf_coils.size());
  WorkspaceObject idf_coil(idf_coils[0]);

  auto _d = idf_coil.getDouble(Coil_Cooling_DX_SingleSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation);
  ASSERT_TRUE(_d);
  EXPECT_DOUBLE_EQ(-7.5, _d.get());
}
