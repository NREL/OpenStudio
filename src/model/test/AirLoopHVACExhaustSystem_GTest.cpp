/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../AirLoopHVACExhaustSystem.hpp"
#include "../AirLoopHVACExhaustSystem_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../ZoneMixers.hpp"
#include "../ZoneMixers_Impl.hpp"

#include "../FansSystemModel.hpp"
#include "../FansSystemModel_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirLoopHVACExhaustSystem_GettersSetters) {
  Model m;
  // TODO: Check regular Ctor arguments
  AirLoopHVACExhaustSystem airLoopHVACExhaustSystem(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // AirLoopHVACExhaustSystem airLoopHVACExhaustSystem = m.getUniqueModelObject<AirLoopHVACExhaustSystem>();

  airLoopHVACExhaustSystem.setName("My AirLoopHVACExhaustSystem");

  // Zone Mixer Name: Required Object
  ZoneMixers zoneMixer(m);
  EXPECT_TRUE(airLoopHVACExhaustSystem.setZoneMixer(zoneMixer));
  EXPECT_EQ(zoneMixer, airLoopHVACExhaustSystem.zoneMixer());

  // Fan Name: Required Object
  FansSystemModel fan(m);
  EXPECT_TRUE(airLoopHVACExhaustSystem.setFan(fan));
  EXPECT_EQ(fan, airLoopHVACExhaustSystem.fan());
}
TEST_F(ModelFixture, AirLoopHVACExhaustSystem_HeatCoolFuelTypes) {
  Model m;
  // TODO: Check regular Ctor arguments
  AirLoopHVACExhaustSystem airLoopHVACExhaustSystem(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // AirLoopHVACExhaustSystem airLoopHVACExhaustSystem = m.getUniqueModelObject<AirLoopHVACExhaustSystem>();

  EXPECT_EQ(ComponentType(ComponentType::Both), airLoopHVACExhaustSystem.componentType());
  testFuelTypeEquality({FuelType::Electricity}, airLoopHVACExhaustSystem.coolingFuelTypes());
  testFuelTypeEquality({FuelType::Electricity, FuelType::Propane}, airLoopHVACExhaustSystem.heatingFuelTypes());
  testAppGFuelTypeEquality({AppGFuelType::Fuel, AppGFuelType::HeatPump}, airLoopHVACExhaustSystem.appGHeatingFuelTypes());
}
