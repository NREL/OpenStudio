/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
  //EXPECT_THROW(intVariant.valueAsDouble(), boost::bad_get); // DLM: allowed this since double values in OSW might appear to be ints
  EXPECT_THROW(intVariant.valueAsString(), boost::bad_get);

  EXPECT_THROW(doubleVariant.valueAsBoolean(), boost::bad_get);
  EXPECT_THROW(doubleVariant.valueAsInteger(), boost::bad_get);
  EXPECT_THROW(doubleVariant.valueAsString(), boost::bad_get);

  EXPECT_THROW(stringVariant.valueAsBoolean(), boost::bad_get);
  EXPECT_THROW(stringVariant.valueAsInteger(), boost::bad_get);
  EXPECT_THROW(stringVariant.valueAsDouble(), boost::bad_get);
}
