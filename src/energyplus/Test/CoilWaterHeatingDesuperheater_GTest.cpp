/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/CoilWaterHeatingDesuperheater.hpp"

// DX
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXTwoSpeed.hpp"
#include "../../model/CoilCoolingDXTwoStageWithHumidityControlMode.hpp"
#include "../../model/CoilCoolingDXVariableSpeed.hpp"
#include "../../model/CoilCoolingDXMultiSpeed.hpp"
#include "../../model/CoilCoolingDX.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance.hpp"

// Not DX
#include "../../model/CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../../model/RefrigerationCondenserAirCooled.hpp"
#include "../../model/RefrigerationCondenserEvaporativeCooled.hpp"
#include "../../model/RefrigerationCondenserWaterCooled.hpp"

#include "../../model/ScheduleConstant.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/IddObject.hpp>
#include <utilities/idd/Coil_WaterHeating_Desuperheater_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilWaterHeatingDesuperheater_DXCoils) {
  Model m;

  ScheduleConstant temperatureSetpointSchedule(m);
  CoilWaterHeatingDesuperheater desuperheater(m, temperatureSetpointSchedule);
  desuperheater.setRatedHeatReclaimRecoveryEfficiency(0.25);

  CoilCoolingDXCurveFitOperatingMode operatingMode(m);
  CoilCoolingDXCurveFitPerformance performance(m, operatingMode);

  // DX Coils that will be wrapped in CoilSystem:Cooling:DX
  std::vector<HVACComponent> testCoils = {CoilCoolingDXSingleSpeed(m),   CoilCoolingDXTwoSpeed(m),   CoilCoolingDXTwoStageWithHumidityControlMode(m),
                                          CoilCoolingDXVariableSpeed(m), CoilCoolingDXMultiSpeed(m), CoilCoolingDX(m, performance)};

  ForwardTranslator forwardTranslator;

  for (const auto& dxCoil : testCoils) {

    desuperheater.setHeatingSource(dxCoil);

    Workspace workspace = forwardTranslator.translateModel(m);

    WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::Coil_WaterHeating_Desuperheater));
    ASSERT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_desuperheater(idfObjs[0]);

    std::string ep_idd_name = dxCoil.iddObject().name().substr(3);

    // Check that the DX coil ends up directly onto the object, and NOT a CoilSystem:Cooling:DX wrapper
    EXPECT_EQ(ep_idd_name, idf_desuperheater.getString(Coil_WaterHeating_DesuperheaterFields::HeatingSourceObjectType).get());
    EXPECT_EQ(dxCoil.nameString(), idf_desuperheater.getString(Coil_WaterHeating_DesuperheaterFields::HeatingSourceName).get());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilWaterHeatingDesuperheater_NonDX) {

  Model m;

  ScheduleConstant temperatureSetpointSchedule(m);
  CoilWaterHeatingDesuperheater desuperheater(m, temperatureSetpointSchedule);
  desuperheater.setRatedHeatReclaimRecoveryEfficiency(0.25);

  // Refrigeration stuff aren't HVACComponent but ParentObject
  std::vector<ModelObject> testCoils = {CoilCoolingWaterToAirHeatPumpEquationFit(m), RefrigerationCondenserAirCooled(m),
                                        RefrigerationCondenserEvaporativeCooled(m), RefrigerationCondenserWaterCooled(m)};

  ForwardTranslator forwardTranslator;

  for (const auto& nodxCoil : testCoils) {

    EXPECT_TRUE(desuperheater.setHeatingSource(nodxCoil));

    Workspace workspace = forwardTranslator.translateModel(m);

    WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::Coil_WaterHeating_Desuperheater));
    ASSERT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_desuperheater(idfObjs[0]);

    std::string ep_idd_name = nodxCoil.iddObject().name().substr(3);

    // Check that the coil ends up directly onto the object (shouldn't be a problem)
    EXPECT_EQ(ep_idd_name, idf_desuperheater.getString(Coil_WaterHeating_DesuperheaterFields::HeatingSourceObjectType).get());
    EXPECT_EQ(nodxCoil.nameString(), idf_desuperheater.getString(Coil_WaterHeating_DesuperheaterFields::HeatingSourceName).get());
  }
}
