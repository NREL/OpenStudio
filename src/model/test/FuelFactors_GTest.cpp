/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelFixture.hpp"

#include "../FuelFactors.hpp"

#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, FuelFactors_GettersSetters) {
  Model m;

  FuelFactors fuelFactors(m);

  // Existing Fuel Resource Name: Required String
  EXPECT_TRUE(fuelFactors.setExistingFuelResourceName("NaturalGas"));
  EXPECT_EQ("NaturalGas", fuelFactors.existingFuelResourceName());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setExistingFuelResourceName("BADENUM"));
  EXPECT_EQ("NaturalGas", fuelFactors.existingFuelResourceName());

  // Source Energy Factor: Required Double
  EXPECT_TRUE(fuelFactors.setSourceEnergyFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.sourceEnergyFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setSourceEnergyFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.sourceEnergyFactor());

  // Source Energy Schedule Name: Optional Object
  ScheduleConstant sch1(m);
  EXPECT_TRUE(fuelFactors.setSourceEnergySchedule(sch1));
  ASSERT_TRUE(fuelFactors.sourceEnergySchedule());
  EXPECT_EQ(sch1, fuelFactors.sourceEnergySchedule().get());

  // CO2 Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setCO2EmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.cO2EmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setCO2EmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.cO2EmissionFactor());

  // CO2 Emission Factor Schedule Name: Optional Object
  ScheduleConstant sch2(m);
  EXPECT_TRUE(fuelFactors.setCO2EmissionFactorSchedule(sch2));
  ASSERT_TRUE(fuelFactors.cO2EmissionFactorSchedule());
  EXPECT_EQ(sch2, fuelFactors.cO2EmissionFactorSchedule().get());

  // CO Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setCOEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.cOEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setCOEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.cOEmissionFactor());

  // CO Emission Factor Schedule Name: Optional Object
  ScheduleConstant sch3(m);
  EXPECT_TRUE(fuelFactors.setCOEmissionFactorSchedule(sch3));
  ASSERT_TRUE(fuelFactors.cOEmissionFactorSchedule());
  EXPECT_EQ(sch3, fuelFactors.cOEmissionFactorSchedule().get());

  // CH4 Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setCH4EmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.cH4EmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setCH4EmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.cH4EmissionFactor());

  // CH4 Emission Factor Schedule Name: Optional Object
  ScheduleConstant sch4(m);
  EXPECT_TRUE(fuelFactors.setCH4EmissionFactorSchedule(sch4));
  ASSERT_TRUE(fuelFactors.cH4EmissionFactorSchedule());
  EXPECT_EQ(sch4, fuelFactors.cH4EmissionFactorSchedule().get());

  // NOx Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setNOxEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.nOxEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setNOxEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.nOxEmissionFactor());

  // NOx Emission Factor Schedule Name: Optional Object
  ScheduleConstant sch5(m);
  EXPECT_TRUE(fuelFactors.setNOxEmissionFactorSchedule(sch5));
  ASSERT_TRUE(fuelFactors.nOxEmissionFactorSchedule());
  EXPECT_EQ(sch5, fuelFactors.nOxEmissionFactorSchedule().get());

  // N2O Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setN2OEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.n2OEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setN2OEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.n2OEmissionFactor());

  // N2O Emission Factor Schedule Name: Optional Object
  ScheduleConstant sch6(m);
  EXPECT_TRUE(fuelFactors.setN2OEmissionFactorSchedule(sch6));
  ASSERT_TRUE(fuelFactors.n2OEmissionFactorSchedule());
  EXPECT_EQ(sch6, fuelFactors.n2OEmissionFactorSchedule().get());

  // SO2 Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setSO2EmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.sO2EmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setSO2EmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.sO2EmissionFactor());

  // SO2 Emission Factor Schedule Name: Optional Object
  ScheduleConstant sch7(m);
  EXPECT_TRUE(fuelFactors.setSO2EmissionFactorSchedule(sch7));
  ASSERT_TRUE(fuelFactors.sO2EmissionFactorSchedule());
  EXPECT_EQ(sch7, fuelFactors.sO2EmissionFactorSchedule().get());

  // PM Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setPMEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.pMEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setPMEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.pMEmissionFactor());

  // PM Emission Factor Schedule Name: Optional Object
  ScheduleConstant sch8(m);
  EXPECT_TRUE(fuelFactors.setPMEmissionFactorSchedule(sch8));
  ASSERT_TRUE(fuelFactors.pMEmissionFactorSchedule());
  EXPECT_EQ(sch8, fuelFactors.pMEmissionFactorSchedule().get());

  // PM10 Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setPM10EmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.pM10EmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setPM10EmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.pM10EmissionFactor());

  // PM10 Emission Factor Schedule Name: Optional Object
  ScheduleConstant sch9(m);
  EXPECT_TRUE(fuelFactors.setPM10EmissionFactorSchedule(sch9));
  ASSERT_TRUE(fuelFactors.pM10EmissionFactorSchedule());
  EXPECT_EQ(sch9, fuelFactors.pM10EmissionFactorSchedule().get());

  // PM2.5 Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setPM25EmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.pM25EmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setPM25EmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.pM25EmissionFactor());

  // PM2.5 Emission Factor Schedule Name: Optional Object
  ScheduleConstant sch10(m);
  EXPECT_TRUE(fuelFactors.setPM25EmissionFactorSchedule(sch10));
  ASSERT_TRUE(fuelFactors.pM25EmissionFactorSchedule());
  EXPECT_EQ(sch10, fuelFactors.pM25EmissionFactorSchedule().get());

  // NH3 Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setNH3EmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.nH3EmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setNH3EmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.nH3EmissionFactor());

  // NH3 Emission Factor Schedule Name: Optional Object
  ScheduleConstant sch11(m);
  EXPECT_TRUE(fuelFactors.setNH3EmissionFactorSchedule(sch11));
  ASSERT_TRUE(fuelFactors.nH3EmissionFactorSchedule());
  EXPECT_EQ(sch11, fuelFactors.nH3EmissionFactorSchedule().get());

  // NMVOC Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setNMVOCEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.nMVOCEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setNMVOCEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.nMVOCEmissionFactor());

  // NMVOC Emission Factor Schedule Name: Optional Object
  ScheduleConstant sch12(m);
  EXPECT_TRUE(fuelFactors.setNMVOCEmissionFactorSchedule(sch12));
  ASSERT_TRUE(fuelFactors.nMVOCEmissionFactorSchedule());
  EXPECT_EQ(sch12, fuelFactors.nMVOCEmissionFactorSchedule().get());

  // Hg Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setHgEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.hgEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setHgEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.hgEmissionFactor());

  // Hg Emission Factor Schedule Name: Optional Object
  ScheduleConstant sch13(m);
  EXPECT_TRUE(fuelFactors.setHgEmissionFactorSchedule(sch13));
  ASSERT_TRUE(fuelFactors.hgEmissionFactorSchedule());
  EXPECT_EQ(sch13, fuelFactors.hgEmissionFactorSchedule().get());

  // Pb Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setPbEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.pbEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setPbEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.pbEmissionFactor());

  // Pb Emission Factor Schedule Name: Optional Object
  ScheduleConstant sch14(m);
  EXPECT_TRUE(fuelFactors.setPbEmissionFactorSchedule(sch14));
  ASSERT_TRUE(fuelFactors.pbEmissionFactorSchedule());
  EXPECT_EQ(sch14, fuelFactors.pbEmissionFactorSchedule().get());

  // Water Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setWaterEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.waterEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setWaterEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.waterEmissionFactor());

  // Water Emission Factor Schedule Name: Optional Object
  ScheduleConstant sch15(m);
  EXPECT_TRUE(fuelFactors.setWaterEmissionFactorSchedule(sch15));
  ASSERT_TRUE(fuelFactors.waterEmissionFactorSchedule());
  EXPECT_EQ(sch15, fuelFactors.waterEmissionFactorSchedule().get());

  // Nuclear High Level Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setNuclearHighLevelEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.nuclearHighLevelEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setNuclearHighLevelEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.nuclearHighLevelEmissionFactor());

  // Nuclear High Level Emission Factor Schedule Name: Optional Object
  ScheduleConstant sch16(m);
  EXPECT_TRUE(fuelFactors.setNuclearHighLevelEmissionFactorSchedule(sch16));
  ASSERT_TRUE(fuelFactors.nuclearHighLevelEmissionFactorSchedule());
  EXPECT_EQ(sch16, fuelFactors.nuclearHighLevelEmissionFactorSchedule().get());

  // Nuclear Low Level Emission Factor: Required Double
  EXPECT_TRUE(fuelFactors.setNuclearLowLevelEmissionFactor(0.1));
  EXPECT_EQ(0.1, fuelFactors.nuclearLowLevelEmissionFactor());
  // Bad Value
  EXPECT_FALSE(fuelFactors.setNuclearLowLevelEmissionFactor(-10.0));
  EXPECT_EQ(0.1, fuelFactors.nuclearLowLevelEmissionFactor());

  // Nuclear Low Level Emission Factor Schedule Name: Optional Object
  ScheduleConstant sch17(m);
  EXPECT_TRUE(fuelFactors.setNuclearLowLevelEmissionFactorSchedule(sch17));
  ASSERT_TRUE(fuelFactors.nuclearLowLevelEmissionFactorSchedule());
  EXPECT_EQ(sch17, fuelFactors.nuclearLowLevelEmissionFactorSchedule().get());
}
