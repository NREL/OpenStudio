/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include <gtest/gtest.h>

#include <model/test/ModelFixture.hpp>

#include <model/CoilHeatingGas.hpp>
#include <model/CoilHeatingGas_Impl.hpp>
#include <model/ScheduleConstant.hpp>
#include <utilities/units/Quantity.hpp>
#include <utilities/units/Unit.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,CoilHeatingGas) {
  Model model;
  ScheduleConstant scheduleConstant(model);

  CoilHeatingGas coilHeatingGas(model, scheduleConstant);

  coilHeatingGas.setGasBurnerEfficiency(0.6);
  EXPECT_EQ(coilHeatingGas.gasBurnerEfficiency(),0.6);

  coilHeatingGas.setNominalCapacity(1535.0);
  EXPECT_EQ(coilHeatingGas.nominalCapacity(),1535.0);

  coilHeatingGas.setParasiticElectricLoad(48.0);
  EXPECT_EQ(coilHeatingGas.parasiticElectricLoad(),48.0);

  coilHeatingGas.setParasiticGasLoad(51.0);
  EXPECT_EQ(coilHeatingGas.parasiticGasLoad(),51.0);

  ScheduleConstant schedule2(model);
  coilHeatingGas.setAvailableSchedule(schedule2);
  EXPECT_EQ(coilHeatingGas.availableSchedule(),schedule2);
}

