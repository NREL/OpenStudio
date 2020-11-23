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
#include "../../model/CoilWaterHeatingDesuperheater.hpp"

// DX
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXTwoSpeed.hpp"
#include "../../model/CoilCoolingDXTwoStageWithHumidityControlMode.hpp"
#include "../../model/CoilCoolingDXVariableSpeed.hpp"
#include "../../model/CoilCoolingDXMultiSpeed.hpp"

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

  // DX Coils that will be wrapped in CoilSystem:Cooling:DX
  std::vector<HVACComponent> testCoils = {CoilCoolingDXSingleSpeed(m), CoilCoolingDXTwoSpeed(m), CoilCoolingDXTwoStageWithHumidityControlMode(m),
                                          CoilCoolingDXVariableSpeed(m), CoilCoolingDXMultiSpeed(m)};

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
