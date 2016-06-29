/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../MaterialPropertyGlazingSpectralData.hpp"
#include "../MaterialPropertyGlazingSpectralData_Impl.hpp"

#include "../StandardGlazing.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, MaterialPropertyGlazingSpectralData)
{
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
