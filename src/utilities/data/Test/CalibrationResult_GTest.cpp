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

#include <gtest/gtest.h>
#include "DataFixture.hpp"

#include "../CalibrationResult.hpp"
#include "../../time/Date.hpp"

using namespace openstudio;


TEST_F(DataFixture, CalibrationResult)
{

  openstudio::path xmlPath = toPath("./calibration_report.xml");
  if(openstudio::filesystem::exists(xmlPath)){
    openstudio::filesystem::remove(xmlPath);
  }

  // these objects do no calculations they just store data as entered

  CalibrationUtilityBill electricBill("Electric Bill", FuelType::Electricity, InstallLocationType::Facility,
    boost::none, boost::none, boost::none, std::string("kWh"), 5.0, std::string("kW"), 5.0, 5, 30.0, 2, 1.0, 1.0);
  EXPECT_EQ("Electric Bill", electricBill.name());
  EXPECT_EQ(FuelType::Electricity, electricBill.fuelType().value());
  EXPECT_EQ(InstallLocationType::Facility, electricBill.meterInstallLocation().value());
  EXPECT_FALSE(electricBill.meterSpecificInstallLocation());
  EXPECT_FALSE(electricBill.meterEndUseCategory());
  EXPECT_FALSE(electricBill.meterSpecificEndUse());
  EXPECT_EQ("kWh", electricBill.consumptionUnit());
  EXPECT_EQ(5.0, electricBill.consumptionUnitConversionFactor());
  ASSERT_TRUE(electricBill.peakDemandUnit());
  EXPECT_EQ("kW", electricBill.peakDemandUnit().get());
  ASSERT_TRUE(electricBill.peakDemandUnitConversionFactor());
  EXPECT_EQ(5.0, electricBill.peakDemandUnitConversionFactor().get());
  ASSERT_TRUE(electricBill.timestepsInPeakDemandWindow());
  EXPECT_EQ(5, electricBill.timestepsInPeakDemandWindow().get());
  ASSERT_TRUE(electricBill.minutesInPeakDemandWindow());
  EXPECT_EQ(30, electricBill.minutesInPeakDemandWindow().get());
  ASSERT_TRUE(electricBill.numberBillingPeriodsInCalculations());
  EXPECT_EQ(2, electricBill.numberBillingPeriodsInCalculations().get());
  ASSERT_TRUE(electricBill.CVRMSE());
  EXPECT_EQ(1.0, electricBill.CVRMSE().get());
  ASSERT_TRUE(electricBill.NMBE());
  EXPECT_EQ(1.0, electricBill.NMBE().get());

  CalibrationBillingPeriod electricityJan(Date(1,1,1999), 30, std::string("kWh"), std::string("kW"),
    10.0, 1.0, boost::none, 11.0, 1.1, boost::none);
  EXPECT_EQ(Date(1,1,1999), electricityJan.startDate());
  EXPECT_EQ(Date(1,30,1999), electricityJan.endDate());
  EXPECT_EQ(30, electricityJan.numberOfDays());
  EXPECT_EQ("kWh", electricityJan.consumptionUnit());
  ASSERT_TRUE(electricityJan.peakDemandUnit());
  EXPECT_EQ("kW", electricityJan.peakDemandUnit().get());
  ASSERT_TRUE(electricityJan.consumption());
  EXPECT_EQ(10.0, electricityJan.consumption().get());
  ASSERT_TRUE(electricityJan.peakDemand());
  EXPECT_EQ(1.0, electricityJan.peakDemand().get());
  EXPECT_FALSE(electricityJan.totalCost());
  ASSERT_TRUE(electricityJan.modelConsumption());
  EXPECT_EQ(11.0, electricityJan.modelConsumption().get());
  ASSERT_TRUE(electricityJan.modelPeakDemand());
  EXPECT_EQ(1.1, electricityJan.modelPeakDemand().get());
  EXPECT_FALSE(electricityJan.modelTotalCost());
  EXPECT_TRUE(electricBill.addBillingPeriod(electricityJan));

  CalibrationBillingPeriod electricityFeb(Date(2,1,1999), 28, std::string("kWh"), std::string("kW"),
    10.0, 1.0, boost::none, 11.0, 1.1, boost::none);
  EXPECT_TRUE(electricBill.addBillingPeriod(electricityFeb));
  EXPECT_EQ(2u, electricBill.billingPeriods().size());


  CalibrationUtilityBill gasBill("Gas Bill", FuelType::Gas, InstallLocationType::Facility,
    boost::none, boost::none, boost::none, std::string("Therms"), 5.0, boost::none, boost::none, boost::none, boost::none, 3, 1.0, 1.0);

  CalibrationBillingPeriod gasJan(Date(1,1,1999), 30, std::string("Therms"), boost::none,
    10.0, boost::none, boost::none, 11.0, boost::none, boost::none);
  EXPECT_TRUE(gasBill.addBillingPeriod(gasJan));

  CalibrationBillingPeriod gasFeb(Date(2,1,1999), 28, std::string("Therms"), boost::none,
    10.0, boost::none, boost::none, 11.0, boost::none, boost::none);
  EXPECT_TRUE(gasBill.addBillingPeriod(gasFeb));
  EXPECT_EQ(2u, gasBill.billingPeriods().size());


  EXPECT_FALSE(electricBill.addBillingPeriod(gasJan));
  EXPECT_EQ(2u, electricBill.billingPeriods().size());
  EXPECT_FALSE(gasBill.addBillingPeriod(electricityJan));
  EXPECT_EQ(2u, gasBill.billingPeriods().size());


  CalibrationResult calibrationResult;
  calibrationResult.addUtilityBill(electricBill);
  calibrationResult.addUtilityBill(gasBill);
  EXPECT_EQ(2u, calibrationResult.utilityBills().size());

  // save to xml
  calibrationResult.attribute().saveToXml(xmlPath);

  // load
  boost::optional<Attribute> testAttribute = Attribute::loadFromXml(xmlPath);
  ASSERT_TRUE(testAttribute);
  boost::optional<CalibrationResult> testCalibrationResult = CalibrationResult::fromAttribute(*testAttribute);
  ASSERT_TRUE(testCalibrationResult);

  ASSERT_EQ(2u, testCalibrationResult->utilityBills().size());
  CalibrationUtilityBill electricBill2 = testCalibrationResult->utilityBills()[0];
  EXPECT_EQ(electricBill.name(), electricBill2.name());
  EXPECT_EQ(electricBill.billingPeriods().size(), electricBill2.billingPeriods().size());

}
