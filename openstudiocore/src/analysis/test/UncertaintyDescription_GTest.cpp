/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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
#include "AnalysisFixture.hpp"

#include "../NormalDistribution.hpp"
#include "../GenericUncertaintyDescription.hpp"

#include "../../utilities/data/Attribute.hpp"

using namespace openstudio;
using namespace openstudio::analysis;

TEST_F(AnalysisFixture,UncertaintyDescription_ConstructionCopyingAndCasting) {
  NormalDistribution stdNormal;
  {
    UncertaintyDescription baseCopy = stdNormal; // copies impl
  } // calls destructor
  EXPECT_DOUBLE_EQ(0.0,stdNormal.mean());
  EXPECT_DOUBLE_EQ(1.0,stdNormal.standardDeviation());
  EXPECT_FALSE(stdNormal.lowerBound());
  EXPECT_FALSE(stdNormal.upperBound());

  {
    UncertaintyDescription baseCopy(stdNormal);
    NormalDistribution stdNormalCopy = baseCopy.cast<NormalDistribution>();
    EXPECT_EQ(UncertaintyDescriptionType(UncertaintyDescriptionType::normal_uncertain),baseCopy.type());
  }

  {
    GenericUncertaintyDescription genericCopy = stdNormal.cast<GenericUncertaintyDescription>();
    EXPECT_EQ(2u,genericCopy.attributes().size());
  }

}
