/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <algorithm>

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../ThermalZone.hpp"
#include "../ZoneHVACBaseboardConvectiveElectric.hpp"

using namespace openstudio;
using namespace openstudio::model;


TEST_F(ModelFixture, ZoneHVACEquipmentList_Logic)
{
  Model m;

  ThermalZone z(m);

  EXPECT_TRUE(z.setLoadDistributionScheme("UniformPLR"));
  EXPECT_EQ(z.loadDistributionScheme(), "UniformPLR");

  ZoneHVACBaseboardConvectiveElectric b1(m);
  EXPECT_TRUE(b1.addToThermalZone(z));
  EXPECT_TRUE(z.setHeatingPriority(b1, 0));
  ZoneHVACBaseboardConvectiveElectric b2(m);
  EXPECT_TRUE(b2.addToThermalZone(z));

  // One that isn't part of the ZoneHVACEquipmentList
  ZoneHVACBaseboardConvectiveElectric bwrong(m);

  EXPECT_EQ(2u, z.equipment().size());
  ASSERT_EQ(2u, z.equipmentInCoolingOrder().size());
  EXPECT_EQ(b1, z.equipmentInCoolingOrder()[0]);
  EXPECT_EQ(b2, z.equipmentInCoolingOrder()[1]);

  // TODO: expected to fail right now due to https://github.com/NREL/EnergyPlus/issues/7193
  // ASSERT_EQ(1u, z.equipmentInHeatingOrder().size());
  // EXPECT_EQ(b2, z.equipmentInHeatingOrder()[0]);

  // Shouldn't be able to get/set anything for an equipment that isn't part of the ZoneEqList
  EXPECT_FALSE(z.setHeatingPriority(bwrong, 3));
  EXPECT_FALSE(z.setCoolingPriority(bwrong, 3));


  // Can't set/get the Sequential Fraction if:

  // 1. Equipment isn't part of the ZoneEqList
  EXPECT_TRUE(z.setLoadDistributionScheme("SequentialLoad"));
  EXPECT_FALSE(z.setSequentialCoolingFraction(bwrong, 0.5));
  EXPECT_FALSE(z.setSequentialHeatingFraction(bwrong, 0.5));
  EXPECT_FALSE(z.sequentialCoolingFraction(bwrong));
  EXPECT_FALSE(z.sequentialHeatingFraction(bwrong));

  // 2. Load Distribution isn't 'SequentialLoad'
  EXPECT_TRUE(z.setLoadDistributionScheme("UniformPLR"));
  EXPECT_FALSE(z.setSequentialCoolingFraction(b1, 0.5));
  EXPECT_FALSE(z.setSequentialHeatingFraction(b1, 0.5));
  EXPECT_FALSE(z.sequentialCoolingFraction(b1));
  EXPECT_FALSE(z.sequentialHeatingFraction(b1));

  EXPECT_TRUE(z.setLoadDistributionScheme("SequentialLoad"));

  // 3. Has a priority of zero
  // TODO: expected to fail (that is, it'll work) right now due to https://github.com/NREL/EnergyPlus/issues/7193
  // EXPECT_FALSE(z.setSequentialHeatingFraction(b1, 0.5));
  // EXPECT_FALSE(z.sequentialHeatingFraction(b1));

  // Rest should work fine
  EXPECT_TRUE(z.setSequentialCoolingFraction(b1, 0.5));
  ASSERT_TRUE(z.sequentialCoolingFraction(b1));
  EXPECT_EQ(0.5, z.sequentialCoolingFraction(b1).get());

  EXPECT_TRUE(z.setSequentialHeatingFraction(b2, 0.65));
  ASSERT_TRUE(z.sequentialHeatingFraction(b2));
  EXPECT_EQ(0.65, z.sequentialHeatingFraction(b2).get());

  EXPECT_TRUE(z.setSequentialCoolingFraction(b2, 0.7));
  ASSERT_TRUE(z.sequentialCoolingFraction(b2));
  EXPECT_EQ(0.7, z.sequentialCoolingFraction(b2).get());


  // Setting a priority to zero should reset the corresponding Sequential Fraction
  // TODO
  // EXPECT_TRUE(z.setCoolingPriority(b2, 0));
  EXPECT_EQ(2u, z.equipment().size());
  // TODO
  // ASSERT_EQ(1u, z.equipmentInCoolingOrder().size());
  // EXPECT_EQ(b1, z.equipmentInCoolingOrder()[0]);

  // TODO
  // EXPECT_FALSE(z.sequentialCoolingFraction(b2)); // reset
  EXPECT_TRUE(z.sequentialHeatingFraction(b2)); // untouched


  // Setting the Load Distribution Scheme to something else should reset all sequential fractions
  EXPECT_TRUE(z.setLoadDistributionScheme("UniformLoad"));
  EXPECT_FALSE(z.sequentialCoolingFraction(b1));
  EXPECT_FALSE(z.sequentialCoolingFraction(b2));
  EXPECT_FALSE(z.sequentialHeatingFraction(b1));
  EXPECT_FALSE(z.sequentialHeatingFraction(b2));

}
