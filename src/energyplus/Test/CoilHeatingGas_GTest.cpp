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
#include "../../model/CoilHeatingGas.hpp"
#include "../../model/CoilHeatingGas_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilHeatingGas) {
  Model model;

  ScheduleConstant scheduleConstant(model);
  CoilHeatingGas coilHeatingGas(model, scheduleConstant);
  coilHeatingGas.setGasBurnerEfficiency(0.6);
  coilHeatingGas.setNominalCapacity(1535.0);
  coilHeatingGas.setParasiticElectricLoad(48.0);
  coilHeatingGas.setParasiticGasLoad(51.0);
  coilHeatingGas.setFuelType("Propane");

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::Coil_Heating_Fuel));
  EXPECT_EQ(1u, idfObjs.size());
  WorkspaceObject coil(idfObjs[0]);
  EXPECT_EQ("Propane", *coil.getString(Coil_Heating_FuelFields::FuelType));
  EXPECT_DOUBLE_EQ(0.6, *coil.getDouble(Coil_Heating_FuelFields::BurnerEfficiency));
  EXPECT_DOUBLE_EQ(1535.0, *coil.getDouble(Coil_Heating_FuelFields::NominalCapacity));
  EXPECT_DOUBLE_EQ(48.0, *coil.getDouble(Coil_Heating_FuelFields::ParasiticElectricLoad));
  EXPECT_DOUBLE_EQ(51.0, *coil.getDouble(Coil_Heating_FuelFields::ParasiticFuelLoad));
}
