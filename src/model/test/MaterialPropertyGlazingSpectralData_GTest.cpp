/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../MaterialPropertyGlazingSpectralData.hpp"
#include "../MaterialPropertyGlazingSpectralData_Impl.hpp"

#include "../StandardGlazing.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, MaterialPropertyGlazingSpectralData) {
  Model model;

  MaterialPropertyGlazingSpectralData mpgsd(model);

  EXPECT_EQ(0u, mpgsd.spectralDataFields().size());

  EXPECT_TRUE(mpgsd.addSpectralDataField(0.3, 0.0, 0.045, 0.045));
  EXPECT_EQ(1u, mpgsd.spectralDataFields().size());

  EXPECT_TRUE(mpgsd.addSpectralDataField(SpectralDataField(0.31, 0.0, 0.044, 0.044)));
  EXPECT_EQ(2u, mpgsd.spectralDataFields().size());

  std::vector<SpectralDataField> spectralDataFields = mpgsd.spectralDataFields();
  spectralDataFields.push_back(SpectralDataField(0.32, 0.0, 0.044, 0.044));

  EXPECT_TRUE(mpgsd.setSpectralDataFields(spectralDataFields));
  EXPECT_EQ(3u, mpgsd.spectralDataFields().size());

  StandardGlazing glazing(model);
  EXPECT_EQ("SpectralAverage", glazing.opticalDataType());
  EXPECT_FALSE(glazing.windowGlassSpectralDataSet());

  EXPECT_TRUE(glazing.setWindowGlassSpectralDataSet(mpgsd));
  EXPECT_TRUE(glazing.windowGlassSpectralDataSet());
  EXPECT_EQ("Spectral", glazing.opticalDataType());

  glazing.resetWindowGlassSpectralDataSet();
  EXPECT_EQ("SpectralAverage", glazing.opticalDataType());
  EXPECT_FALSE(glazing.windowGlassSpectralDataSet());
}
