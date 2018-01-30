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

#include <algorithm>
#include <gtest/gtest.h>

#include <string>
#include <boost/optional.hpp>

#include "ModelFixture.hpp"

#include "../AdditionalProperties.hpp"
#include "../AdditionalProperties_Impl.hpp"
#include "../ModelObject.hpp"
#include "../ModelObject_Impl.hpp"
#include "../Construction.hpp"
#include "../Construction_Impl.hpp"

#include "../utilities/geometry/Geometry.hpp"
#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AdditionalProperties_Features)
{
  Model model;

  Construction construction(model);
  
  AdditionalProperties addlProps = construction.additionalProperties();

  // Test initially setting a string value
  EXPECT_TRUE(addlProps.setFeature("NumberOfBedrooms", "five"));
  boost::optional<std::string> dataType = addlProps.getFeatureDataType("NumberOfBedrooms");
  ASSERT_TRUE(dataType);
  EXPECT_EQ(*dataType, "String");
  boost::optional<std::string> strValue = addlProps.getFeatureAsString("NumberOfBedrooms");
  ASSERT_TRUE(strValue);
  EXPECT_EQ(*strValue, "five");

  // Test changing a string value
  EXPECT_TRUE(addlProps.setFeature("NumberOfBedrooms", "six"));
  dataType = addlProps.getFeatureDataType("NumberOfBedrooms");
  ASSERT_TRUE(dataType);
  EXPECT_EQ(*dataType, "String");
  strValue = addlProps.getFeatureAsString("NumberOfBedrooms");
  ASSERT_TRUE(strValue);
  EXPECT_EQ(*strValue, "six");

  // Test setting a boolean value
  EXPECT_TRUE(addlProps.setFeature("isNicePlace", false));
  dataType = addlProps.getFeatureDataType("isNicePlace");
  ASSERT_TRUE(dataType);
  EXPECT_EQ(*dataType, "Boolean");
  boost::optional<bool> boolValue = addlProps.getFeatureAsBoolean("isNicePlace");
  ASSERT_TRUE(boolValue);
  ASSERT_FALSE(*boolValue);

  // Test changing a boolean value
  EXPECT_TRUE(addlProps.setFeature("isNicePlace", true));
  dataType = addlProps.getFeatureDataType("isNicePlace");
  ASSERT_TRUE(dataType);
  EXPECT_EQ(*dataType, "Boolean");
  boolValue = addlProps.getFeatureAsBoolean("isNicePlace");
  ASSERT_TRUE(boolValue);
  ASSERT_TRUE(*boolValue);

  // Test Feature Name getter
  const std::vector<std::string> featureNames(addlProps.featureNames());
  EXPECT_EQ(featureNames.size(), 2);
  EXPECT_EQ(featureNames[0], "NumberOfBedrooms");
  EXPECT_EQ(featureNames[1], "isNicePlace");

  // Test setting a double value
  EXPECT_TRUE(addlProps.setFeature("NumberOfBathrooms", 1.5));
  dataType = addlProps.getFeatureDataType("NumberOfBathrooms");
  ASSERT_TRUE(dataType);
  EXPECT_EQ(*dataType, "Double");
  boost::optional<double> dblValue = addlProps.getFeatureAsDouble("NumberOfBathrooms");
  ASSERT_TRUE(dblValue);
  boost::optional<int> intValue = addlProps.getFeatureAsInteger("NumberOfBathrooms");
  ASSERT_FALSE(intValue);
  ASSERT_FLOAT_EQ(*dblValue, 1.5);

  // Test setting a feature to a value of a different type
  EXPECT_TRUE(addlProps.setFeature("NumberOfBedrooms", 3));
  dataType = addlProps.getFeatureDataType("NumberOfBedrooms");
  ASSERT_TRUE(dataType);
  EXPECT_EQ(*dataType, "Integer");
  dblValue = addlProps.getFeatureAsDouble("NumberOfBedrooms");
  ASSERT_FALSE(dblValue);
  intValue = addlProps.getFeatureAsInteger("NumberOfBedrooms");
  ASSERT_TRUE(intValue);
  ASSERT_EQ(*intValue, 3);

  // Test resetting a feature
  EXPECT_TRUE(addlProps.resetFeature("isNicePlace"));
  dataType = addlProps.getFeatureDataType("isNicePlace");
  ASSERT_FALSE(dataType);
  EXPECT_FALSE(addlProps.resetFeature("bogusName"));

  // Test suggested feature names
  EXPECT_TRUE(addlProps.setFeature("MyUniqueFeature", 5));
  Construction construction2(model);  
  AdditionalProperties addlProps2 = construction2.additionalProperties();
  std::vector<std::string> suggestedFeatures(addlProps2.suggestedFeatures());
  ASSERT_EQ(suggestedFeatures.size(), 3);
  ASSERT_NE(std::find(suggestedFeatures.begin(), suggestedFeatures.end(), "NumberOfBedrooms"), suggestedFeatures.end());
  ASSERT_NE(std::find(suggestedFeatures.begin(), suggestedFeatures.end(), "NumberOfBathrooms"), suggestedFeatures.end());
  ASSERT_NE(std::find(suggestedFeatures.begin(), suggestedFeatures.end(), "MyUniqueFeature"), suggestedFeatures.end());

}

TEST_F(ModelFixture, ModelObject_AdditionalProperties)
{

  Model model;

  Construction construction(model);
  EXPECT_EQ(0, model.getModelObjects<AdditionalProperties>().size());

  AdditionalProperties tmp = construction.additionalProperties();
  EXPECT_EQ(1u, model.getModelObjects<AdditionalProperties>().size());

  AdditionalProperties addlProps = construction.additionalProperties();
  EXPECT_EQ(1u, model.getModelObjects<AdditionalProperties>().size());
  EXPECT_EQ(toString(addlProps.handle()), toString(tmp.handle()));
  // EXPECT_EQ(toString(construction.handle()), toString(addlProps.construction().handle()));

}