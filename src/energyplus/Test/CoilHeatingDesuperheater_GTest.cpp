/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/CoilHeatingDesuperheater.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXTwoSpeed.hpp"
#include "../../model/CoilCoolingDXTwoStageWithHumidityControlMode.hpp"
#include "../../model/CoilCoolingDX.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance.hpp"

#include <utilities/idd/IddObject.hpp>
#include <utilities/idd/Coil_Heating_Desuperheater_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilHeatingDesuperheater) {
  Model m;

  CoilHeatingDesuperheater desuperheater(m);

  CoilCoolingDXCurveFitOperatingMode operatingMode(m);
  CoilCoolingDXCurveFitPerformance performance(m, operatingMode);

  std::vector<HVACComponent> testCoils = {CoilCoolingDXSingleSpeed(m), CoilCoolingDXTwoSpeed(m), CoilCoolingDXTwoStageWithHumidityControlMode(m),
                                          CoilCoolingDX(m, performance)};

  ForwardTranslator forwardTranslator;

  for (const auto& dxCoil : testCoils) {

    desuperheater.setHeatingSource(dxCoil);

    Workspace workspace = forwardTranslator.translateModel(m);

    WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::Coil_Heating_Desuperheater));
    ASSERT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_desuperheater(idfObjs[0]);

    std::string ep_idd_name = dxCoil.iddObject().name().substr(3);

    // Check that the DX coil ends up directly onto the object, and NOT a CoilSystem:Cooling:DX wrapper
    EXPECT_EQ(ep_idd_name, idf_desuperheater.getString(Coil_Heating_DesuperheaterFields::HeatingSourceObjectType).get());
    EXPECT_EQ(dxCoil.nameString(), idf_desuperheater.getString(Coil_Heating_DesuperheaterFields::HeatingSourceName).get());
  }
}
