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

#include "../WaterHeaterSizing.hpp"
#include "../WaterHeaterSizing_Impl.hpp"

#include "../Model.hpp"
#include "../WaterHeaterMixed.hpp"
#include "../WaterHeaterMixed_Impl.hpp"
#include "../WaterHeaterStratified.hpp"
#include "../WaterHeaterStratified_Impl.hpp"
#include "../WaterToWaterComponent.hpp"

#include "../ChillerElectricEIR.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, WaterHeaterSizing_GettersSetters) {
  Model m;

  WaterHeaterMixed wh(m);
  WaterHeaterSizing waterHeaterSizing = wh.waterHeaterSizing();

  // Design Mode: Optional String
  EXPECT_TRUE(waterHeaterSizing.setDesignMode("PeakDraw"));
  ASSERT_TRUE(waterHeaterSizing.designMode());
  EXPECT_EQ("PeakDraw", waterHeaterSizing.designMode().get());
  // Bad Value
  EXPECT_FALSE(waterHeaterSizing.setDesignMode("BADENUM"));
  ASSERT_TRUE(waterHeaterSizing.designMode());
  EXPECT_EQ("PeakDraw", waterHeaterSizing.designMode().get());

  // Time Storage Can Meet Peak Draw: Optional Double
  EXPECT_TRUE(waterHeaterSizing.setTimeStorageCanMeetPeakDraw(0.1));
  ASSERT_TRUE(waterHeaterSizing.timeStorageCanMeetPeakDraw());
  EXPECT_EQ(0.1, waterHeaterSizing.timeStorageCanMeetPeakDraw().get());
  // Bad Value
  EXPECT_FALSE(waterHeaterSizing.setTimeStorageCanMeetPeakDraw(-10.0));
  ASSERT_TRUE(waterHeaterSizing.timeStorageCanMeetPeakDraw());
  EXPECT_EQ(0.1, waterHeaterSizing.timeStorageCanMeetPeakDraw().get());

  // Time for Tank Recovery: Optional Double
  EXPECT_TRUE(waterHeaterSizing.setTimeforTankRecovery(0.1));
  ASSERT_TRUE(waterHeaterSizing.timeforTankRecovery());
  EXPECT_EQ(0.1, waterHeaterSizing.timeforTankRecovery().get());
  // Bad Value
  EXPECT_FALSE(waterHeaterSizing.setTimeforTankRecovery(-10.0));
  ASSERT_TRUE(waterHeaterSizing.timeforTankRecovery());
  EXPECT_EQ(0.1, waterHeaterSizing.timeforTankRecovery().get());

  // Nominal Tank Volume for Autosizing Plant Connections: Optional Double
  EXPECT_TRUE(waterHeaterSizing.setNominalTankVolumeforAutosizingPlantConnections(3));
  ASSERT_TRUE(waterHeaterSizing.nominalTankVolumeforAutosizingPlantConnections());
  EXPECT_EQ(3, waterHeaterSizing.nominalTankVolumeforAutosizingPlantConnections().get());

  // Number of Bedrooms: Optional Integer
  EXPECT_TRUE(waterHeaterSizing.setNumberofBedrooms(3));
  ASSERT_TRUE(waterHeaterSizing.numberofBedrooms());
  EXPECT_EQ(3, waterHeaterSizing.numberofBedrooms().get());
  // Bad Value
  EXPECT_FALSE(waterHeaterSizing.setNumberofBedrooms(-9));
  ASSERT_TRUE(waterHeaterSizing.numberofBedrooms());
  EXPECT_EQ(3, waterHeaterSizing.numberofBedrooms().get());

  // Number of Bathrooms: Optional Integer
  EXPECT_TRUE(waterHeaterSizing.setNumberofBathrooms(2));
  ASSERT_TRUE(waterHeaterSizing.numberofBathrooms());
  EXPECT_EQ(2, waterHeaterSizing.numberofBathrooms().get());
  // Bad Value
  EXPECT_FALSE(waterHeaterSizing.setNumberofBathrooms(-9));
  ASSERT_TRUE(waterHeaterSizing.numberofBathrooms());
  EXPECT_EQ(2, waterHeaterSizing.numberofBathrooms().get());

  // Storage Capacity per Person: Optional Double
  EXPECT_TRUE(waterHeaterSizing.setStorageCapacityperPerson(0.1));
  ASSERT_TRUE(waterHeaterSizing.storageCapacityperPerson());
  EXPECT_EQ(0.1, waterHeaterSizing.storageCapacityperPerson().get());
  // Bad Value
  EXPECT_FALSE(waterHeaterSizing.setStorageCapacityperPerson(-10.0));
  ASSERT_TRUE(waterHeaterSizing.storageCapacityperPerson());
  EXPECT_EQ(0.1, waterHeaterSizing.storageCapacityperPerson().get());

  // Recovery Capacity per Person: Optional Double
  EXPECT_TRUE(waterHeaterSizing.setRecoveryCapacityperPerson(0.1));
  ASSERT_TRUE(waterHeaterSizing.recoveryCapacityperPerson());
  EXPECT_EQ(0.1, waterHeaterSizing.recoveryCapacityperPerson().get());
  // Bad Value
  EXPECT_FALSE(waterHeaterSizing.setRecoveryCapacityperPerson(-10.0));
  ASSERT_TRUE(waterHeaterSizing.recoveryCapacityperPerson());
  EXPECT_EQ(0.1, waterHeaterSizing.recoveryCapacityperPerson().get());

  // Storage Capacity per Floor Area: Optional Double
  EXPECT_TRUE(waterHeaterSizing.setStorageCapacityperFloorArea(0.1));
  ASSERT_TRUE(waterHeaterSizing.storageCapacityperFloorArea());
  EXPECT_EQ(0.1, waterHeaterSizing.storageCapacityperFloorArea().get());
  // Bad Value
  EXPECT_FALSE(waterHeaterSizing.setStorageCapacityperFloorArea(-10.0));
  ASSERT_TRUE(waterHeaterSizing.storageCapacityperFloorArea());
  EXPECT_EQ(0.1, waterHeaterSizing.storageCapacityperFloorArea().get());

  // Recovery Capacity per Floor Area: Optional Double
  EXPECT_TRUE(waterHeaterSizing.setRecoveryCapacityperFloorArea(0.1));
  ASSERT_TRUE(waterHeaterSizing.recoveryCapacityperFloorArea());
  EXPECT_EQ(0.1, waterHeaterSizing.recoveryCapacityperFloorArea().get());
  // Bad Value
  EXPECT_FALSE(waterHeaterSizing.setRecoveryCapacityperFloorArea(-10.0));
  ASSERT_TRUE(waterHeaterSizing.recoveryCapacityperFloorArea());
  EXPECT_EQ(0.1, waterHeaterSizing.recoveryCapacityperFloorArea().get());

  // Number of Units: Optional Double
  EXPECT_TRUE(waterHeaterSizing.setNumberofUnits(3));
  ASSERT_TRUE(waterHeaterSizing.numberofUnits());
  EXPECT_EQ(3, waterHeaterSizing.numberofUnits().get());

  // Storage Capacity per Unit: Optional Double
  EXPECT_TRUE(waterHeaterSizing.setStorageCapacityperUnit(0.1));
  ASSERT_TRUE(waterHeaterSizing.storageCapacityperUnit());
  EXPECT_EQ(0.1, waterHeaterSizing.storageCapacityperUnit().get());
  // Bad Value
  EXPECT_FALSE(waterHeaterSizing.setStorageCapacityperUnit(-10.0));
  ASSERT_TRUE(waterHeaterSizing.storageCapacityperUnit());
  EXPECT_EQ(0.1, waterHeaterSizing.storageCapacityperUnit().get());

  // Recovery Capacity PerUnit: Optional Double
  EXPECT_TRUE(waterHeaterSizing.setRecoveryCapacityPerUnit(0.1));
  ASSERT_TRUE(waterHeaterSizing.recoveryCapacityPerUnit());
  EXPECT_EQ(0.1, waterHeaterSizing.recoveryCapacityPerUnit().get());
  // Bad Value
  EXPECT_FALSE(waterHeaterSizing.setRecoveryCapacityPerUnit(-10.0));
  ASSERT_TRUE(waterHeaterSizing.recoveryCapacityPerUnit());
  EXPECT_EQ(0.1, waterHeaterSizing.recoveryCapacityPerUnit().get());

  // Storage Capacity per Collector Area: Optional Double
  EXPECT_TRUE(waterHeaterSizing.setStorageCapacityperCollectorArea(0.1));
  ASSERT_TRUE(waterHeaterSizing.storageCapacityperCollectorArea());
  EXPECT_EQ(0.1, waterHeaterSizing.storageCapacityperCollectorArea().get());
  // Bad Value
  EXPECT_FALSE(waterHeaterSizing.setStorageCapacityperCollectorArea(-10.0));
  ASSERT_TRUE(waterHeaterSizing.storageCapacityperCollectorArea());
  EXPECT_EQ(0.1, waterHeaterSizing.storageCapacityperCollectorArea().get());

  // Height Aspect Ratio: Optional Double
  EXPECT_TRUE(waterHeaterSizing.setHeightAspectRatio(0.1));
  ASSERT_TRUE(waterHeaterSizing.heightAspectRatio());
  EXPECT_EQ(0.1, waterHeaterSizing.heightAspectRatio().get());
  // Bad Value
  EXPECT_FALSE(waterHeaterSizing.setHeightAspectRatio(-10.0));
  ASSERT_TRUE(waterHeaterSizing.heightAspectRatio());
  EXPECT_EQ(0.1, waterHeaterSizing.heightAspectRatio().get());
}

TEST_F(ModelFixture, WaterHeaterSizing_Mixed) {

  Model m;
  WaterHeaterMixed wh(m);
  WaterHeaterSizing waterHeaterSizing = wh.waterHeaterSizing();
  EXPECT_EQ(wh, waterHeaterSizing.waterHeater());

  EXPECT_EQ(1, m.getConcreteModelObjects<WaterHeaterSizing>().size());
  auto whClone = wh.clone(m).cast<WaterHeaterMixed>();
  EXPECT_EQ(2, m.getConcreteModelObjects<WaterHeaterSizing>().size());
  EXPECT_NE(wh.waterHeaterSizing(), whClone.waterHeaterSizing());

  whClone.remove();
  EXPECT_EQ(1, m.getConcreteModelObjects<WaterHeaterSizing>().size());
  waterHeaterSizing = wh.waterHeaterSizing();
  EXPECT_EQ(wh, waterHeaterSizing.waterHeater());
}

TEST_F(ModelFixture, WaterHeaterSizing_Stratified) {

  Model m;
  WaterHeaterStratified wh(m);
  WaterHeaterSizing waterHeaterSizing = wh.waterHeaterSizing();
  EXPECT_EQ(wh, waterHeaterSizing.waterHeater());

  EXPECT_EQ(1, m.getConcreteModelObjects<WaterHeaterSizing>().size());
  auto whClone = wh.clone(m).cast<WaterHeaterStratified>();
  EXPECT_EQ(2, m.getConcreteModelObjects<WaterHeaterSizing>().size());
  EXPECT_NE(wh.waterHeaterSizing(), whClone.waterHeaterSizing());

  whClone.remove();
  EXPECT_EQ(1, m.getConcreteModelObjects<WaterHeaterSizing>().size());
  waterHeaterSizing = wh.waterHeaterSizing();
  EXPECT_EQ(wh, waterHeaterSizing.waterHeater());
}
