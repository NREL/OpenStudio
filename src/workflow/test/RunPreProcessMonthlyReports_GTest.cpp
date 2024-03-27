/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "../RunPreProcessMonthlyReports.hpp"
#include "../../model/OutputMeter.hpp"

#include "../../utilities/data/DataEnums.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <boost/regex.hpp>

#include <string>

class WorkflowFixture : public testing::Test
{
};

TEST_F(WorkflowFixture, RunPreProcessMonthlyReports) {

  const std::string peakDemandSuffix = " Peak Demand";
  const std::string expectedPrefix = "Building Energy Performance";

  size_t n_regular = 0;
  size_t n_demand = 0;

  const auto meterRegex = openstudio::model::OutputMeter::meterRegex();

  auto endUseType = [&meterRegex](const std::string& name) -> boost::optional<openstudio::EndUseType> {
    boost::optional<openstudio::EndUseType> result;
    boost::smatch matches;
    if (boost::regex_search(name, matches, meterRegex)) {
      if (matches[2].matched) {
        result = openstudio::EndUseType(std::string(matches[2].first, matches[2].second));
      }
    }

    return result;
  };

  auto fuelType = [&meterRegex](const std::string& name) -> boost::optional<openstudio::FuelType> {
    boost::optional<openstudio::FuelType> result;
    boost::smatch matches;
    if (boost::regex_search(name, matches, meterRegex)) {
      if (matches[3].matched) {
        result = openstudio::FuelType(std::string(matches[3].first, matches[3].second));
      }
    }

    return result;
  };

  auto installLocationType = [&meterRegex](const std::string& name) -> boost::optional<openstudio::InstallLocationType> {
    boost::optional<openstudio::InstallLocationType> result;
    boost::smatch matches;
    if (boost::regex_search(name, matches, meterRegex)) {
      if (matches[4].matched) {
        result = openstudio::InstallLocationType(std::string(matches[4].first, matches[4].second));
      }
    }

    return result;
  };

  for (auto meter : c_monthlyReports) {
    auto idfObject_ = openstudio::IdfObject::load(std::string{meter});

    ASSERT_TRUE(idfObject_);
    auto idfObject = std::move(*idfObject_);
    auto name = idfObject.nameString();
    SCOPED_TRACE("Processing " + name + "\n========================================");
    const auto n = name.find(" - ");
    auto tableFuelType = name.substr(n + 3);

    EXPECT_NE(n, std::string::npos);
    auto prefix = name.substr(0, n);
    EXPECT_EQ(expectedPrefix, prefix);

    auto demandN = tableFuelType.find(peakDemandSuffix);
    bool isDemand = false;
    if (demandN != std::string::npos) {
      isDemand = true;
      ++n_demand;
      tableFuelType = tableFuelType.substr(0, tableFuelType.size() - peakDemandSuffix.size());
    } else {
      ++n_regular;
    }
    try {
      auto tableEUFT = openstudio::EndUseFuelType(tableFuelType);
      EXPECT_EQ(tableFuelType, tableEUFT.valueDescription());
    } catch (...) {
      EXPECT_TRUE(false) << tableFuelType << " is not a valid EndUseFuelType";
    }

    EXPECT_EQ(2, idfObject.getInt(1).get());
    const std::string aggregrationType = isDemand ? "ValueWhenMaximumOrMinimum" : "SumOrAverage";
    bool is_first = true;
    for (const auto& eg : idfObject.extensibleGroups()) {
      const auto meterName = eg.getString(0).get();
      SCOPED_TRACE("Processing extensible group " + meterName);

      const auto eut = endUseType(meterName);
      const auto ft = fuelType(meterName);
      const auto loc = installLocationType(meterName);

      const auto reconstructedMeterName = openstudio::model::OutputMeter::getName(boost::none, eut, ft, loc, boost::none);

      // This is the definitive test
      EXPECT_EQ(reconstructedMeterName, meterName);

      if (isDemand && is_first) {
        // Electricity:Facility : Maximum
        EXPECT_FALSE(eut) << "EndUseType should not be present";
        EXPECT_TRUE(ft) << "FuelType should be present";
        ASSERT_TRUE(loc) << "InstallLocationType should be present";
        EXPECT_EQ("Facility", loc->valueName());

        EXPECT_EQ("Maximum", eg.getString(1).get());

        is_first = false;
        continue;
      }

      EXPECT_TRUE(eut) << "EndUseType should be present";
      EXPECT_TRUE(ft) << "FuelType should be present";
      EXPECT_FALSE(loc) << "InstallLocationType should not be present";

      EXPECT_EQ(aggregrationType, eg.getString(1).get());
    }
  }

  EXPECT_EQ(n_regular, n_demand);
  EXPECT_EQ(4, n_regular);
}
