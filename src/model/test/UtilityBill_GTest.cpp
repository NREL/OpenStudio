/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../UtilityBill.hpp"
#include "../YearDescription.hpp"
#include "../YearDescription_Impl.hpp"
#include "../OutputMeter.hpp"
#include "../OutputMeter_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, UtilityBill_Electricity) {
  Model model;

  model::YearDescription yearDescription = model.getUniqueModelObject<model::YearDescription>();
  EXPECT_FALSE(yearDescription.calendarYear());
  yearDescription.setCalendarYear(1999);
  ASSERT_TRUE(yearDescription.calendarYear());
  EXPECT_EQ(1999, yearDescription.calendarYear().get());
  EXPECT_EQ(1999, yearDescription.assumedYear());
  EXPECT_EQ("Friday", yearDescription.dayofWeekforStartDay());
  EXPECT_FALSE(yearDescription.isLeapYear());

  UtilityBill utilityBill(FuelType::Electricity, model);
  EXPECT_EQ(FuelType::Electricity, utilityBill.fuelType().value());
  EXPECT_EQ(InstallLocationType::Facility, utilityBill.meterInstallLocation().value());
  EXPECT_TRUE(utilityBill.isMeterInstallLocationDefaulted());
  EXPECT_FALSE(utilityBill.meterSpecificInstallLocation());
  EXPECT_FALSE(utilityBill.meterEndUseCategory());
  EXPECT_FALSE(utilityBill.meterSpecificEndUse());
  EXPECT_EQ("kWh", utilityBill.consumptionUnit());
  EXPECT_NEAR(3600000, utilityBill.consumptionUnitConversionFactor(), 1);
  ASSERT_TRUE(utilityBill.peakDemandUnit());
  EXPECT_EQ("kW", utilityBill.peakDemandUnit().get());
  EXPECT_EQ(0, utilityBill.billingPeriods().size());

  BillingPeriod bp1 = utilityBill.addBillingPeriod();
  EXPECT_EQ(Date(1, 1, 1999), bp1.startDate());
  EXPECT_EQ(30, bp1.numberOfDays());
  EXPECT_EQ(Date(1, 30, 1999), bp1.endDate());

  BillingPeriod bp2 = utilityBill.addBillingPeriod();
  EXPECT_EQ(Date(1, 31, 1999), bp2.startDate());
  EXPECT_EQ(30, bp2.numberOfDays());
  EXPECT_EQ(Date(3, 1, 1999), bp2.endDate());
  EXPECT_TRUE(bp2.setNumberOfDays(10));
  EXPECT_EQ(Date(1, 31, 1999), bp2.startDate());
  EXPECT_EQ(10, bp2.numberOfDays());
  EXPECT_EQ(Date(2, 9, 1999), bp2.endDate());

  BillingPeriod bp3 = utilityBill.addBillingPeriod();
  EXPECT_EQ(Date(2, 10, 1999), bp3.startDate());
  EXPECT_EQ(10, bp3.numberOfDays());
  EXPECT_EQ(Date(2, 19, 1999), bp3.endDate());

  /** Sets the start date.  If startDate is before endDate then endDate is retained.
      If startDate is after endDate then numberOfDays is retained. */
  EXPECT_TRUE(bp1.setStartDate(Date(1, 6, 1999)));
  EXPECT_EQ(Date(1, 6, 1999), bp1.startDate());
  EXPECT_EQ(25, bp1.numberOfDays());
  EXPECT_EQ(Date(1, 30, 1999), bp1.endDate());

  EXPECT_TRUE(bp1.setStartDate(Date(2, 1, 1999)));
  EXPECT_EQ(Date(2, 1, 1999), bp1.startDate());
  EXPECT_EQ(25, bp1.numberOfDays());
  EXPECT_EQ(Date(2, 25, 1999), bp1.endDate());

  /** Sets the number of days in billing period, startDate is always retained. */
  EXPECT_TRUE(bp1.setNumberOfDays(4));
  EXPECT_EQ(Date(2, 1, 1999), bp1.startDate());
  EXPECT_EQ(4, bp1.numberOfDays());
  EXPECT_EQ(Date(2, 4, 1999), bp1.endDate());

  /** Sets the end date.  If endDate is after startDate then startDate is retained.
      If endDate is before startDate then numberOfDays is retained. */
  EXPECT_TRUE(bp1.setEndDate(Date(2, 8, 1999)));
  EXPECT_EQ(Date(2, 1, 1999), bp1.startDate());
  EXPECT_EQ(8, bp1.numberOfDays());
  EXPECT_EQ(Date(2, 8, 1999), bp1.endDate());

  EXPECT_TRUE(bp1.setEndDate(Date(1, 8, 1999)));
  EXPECT_EQ(Date(1, 1, 1999), bp1.startDate());
  EXPECT_EQ(8, bp1.numberOfDays());
  EXPECT_EQ(Date(1, 8, 1999), bp1.endDate());

  EXPECT_EQ(0u, model.getModelObjects<OutputMeter>().size());
  OutputMeter meter = utilityBill.consumptionMeter();
  EXPECT_EQ(1u, model.getModelObjects<OutputMeter>().size());
  EXPECT_EQ("Daily", meter.reportingFrequency());
  ASSERT_TRUE(meter.fuelType());
  EXPECT_EQ(FuelType::Electricity, meter.fuelType()->value());
  EXPECT_FALSE(meter.specificEndUse());
  EXPECT_FALSE(meter.endUseType());
  ASSERT_TRUE(meter.installLocationType());
  EXPECT_EQ(InstallLocationType::Facility, meter.installLocationType()->value());
  EXPECT_FALSE(meter.specificInstallLocation());

  OutputMeter meter2 = utilityBill.consumptionMeter();
  EXPECT_EQ(1u, model.getModelObjects<OutputMeter>().size());

  OutputMeter meter3 = utilityBill.consumptionMeter();
  EXPECT_EQ(1u, model.getModelObjects<OutputMeter>().size());

  OutputMeter meter4 = utilityBill.consumptionMeter();
  EXPECT_EQ(1u, model.getModelObjects<OutputMeter>().size());

  boost::optional<OutputMeter> meter5 = utilityBill.peakDemandMeter();
  ASSERT_TRUE(meter5);
  EXPECT_EQ(2u, model.getModelObjects<OutputMeter>().size());

  boost::optional<OutputMeter> meter6 = utilityBill.peakDemandMeter();
  ASSERT_TRUE(meter5);
  EXPECT_EQ(2u, model.getModelObjects<OutputMeter>().size());

  boost::optional<OutputMeter> meter7 = utilityBill.peakDemandMeter();
  ASSERT_TRUE(meter6);
  EXPECT_EQ(2u, model.getModelObjects<OutputMeter>().size());

  OutputMeter meter8 = utilityBill.consumptionMeter();
  EXPECT_EQ(2u, model.getModelObjects<OutputMeter>().size());

  boost::optional<OutputMeter> meter9 = utilityBill.peakDemandMeter();
  ASSERT_TRUE(meter9);
  EXPECT_EQ(2u, model.getModelObjects<OutputMeter>().size());
}

TEST_F(ModelFixture, UtilityBill_Gas_Issue765) {
  Model model;

  model::YearDescription yearDescription = model.getUniqueModelObject<model::YearDescription>();
  EXPECT_FALSE(yearDescription.calendarYear());
  yearDescription.setCalendarYear(2011);
  ASSERT_TRUE(yearDescription.calendarYear());
  EXPECT_EQ(2011, yearDescription.calendarYear().get());
  EXPECT_EQ(2011, yearDescription.assumedYear());
  EXPECT_EQ("Saturday", yearDescription.dayofWeekforStartDay());
  EXPECT_FALSE(yearDescription.isLeapYear());

  UtilityBill utilityBill(FuelType::Gas, model);
  EXPECT_EQ(FuelType::Gas, utilityBill.fuelType().value());
  EXPECT_EQ(InstallLocationType::Facility, utilityBill.meterInstallLocation().value());
  EXPECT_TRUE(utilityBill.isMeterInstallLocationDefaulted());
  EXPECT_FALSE(utilityBill.meterSpecificInstallLocation());
  EXPECT_FALSE(utilityBill.meterEndUseCategory());
  EXPECT_FALSE(utilityBill.meterSpecificEndUse());
  EXPECT_EQ("therms", utilityBill.consumptionUnit());
  EXPECT_NEAR(105505585, utilityBill.consumptionUnitConversionFactor(), 1);
  EXPECT_FALSE(utilityBill.peakDemandUnit());
  EXPECT_EQ(0, utilityBill.billingPeriods().size());

  BillingPeriod bp1 = utilityBill.addBillingPeriod();
  EXPECT_EQ(Date(1, 1, 2011), bp1.startDate());
  EXPECT_EQ(30, bp1.numberOfDays());
  EXPECT_EQ(Date(1, 30, 2011), bp1.endDate());

  BillingPeriod bp2 = utilityBill.addBillingPeriod();
  EXPECT_EQ(Date(1, 31, 2011), bp2.startDate());
  EXPECT_EQ(30, bp2.numberOfDays());
  EXPECT_EQ(Date(3, 1, 2011), bp2.endDate());

  EXPECT_TRUE(bp2.setStartDate(Date(2, 1, 2011)));
  EXPECT_EQ(Date(2, 1, 2011), bp2.startDate());
  EXPECT_EQ(29, bp2.numberOfDays());
  EXPECT_EQ(Date(3, 1, 2011), bp2.endDate());
}

TEST_F(ModelFixture, UtilityBill_Electricity_Issue765) {
  Model model;

  model::YearDescription yearDescription = model.getUniqueModelObject<model::YearDescription>();
  EXPECT_FALSE(yearDescription.calendarYear());
  yearDescription.setCalendarYear(2011);
  ASSERT_TRUE(yearDescription.calendarYear());
  EXPECT_EQ(2011, yearDescription.calendarYear().get());
  EXPECT_EQ(2011, yearDescription.assumedYear());
  EXPECT_EQ("Saturday", yearDescription.dayofWeekforStartDay());
  EXPECT_FALSE(yearDescription.isLeapYear());

  UtilityBill utilityBill(FuelType::Electricity, model);
  EXPECT_EQ(FuelType::Electricity, utilityBill.fuelType().value());
  EXPECT_EQ(InstallLocationType::Facility, utilityBill.meterInstallLocation().value());
  EXPECT_TRUE(utilityBill.isMeterInstallLocationDefaulted());
  EXPECT_FALSE(utilityBill.meterSpecificInstallLocation());
  EXPECT_FALSE(utilityBill.meterEndUseCategory());
  EXPECT_FALSE(utilityBill.meterSpecificEndUse());
  EXPECT_EQ("kWh", utilityBill.consumptionUnit());
  EXPECT_NEAR(3600000, utilityBill.consumptionUnitConversionFactor(), 1);
  ASSERT_TRUE(utilityBill.peakDemandUnit());
  EXPECT_EQ("kW", utilityBill.peakDemandUnit().get());
  EXPECT_EQ(0, utilityBill.billingPeriods().size());

  BillingPeriod bp1 = utilityBill.addBillingPeriod();
  EXPECT_EQ(Date(1, 1, 2011), bp1.startDate());
  EXPECT_EQ(30, bp1.numberOfDays());
  EXPECT_EQ(Date(1, 30, 2011), bp1.endDate());

  BillingPeriod bp2 = utilityBill.addBillingPeriod();
  EXPECT_EQ(Date(1, 31, 2011), bp2.startDate());
  EXPECT_EQ(30, bp2.numberOfDays());
  EXPECT_EQ(Date(3, 1, 2011), bp2.endDate());

  EXPECT_TRUE(bp2.setStartDate(Date(2, 1, 2011)));
  EXPECT_EQ(Date(2, 1, 2011), bp2.startDate());
  EXPECT_EQ(29, bp2.numberOfDays());
  EXPECT_EQ(Date(3, 1, 2011), bp2.endDate());
}

TEST_F(ModelFixture, UtilityBill_Coverage) {
  Model model;

  for (FuelType fuelType : FuelType::getValues()) {

    UtilityBill utilityBill(fuelType, model);
    EXPECT_EQ(fuelType, utilityBill.fuelType().value());
    EXPECT_EQ(InstallLocationType::Facility, utilityBill.meterInstallLocation().value());
    EXPECT_TRUE(utilityBill.isMeterInstallLocationDefaulted());
    EXPECT_FALSE(utilityBill.meterSpecificInstallLocation());
    EXPECT_FALSE(utilityBill.meterEndUseCategory());
    EXPECT_FALSE(utilityBill.meterSpecificEndUse());
    EXPECT_NE("", utilityBill.consumptionUnit());
    EXPECT_NE(0, utilityBill.consumptionUnitConversionFactor());
    EXPECT_EQ(0, utilityBill.billingPeriods().size());

    EXPECT_FALSE(utilityBill.consumptionUnitValues().empty());
    for (const std::string& consumptionUnit : utilityBill.consumptionUnitValues()) {

      OutputMeter meter = utilityBill.consumptionMeter();

      EXPECT_TRUE(utilityBill.setConsumptionUnit(consumptionUnit)) << fuelType.valueName() << ", " << consumptionUnit;
      EXPECT_EQ(consumptionUnit, utilityBill.consumptionUnit());
      EXPECT_NE(0, utilityBill.consumptionUnitConversionFactor());
      EXPECT_TRUE(utilityBill.consumptionUnitDescription());

      for (const std::string& peakDemandUnit : utilityBill.peakDemandUnitValues()) {

        boost::optional<OutputMeter> peakDemandMeter = utilityBill.peakDemandMeter();

        EXPECT_TRUE(utilityBill.setPeakDemandUnit(peakDemandUnit)) << fuelType.valueName() << ", " << peakDemandUnit;
        ASSERT_TRUE(utilityBill.peakDemandUnit());
        EXPECT_EQ(peakDemandUnit, utilityBill.peakDemandUnit().get());
        ASSERT_TRUE(utilityBill.peakDemandUnitConversionFactor());
        EXPECT_NE(0, utilityBill.peakDemandUnitConversionFactor().get());
        EXPECT_TRUE(utilityBill.peakDemandUnitDescription());
      }
    }
  }

  for (const std::string& calibrationGuideline : UtilityBill::calibrationGuidelines()) {
    // results tabs in OS app and PAT require these return values for each calibrationGuideline
    EXPECT_TRUE(UtilityBill::maxNMBE(calibrationGuideline)) << "maxNMBE " << calibrationGuideline;
    EXPECT_TRUE(UtilityBill::maxCVRMSE(calibrationGuideline)) << "maxCVRMSE " << calibrationGuideline;
  }
}
