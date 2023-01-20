/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "../CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_GettersSetters) {
  Model m;
  // TODO: Check regular Ctor arguments
  CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl = m.getUniqueModelObject<CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl>();

  coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.setName("My CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl");

  // Availability Schedule: Required Object
  Schedule obj(m);
  EXPECT_TRUE(coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.setAvailabilitySchedule(obj));
  EXPECT_EQ(obj, coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.availabilitySchedule());

  // Coil Air Inlet Node: Optional Object
  boost::optional<Connection> obj(m);
  EXPECT_TRUE(coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.setCoilAirInletNode(obj));
  ASSERT_TRUE(coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.coilAirInletNode());
  EXPECT_EQ(obj, coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.coilAirInletNode().get());

  // Coil Air Outlet Node: Optional Object
  boost::optional<Connection> obj(m);
  EXPECT_TRUE(coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.setCoilAirOutletNode(obj));
  ASSERT_TRUE(coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.coilAirOutletNode());
  EXPECT_EQ(obj, coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.coilAirOutletNode().get());

  // Rated Total Heating Capacity: Required Double
  // Autosize
  coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.autosizeRatedTotalHeatingCapacity();
  EXPECT_TRUE(coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.isRatedTotalHeatingCapacityAutosized());
  // Set
  EXPECT_TRUE(coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.setRatedTotalHeatingCapacity(0.6));
  ASSERT_TRUE(coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.ratedTotalHeatingCapacity());
  EXPECT_EQ(0.6, coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.ratedTotalHeatingCapacity().get());
  // Bad Value
  EXPECT_FALSE(coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.setRatedTotalHeatingCapacity(-10.0));
  ASSERT_TRUE(coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.ratedTotalHeatingCapacity());
  EXPECT_EQ(0.6, coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.ratedTotalHeatingCapacity().get());
  EXPECT_FALSE(coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.isRatedTotalHeatingCapacityAutosized());

  // Indoor Unit Reference Subcooling: Required Double
  EXPECT_TRUE(coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.setIndoorUnitReferenceSubcooling(0.7));
  EXPECT_EQ(0.7, coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.indoorUnitReferenceSubcooling());
  // Bad Value
  EXPECT_FALSE(coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.setIndoorUnitReferenceSubcooling(-10.0));
  EXPECT_EQ(0.7, coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.indoorUnitReferenceSubcooling());

  // Indoor Unit Condensing Temperature Function of Subcooling Curve: Required Object
  UnivariateFunctions obj(m);
  EXPECT_TRUE(coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.setIndoorUnitCondensingTemperatureFunctionofSubcoolingCurve(obj));
  EXPECT_EQ(obj, coilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.indoorUnitCondensingTemperatureFunctionofSubcoolingCurve());
}
