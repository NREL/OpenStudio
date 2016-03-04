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
#include "DataFixture.hpp"

#include "../Variant.hpp"

using namespace openstudio;


TEST_F(DataFixture, Variant)
{
  Variant boolVariant(true);
  Variant intVariant(1);
  Variant doubleVariant(1.0);
  Variant stringVariant("1");

  EXPECT_EQ(VariantType::Boolean, boolVariant.variantType().value());
  EXPECT_EQ(VariantType::Integer, intVariant.variantType().value());
  EXPECT_EQ(VariantType::Double, doubleVariant.variantType().value());
  EXPECT_EQ(VariantType::String, stringVariant.variantType().value());

  EXPECT_EQ(true, boolVariant.valueAsBoolean());
  EXPECT_EQ(1, intVariant.valueAsInteger());
  EXPECT_EQ(1.0, doubleVariant.valueAsDouble());
  EXPECT_EQ("1", stringVariant.valueAsString());

  EXPECT_THROW(boolVariant.valueAsInteger(), boost::bad_get);
  EXPECT_THROW(boolVariant.valueAsDouble(), boost::bad_get);
  EXPECT_THROW(boolVariant.valueAsString(), boost::bad_get);

  EXPECT_THROW(intVariant.valueAsBoolean(), boost::bad_get);
  EXPECT_THROW(intVariant.valueAsDouble(), boost::bad_get);
  EXPECT_THROW(intVariant.valueAsString(), boost::bad_get);

  EXPECT_THROW(doubleVariant.valueAsBoolean(), boost::bad_get);
  EXPECT_THROW(doubleVariant.valueAsInteger(), boost::bad_get);
  EXPECT_THROW(doubleVariant.valueAsString(), boost::bad_get);

  EXPECT_THROW(stringVariant.valueAsBoolean(), boost::bad_get);
  EXPECT_THROW(stringVariant.valueAsInteger(), boost::bad_get);
  EXPECT_THROW(stringVariant.valueAsDouble(), boost::bad_get);
}
