/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AirflowNetworkDetailedOpening.hpp"
#include "../AirflowNetworkDetailedOpening_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirflowNetwork_DetailedOpening) {
  Model model;

  std::vector<DetailedOpeningFactorData> data = {DetailedOpeningFactorData(0.0, 0.01, 0.0, 0.0, 0.0),
                                                 DetailedOpeningFactorData(1.0, 0.5, 1.0, 1.0, 0.0)};

  AirflowNetworkDetailedOpening detailed0(model, 1.0, data);
  AirflowNetworkDetailedOpening detailed1(model, 1.0, 0.5, "HorizontallyPivoted", 0.0, data);

  EXPECT_EQ(1, detailed0.airMassFlowCoefficientWhenOpeningisClosed());
  EXPECT_EQ(0.65, detailed0.airMassFlowExponentWhenOpeningisClosed());
  EXPECT_EQ("NonPivoted", detailed0.typeofRectangularLargeVerticalOpening());
  std::vector<DetailedOpeningFactorData> retData = detailed0.openingFactors();
  ASSERT_EQ(2u, retData.size());
  EXPECT_EQ(0, retData[0].openingFactor());
  EXPECT_EQ(1, retData[1].openingFactor());

  EXPECT_EQ(1, detailed1.airMassFlowCoefficientWhenOpeningisClosed());
  EXPECT_EQ(0.5, detailed1.airMassFlowExponentWhenOpeningisClosed());
  EXPECT_EQ("HorizontallyPivoted", detailed1.typeofRectangularLargeVerticalOpening());
  retData = detailed1.openingFactors();
  ASSERT_EQ(2u, retData.size());
  EXPECT_EQ(0, retData[0].openingFactor());
  EXPECT_EQ(1, retData[1].openingFactor());
}

TEST_F(ModelFixture, AirflowNetwork_DetailedOpening_InvalidOpeningFactors) {
  Model model;

  std::vector<DetailedOpeningFactorData> data;

  data = {DetailedOpeningFactorData(0.0, 0.01, 0.0, 0.0, 0.0), DetailedOpeningFactorData(1.0, 0.5, 1.0, 1.0, 0.0)};

  AirflowNetworkDetailedOpening detailed0(model, 1.0, data);

  EXPECT_TRUE(detailed0.setOpeningFactors(data));

  // Number of Sets of Opening Factor Data
  data = {DetailedOpeningFactorData(0.0, 0.01, 0.0, 0.0, 0.0)};
  EXPECT_FALSE(detailed0.setOpeningFactors(data));

  data = {DetailedOpeningFactorData(0.0, 0.01, 0.0, 0.0, 0.0), DetailedOpeningFactorData(1.0, 0.5, 1.0, 1.0, 0.0),
          DetailedOpeningFactorData(1.0, 0.5, 1.0, 1.0, 0.0), DetailedOpeningFactorData(1.0, 0.5, 1.0, 1.0, 0.0),
          DetailedOpeningFactorData(1.0, 0.5, 1.0, 1.0, 0.0)};
  EXPECT_FALSE(detailed0.setOpeningFactors(data));

  // Opening Factor
  data = {DetailedOpeningFactorData(0.5, 0.01, 0.0, 0.0, 0.0), DetailedOpeningFactorData(1.0, 0.5, 1.0, 1.0, 0.0)};
  EXPECT_FALSE(detailed0.setOpeningFactors(data));

  data = {DetailedOpeningFactorData(0.0, 0.01, 0.0, 0.0, 0.0), DetailedOpeningFactorData(0.5, 0.5, 1.0, 1.0, 0.0)};
  EXPECT_FALSE(detailed0.setOpeningFactors(data));

  // Height Factor and Start Height Factor
  data = {DetailedOpeningFactorData(0.0, 0.01, 0.0, 0.0, 0.0), DetailedOpeningFactorData(1.0, 0.5, 1.0, 1.0, 1.0)};
  EXPECT_FALSE(detailed0.setOpeningFactors(data));
}
