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

#include "../CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "../CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_GettersSetters) {
  Model m;
  // TODO: Check regular Ctor arguments
  CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl = m.getUniqueModelObject<CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl>();

  coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.setName("My CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl");

  // Availability Schedule: Required Object
  Schedule obj(m);
  EXPECT_TRUE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.setAvailabilitySchedule(obj));
EXPECT_EQ(obj, coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.availabilitySchedule());

  // Coil Air Inlet Node: Optional Object
  boost::optional<Connection> obj(m);
  EXPECT_TRUE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.setCoilAirInletNode(obj));
  ASSERT_TRUE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.coilAirInletNode());
  EXPECT_EQ(obj, coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.coilAirInletNode().get());

  // Coil Air Outlet Node: Optional Object
  boost::optional<Connection> obj(m);
  EXPECT_TRUE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.setCoilAirOutletNode(obj));
  ASSERT_TRUE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.coilAirOutletNode());
  EXPECT_EQ(obj, coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.coilAirOutletNode().get());

  // Rated Total Cooling Capacity: Required Double
  // Autosize
  coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.autosizeRatedTotalCoolingCapacity();
  EXPECT_TRUE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.isRatedTotalCoolingCapacityAutosized());
  // Set
  EXPECT_TRUE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.setRatedTotalCoolingCapacity(0.6));
  ASSERT_TRUE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.ratedTotalCoolingCapacity());
  EXPECT_EQ(0.6, coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.ratedTotalCoolingCapacity().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.setRatedTotalCoolingCapacity(-10.0));
  ASSERT_TRUE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.ratedTotalCoolingCapacity());
  EXPECT_EQ(0.6, coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.ratedTotalCoolingCapacity().get());
  EXPECT_FALSE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.isRatedTotalCoolingCapacityAutosized());

  // Rated Sensible Heat Ratio: Required Double
  // Autosize
  coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.autosizeRatedSensibleHeatRatio();
  EXPECT_TRUE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.isRatedSensibleHeatRatioAutosized());
  // Set
  EXPECT_TRUE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.setRatedSensibleHeatRatio(0.7));
  ASSERT_TRUE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.ratedSensibleHeatRatio());
  EXPECT_EQ(0.7, coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.ratedSensibleHeatRatio().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.setRatedSensibleHeatRatio(-10.0));
  ASSERT_TRUE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.ratedSensibleHeatRatio());
  EXPECT_EQ(0.7, coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.ratedSensibleHeatRatio().get());
  EXPECT_FALSE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.isRatedSensibleHeatRatioAutosized());

  // Indoor Unit Reference Superheating: Required Double
  EXPECT_TRUE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.setIndoorUnitReferenceSuperheating(0.8));
  EXPECT_EQ(0.8, coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.indoorUnitReferenceSuperheating());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.setIndoorUnitReferenceSuperheating(-10.0));
  EXPECT_EQ(0.8, coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.indoorUnitReferenceSuperheating());

  // Indoor Unit Evaporating Temperature Function of Superheating Curve: Required Object
  UnivariateFunctions obj(m);
  EXPECT_TRUE(coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.setIndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(obj));
EXPECT_EQ(obj, coilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve());

}
