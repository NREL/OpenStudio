/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../FuelFactors.hpp"
#include "../FuelFactors_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, FuelFactors_GettersSetters) {
  Model m;
  // TODO: Check regular Ctor arguments
  FuelFactors fuelFactors(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // FuelFactors fuelFactors = m.getUniqueModelObject<FuelFactors>();

  // Existing Fuel Resource Name: Required String
  EXPECT_TRUE(fuelFactors.setExistingFuelResourceName("Electricity"));
  EXPECT_EQ("Electricity", fuelFactors.existingFuelResourceName());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setExistingFuelResourceName("BADENUM"));
  EXPECT_EQ("Electricity", fuelFactors.existingFuelResourceName());

  // Source Energy Factor: Required Double
  EXPECT_TRUE(fuelFactors.setSourceEnergyFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.sourceEnergyFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setSourceEnergyFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.sourceEnergyFactor());

  // Source Energy Schedule Name: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(fuelFactors.setSourceEnergySchedule(obj));
  ASSERT_TRUE(fuelFactors.sourceEnergySchedule());
  EXPECT_EQ(obj, fuelFactors.sourceEnergySchedule().get());

  // CO2 Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setCO2EmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.cO2EmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setCO2EmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.cO2EmissionFactor());

  // CO2 Emission Factor Schedule Name: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(fuelFactors.setCO2EmissionFactorSchedule(obj));
  ASSERT_TRUE(fuelFactors.cO2EmissionFactorSchedule());
  EXPECT_EQ(obj, fuelFactors.cO2EmissionFactorSchedule().get());

  // CO Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setCOEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.cOEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setCOEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.cOEmissionFactor());

  // CO Emission Factor Schedule Name: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(fuelFactors.setCOEmissionFactorSchedule(obj));
  ASSERT_TRUE(fuelFactors.cOEmissionFactorSchedule());
  EXPECT_EQ(obj, fuelFactors.cOEmissionFactorSchedule().get());

  // CH4 Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setCH4EmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.cH4EmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setCH4EmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.cH4EmissionFactor());

  // CH4 Emission Factor Schedule Name: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(fuelFactors.setCH4EmissionFactorSchedule(obj));
  ASSERT_TRUE(fuelFactors.cH4EmissionFactorSchedule());
  EXPECT_EQ(obj, fuelFactors.cH4EmissionFactorSchedule().get());

  // NOx Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setNOxEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.nOxEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setNOxEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.nOxEmissionFactor());

  // NOx Emission Factor Schedule Name: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(fuelFactors.setNOxEmissionFactorSchedule(obj));
  ASSERT_TRUE(fuelFactors.nOxEmissionFactorSchedule());
  EXPECT_EQ(obj, fuelFactors.nOxEmissionFactorSchedule().get());

  // N1O Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setN1OEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.n1OEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setN1OEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.n1OEmissionFactor());

  // N1O Emission Factor Schedule Name: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(fuelFactors.setN1OEmissionFactorSchedule(obj));
  ASSERT_TRUE(fuelFactors.n1OEmissionFactorSchedule());
  EXPECT_EQ(obj, fuelFactors.n1OEmissionFactorSchedule().get());

  // SO2 Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setSO2EmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.sO2EmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setSO2EmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.sO2EmissionFactor());

  // SO2 Emission Factor Schedule Name: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(fuelFactors.setSO2EmissionFactorSchedule(obj));
  ASSERT_TRUE(fuelFactors.sO2EmissionFactorSchedule());
  EXPECT_EQ(obj, fuelFactors.sO2EmissionFactorSchedule().get());

  // PM Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setPMEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.pMEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setPMEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.pMEmissionFactor());

  // PM Emission Factor Schedule Name: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(fuelFactors.setPMEmissionFactorSchedule(obj));
  ASSERT_TRUE(fuelFactors.pMEmissionFactorSchedule());
  EXPECT_EQ(obj, fuelFactors.pMEmissionFactorSchedule().get());

  // PM10 Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setPM10EmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.pM10EmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setPM10EmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.pM10EmissionFactor());

  // PM10 Emission Factor Schedule Name: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(fuelFactors.setPM10EmissionFactorSchedule(obj));
  ASSERT_TRUE(fuelFactors.pM10EmissionFactorSchedule());
  EXPECT_EQ(obj, fuelFactors.pM10EmissionFactorSchedule().get());

  // PM2.5 Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setPM25EmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.pM25EmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setPM25EmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.pM25EmissionFactor());

  // PM2.5 Emission Factor Schedule Name: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(fuelFactors.setPM25EmissionFactorSchedule(obj));
  ASSERT_TRUE(fuelFactors.pM25EmissionFactorSchedule());
  EXPECT_EQ(obj, fuelFactors.pM25EmissionFactorSchedule().get());

  // NH3 Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setNH3EmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.nH3EmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setNH3EmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.nH3EmissionFactor());

  // NH3 Emission Factor Schedule Name: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(fuelFactors.setNH3EmissionFactorSchedule(obj));
  ASSERT_TRUE(fuelFactors.nH3EmissionFactorSchedule());
  EXPECT_EQ(obj, fuelFactors.nH3EmissionFactorSchedule().get());

  // NMVOC Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setNMVOCEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.nMVOCEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setNMVOCEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.nMVOCEmissionFactor());

  // NMVOC Emission Factor Schedule Name: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(fuelFactors.setNMVOCEmissionFactorSchedule(obj));
  ASSERT_TRUE(fuelFactors.nMVOCEmissionFactorSchedule());
  EXPECT_EQ(obj, fuelFactors.nMVOCEmissionFactorSchedule().get());

  // Hg Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setHgEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.hgEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setHgEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.hgEmissionFactor());

  // Hg Emission Factor Schedule Name: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(fuelFactors.setHgEmissionFactorSchedule(obj));
  ASSERT_TRUE(fuelFactors.hgEmissionFactorSchedule());
  EXPECT_EQ(obj, fuelFactors.hgEmissionFactorSchedule().get());

  // Pb Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setPbEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.pbEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setPbEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.pbEmissionFactor());

  // Pb Emission Factor Schedule Name: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(fuelFactors.setPbEmissionFactorSchedule(obj));
  ASSERT_TRUE(fuelFactors.pbEmissionFactorSchedule());
  EXPECT_EQ(obj, fuelFactors.pbEmissionFactorSchedule().get());

  // Water Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setWaterEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.waterEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setWaterEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.waterEmissionFactor());

  // Water Emission Factor Schedule Name: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(fuelFactors.setWaterEmissionFactorSchedule(obj));
  ASSERT_TRUE(fuelFactors.waterEmissionFactorSchedule());
  EXPECT_EQ(obj, fuelFactors.waterEmissionFactorSchedule().get());

  // Nuclear High Level Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setNuclearHighLevelEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.nuclearHighLevelEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setNuclearHighLevelEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.nuclearHighLevelEmissionFactor());

  // Nuclear High Level Emission Factor Schedule Name: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(fuelFactors.setNuclearHighLevelEmissionFactorSchedule(obj));
  ASSERT_TRUE(fuelFactors.nuclearHighLevelEmissionFactorSchedule());
  EXPECT_EQ(obj, fuelFactors.nuclearHighLevelEmissionFactorSchedule().get());

  // Nuclear Low Level Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setNuclearLowLevelEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.nuclearLowLevelEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setNuclearLowLevelEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.nuclearLowLevelEmissionFactor());

  // Nuclear Low Level Emission Factor Schedule Name: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(fuelFactors.setNuclearLowLevelEmissionFactorSchedule(obj));
  ASSERT_TRUE(fuelFactors.nuclearLowLevelEmissionFactorSchedule());
  EXPECT_EQ(obj, fuelFactors.nuclearLowLevelEmissionFactorSchedule().get());
