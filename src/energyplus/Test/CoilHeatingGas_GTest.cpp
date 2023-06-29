/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
