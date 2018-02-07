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

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../AdditionalProperties.hpp"
#include "../AdditionalProperties_Impl.hpp"
#include "../Construction.hpp"
#include "../Construction_Impl.hpp"
#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"

#include "../utilities/geometry/Geometry.hpp"
#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AdditionalProperties_AdditionalProperties) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

	ASSERT_EXIT(
	{
		// create a model to use
		Model model;

		// create a material object to use
		StandardOpaqueMaterial material(model);

		exit(0);
	},
		::testing::ExitedWithCode(0),
		""
		);

	// create a model to use
	Model model;

	// create a material object to use
	StandardOpaqueMaterial material(model);
	EXPECT_EQ(1, model.modelObjects().size());

	// create an additional properties object to use
	AdditionalProperties props = material.additionalProperties();
	EXPECT_EQ(2, model.modelObjects().size());
	EXPECT_EQ(1, model.getModelObjects<AdditionalProperties>().size());

	// return, instead of create, additional properies when it already exists
	AdditionalProperties props2 = material.additionalProperties();
	EXPECT_EQ(2, model.modelObjects().size());
	EXPECT_EQ(1, model.getModelObjects<AdditionalProperties>().size());
	EXPECT_EQ(props, props2);

}

// test setting and getting
TEST_F(ModelFixture, AdditionalProperties_Features) {

  Model model;
  Construction construction(model);
  AdditionalProperties props = construction.additionalProperties();

  // Test initially setting a string value
	EXPECT_TRUE(props.setFeature("NumberOfBedrooms", "five"));
  boost::optional<std::string> dataType = props.getFeatureDataType("NumberOfBedrooms");
  ASSERT_TRUE(dataType);
  EXPECT_EQ(*dataType, "String");
	boost::optional<std::string> strValue = props.getFeatureAsString("NumberOfBedrooms");
  ASSERT_TRUE(strValue);
  EXPECT_EQ(*strValue, "five");

  // Test changing a string value
	EXPECT_TRUE(props.setFeature("NumberOfBedrooms", "six"));
	dataType = props.getFeatureDataType("NumberOfBedrooms");
  ASSERT_TRUE(dataType);
  EXPECT_EQ(*dataType, "String");
	strValue = props.getFeatureAsString("NumberOfBedrooms");
  ASSERT_TRUE(strValue);
  EXPECT_EQ(*strValue, "six");

  // Test setting a boolean value
	EXPECT_TRUE(props.setFeature("isNicePlace", false));
	dataType = props.getFeatureDataType("isNicePlace");
  ASSERT_TRUE(dataType);
  EXPECT_EQ(*dataType, "Boolean");
	boost::optional<bool> boolValue = props.getFeatureAsBoolean("isNicePlace");
  ASSERT_TRUE(boolValue);
  ASSERT_FALSE(*boolValue);

  // Test changing a boolean value
	EXPECT_TRUE(props.setFeature("isNicePlace", true));
	dataType = props.getFeatureDataType("isNicePlace");
  ASSERT_TRUE(dataType);
  EXPECT_EQ(*dataType, "Boolean");
	boolValue = props.getFeatureAsBoolean("isNicePlace");
  ASSERT_TRUE(boolValue);
  ASSERT_TRUE(*boolValue);

  // Test Feature Name getter
	const std::vector<std::string> featureNames(props.featureNames());
  EXPECT_EQ(featureNames.size(), 2);
  EXPECT_EQ(featureNames[0], "NumberOfBedrooms");
  EXPECT_EQ(featureNames[1], "isNicePlace");

  // Test setting a double value
	EXPECT_TRUE(props.setFeature("NumberOfBathrooms", 1.5));
	dataType = props.getFeatureDataType("NumberOfBathrooms");
  ASSERT_TRUE(dataType);
  EXPECT_EQ(*dataType, "Double");
	boost::optional<double> dblValue = props.getFeatureAsDouble("NumberOfBathrooms");
  ASSERT_TRUE(dblValue);
	boost::optional<int> intValue = props.getFeatureAsInteger("NumberOfBathrooms");
  ASSERT_FALSE(intValue);
  ASSERT_FLOAT_EQ(*dblValue, 1.5);

  // Test setting a feature to a value of a different type
	EXPECT_TRUE(props.setFeature("NumberOfBedrooms", 3));
	dataType = props.getFeatureDataType("NumberOfBedrooms");
  ASSERT_TRUE(dataType);
  EXPECT_EQ(*dataType, "Integer");
	dblValue = props.getFeatureAsDouble("NumberOfBedrooms");
  ASSERT_FALSE(dblValue);
	intValue = props.getFeatureAsInteger("NumberOfBedrooms");
  ASSERT_TRUE(intValue);
  ASSERT_EQ(*intValue, 3);

  // Test resetting a feature
	EXPECT_TRUE(props.resetFeature("isNicePlace"));
	dataType = props.getFeatureDataType("isNicePlace");
  ASSERT_FALSE(dataType);
	EXPECT_FALSE(props.resetFeature("bogusName"));

  // Test suggested feature names
	EXPECT_TRUE(props.setFeature("MyUniqueFeature", 5));
  Construction construction2(model);  
	AdditionalProperties props2 = construction2.additionalProperties();
	std::vector<std::string> suggestedFeatures(props2.suggestedFeatures());
  ASSERT_EQ(suggestedFeatures.size(), 3);
  ASSERT_NE(std::find(suggestedFeatures.begin(), suggestedFeatures.end(), "NumberOfBedrooms"), suggestedFeatures.end());
  ASSERT_NE(std::find(suggestedFeatures.begin(), suggestedFeatures.end(), "NumberOfBathrooms"), suggestedFeatures.end());
  ASSERT_NE(std::find(suggestedFeatures.begin(), suggestedFeatures.end(), "MyUniqueFeature"), suggestedFeatures.end());

}

// check returning model object pointed to
TEST_F(ModelFixture, AdditionalProperties_ModelObject) {
	Model model;
	StandardOpaqueMaterial material(model);
	AdditionalProperties props = material.additionalProperties();
	ModelObject modelObject = props.modelObject();
	StandardOpaqueMaterial material2 = modelObject.cast<StandardOpaqueMaterial>();
	EXPECT_EQ(material, material2);
}

// check that parent remove works
TEST_F(ModelFixture, AdditionalProperties_ParentRemove) {
	Model model;
	auto size = model.modelObjects().size();
	StandardOpaqueMaterial material(model);
	AdditionalProperties props = material.additionalProperties();
	EXPECT_FALSE(material.remove().empty());
	EXPECT_EQ(size, model.modelObjects().size());
}

// check that child remove works
TEST_F(ModelFixture, AdditionalProperties_ChildRemove) {
  Model model;
	StandardOpaqueMaterial material(model);
	auto size = model.modelObjects().size();
	AdditionalProperties props = material.additionalProperties();
	EXPECT_FALSE(props.remove().empty());
	EXPECT_EQ(size, model.modelObjects().size());
}

// test that parent clone works
TEST_F(ModelFixture, AdditionalProperties_ParentClone) {
	Model model;
	StandardOpaqueMaterial material(model);
	AdditionalProperties props = material.additionalProperties();
	EXPECT_EQ(2, model.modelObjects().size());
	StandardOpaqueMaterial materialClone = material.clone(model).cast<StandardOpaqueMaterial>();
	EXPECT_EQ(4, model.modelObjects().size());
}

// test that child clone does not work
TEST_F(ModelFixture, AdditionalProperties_ChildClone) {
	Model model;
	StandardOpaqueMaterial material(model);
	AdditionalProperties props = material.additionalProperties();
	EXPECT_EQ(2, model.modelObjects().size());
	AdditionalProperties propsClone = props.clone(model).cast<AdditionalProperties>();
	EXPECT_EQ(2, model.modelObjects().size());
}