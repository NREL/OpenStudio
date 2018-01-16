/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ZoneCapacitanceMultiplierResearchSpecial.hpp"
#include "../ZoneCapacitanceMultiplierResearchSpecial_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,ZoneCapacitanceMultiplierResearchSpecial_TemperatureCapacityMultiplier_Quantity) {
  Model model;
  // TODO: Check constructor.
  ZoneCapacitanceMultiplierResearchSpecial zoneCapacitanceMultiplierResearchSpecial = model.getUniqueModelObject<ZoneCapacitanceMultiplierResearchSpecial>();
  Unit units = zoneCapacitanceMultiplierResearchSpecial.getTemperatureCapacityMultiplier(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneCapacitanceMultiplierResearchSpecial.setTemperatureCapacityMultiplier(testQ));
  Quantity q = zoneCapacitanceMultiplierResearchSpecial.getTemperatureCapacityMultiplier(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneCapacitanceMultiplierResearchSpecial_HumidityCapacityMultiplier_Quantity) {
  Model model;
  // TODO: Check constructor.
  ZoneCapacitanceMultiplierResearchSpecial zoneCapacitanceMultiplierResearchSpecial = model.getUniqueModelObject<ZoneCapacitanceMultiplierResearchSpecial>();

  Unit units = zoneCapacitanceMultiplierResearchSpecial.getHumidityCapacityMultiplier(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneCapacitanceMultiplierResearchSpecial.setHumidityCapacityMultiplier(testQ));
  Quantity q = zoneCapacitanceMultiplierResearchSpecial.getHumidityCapacityMultiplier(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneCapacitanceMultiplierResearchSpecial_CarbonDioxideCapacityMultiplier_Quantity) {
  Model model;
  // TODO: Check constructor.
  ZoneCapacitanceMultiplierResearchSpecial zoneCapacitanceMultiplierResearchSpecial = model.getUniqueModelObject<ZoneCapacitanceMultiplierResearchSpecial>();

  Unit units = zoneCapacitanceMultiplierResearchSpecial.getCarbonDioxideCapacityMultiplier(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneCapacitanceMultiplierResearchSpecial.setCarbonDioxideCapacityMultiplier(testQ));
  Quantity q = zoneCapacitanceMultiplierResearchSpecial.getCarbonDioxideCapacityMultiplier(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

