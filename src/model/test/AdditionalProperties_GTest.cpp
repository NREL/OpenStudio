/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
#include "../Lights.hpp"
#include "../LightsDefinition.hpp"

#include <utilities/idd/OS_AdditionalProperties_FieldEnums.hxx>

#include "../../utilities/geometry/Geometry.hpp"

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
    ::testing::ExitedWithCode(0), "");

  // create a model to use
  Model model;

  // create a material object to use
  StandardOpaqueMaterial material(model);
  EXPECT_EQ(1, model.modelObjects().size());

  // create an additional properties object to use
  AdditionalProperties props = material.additionalProperties();
  EXPECT_EQ(2, model.modelObjects().size());
  EXPECT_EQ(1, model.getConcreteModelObjects<AdditionalProperties>().size());

  // return, instead of create, additional properies when it already exists
  AdditionalProperties props2 = material.additionalProperties();
  EXPECT_EQ(2, model.modelObjects().size());
  EXPECT_EQ(1, model.getConcreteModelObjects<AdditionalProperties>().size());
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
  ASSERT_DOUBLE_EQ(*dblValue, 1.5);

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
  std::vector<std::string> suggestedFeatureNames(props2.suggestedFeatureNames());
  ASSERT_EQ(suggestedFeatureNames.size(), 3);
  ASSERT_NE(std::find(suggestedFeatureNames.begin(), suggestedFeatureNames.end(), "MyUniqueFeature"), suggestedFeatureNames.end());
}

// check returning model object pointed to
TEST_F(ModelFixture, AdditionalProperties_ModelObject) {
  Model model;
  StandardOpaqueMaterial material(model);
  EXPECT_EQ(0, model.getConcreteModelObjects<AdditionalProperties>().size());
  AdditionalProperties props = material.additionalProperties();
  EXPECT_EQ(1u, model.getConcreteModelObjects<AdditionalProperties>().size());
  AdditionalProperties props2 = material.additionalProperties();
  EXPECT_EQ(1u, model.getConcreteModelObjects<AdditionalProperties>().size());
  EXPECT_EQ(props.handle(), props2.handle());

  ModelObject modelObject = props.modelObject();
  auto material2 = modelObject.cast<StandardOpaqueMaterial>();
  EXPECT_EQ(material, material2);
}

// check that remove works on ParentObject and ModelObject derived classes
TEST_F(ModelFixture, AdditionalProperties_NonParentRemove) {
  Model model;
  auto size = model.modelObjects().size();
  LightsDefinition def(model);
  AdditionalProperties defProps = def.additionalProperties();
  Lights lights1(def);
  AdditionalProperties props1 = lights1.additionalProperties();
  Lights lights2(def);
  AdditionalProperties props2 = lights2.additionalProperties();
  EXPECT_EQ(2u, lights2.remove().size());
  EXPECT_FALSE(defProps.handle().isNull());
  EXPECT_FALSE(props1.handle().isNull());
  EXPECT_TRUE(props2.handle().isNull());

  EXPECT_EQ(4u, def.remove().size());
  EXPECT_TRUE(defProps.handle().isNull());
  EXPECT_TRUE(props1.handle().isNull());
  EXPECT_TRUE(props2.handle().isNull());

  EXPECT_EQ(size, model.modelObjects().size()) << model;
}

// check that remove works on the object
TEST_F(ModelFixture, AdditionalProperties_ParentRemove) {
  Model model;
  EXPECT_EQ(0u, model.getConcreteModelObjects<AdditionalProperties>().size());
  auto size = model.modelObjects().size();
  StandardOpaqueMaterial material(model);
  AdditionalProperties props = material.additionalProperties();
  EXPECT_EQ(1u, model.getConcreteModelObjects<AdditionalProperties>().size());
  EXPECT_EQ(2u, material.remove().size());
  EXPECT_EQ(size, model.modelObjects().size());
  EXPECT_EQ(0u, model.getConcreteModelObjects<AdditionalProperties>().size());
}

// check that remove works on the props
TEST_F(ModelFixture, AdditionalProperties_ChildRemove) {
  Model model;
  EXPECT_EQ(0u, model.getConcreteModelObjects<AdditionalProperties>().size());
  StandardOpaqueMaterial material(model);
  auto size = model.modelObjects().size();
  AdditionalProperties props = material.additionalProperties();
  EXPECT_EQ(1u, model.getConcreteModelObjects<AdditionalProperties>().size());
  EXPECT_FALSE(props.remove().empty());
  EXPECT_EQ(size, model.modelObjects().size());
  EXPECT_EQ(0u, model.getConcreteModelObjects<AdditionalProperties>().size());
}

// test that parent clone works
TEST_F(ModelFixture, AdditionalProperties_ParentClone) {
  Model model;
  EXPECT_EQ(0u, model.getConcreteModelObjects<AdditionalProperties>().size());
  StandardOpaqueMaterial material(model);
  AdditionalProperties props = material.additionalProperties();
  EXPECT_EQ(1u, model.getConcreteModelObjects<AdditionalProperties>().size());
  EXPECT_EQ(2, model.modelObjects().size());
  auto materialClone = material.clone(model).cast<StandardOpaqueMaterial>();
  EXPECT_EQ(4, model.modelObjects().size());
  EXPECT_EQ(2u, model.getConcreteModelObjects<AdditionalProperties>().size());
  EXPECT_NE(material.additionalProperties().handle(), materialClone.additionalProperties().handle());
  EXPECT_EQ(4, model.modelObjects().size());
  EXPECT_EQ(2u, model.getConcreteModelObjects<AdditionalProperties>().size());

  Model model2;
  auto materialClone2 = material.clone(model2).cast<StandardOpaqueMaterial>();
  EXPECT_EQ(2, model2.modelObjects().size());
  EXPECT_EQ(1u, model2.getConcreteModelObjects<AdditionalProperties>().size());
  EXPECT_NE(material.additionalProperties().handle(), materialClone2.additionalProperties().handle());
  EXPECT_EQ(2, model2.modelObjects().size());
  EXPECT_EQ(1u, model2.getConcreteModelObjects<AdditionalProperties>().size());
}

// test that child clone does not work
TEST_F(ModelFixture, AdditionalProperties_ChildClone) {
  Model model;
  EXPECT_EQ(0u, model.getConcreteModelObjects<AdditionalProperties>().size());
  StandardOpaqueMaterial material(model);
  AdditionalProperties props = material.additionalProperties();
  EXPECT_EQ(1u, model.getConcreteModelObjects<AdditionalProperties>().size());
  EXPECT_EQ(2, model.modelObjects().size());

  EXPECT_EQ(0, props.resources().size());

  // DLM: do we want to allow this clone?
  auto propsClone = props.clone(model).cast<AdditionalProperties>();
  EXPECT_EQ(3, model.modelObjects().size());
  EXPECT_EQ(2u, model.getConcreteModelObjects<AdditionalProperties>().size());
  EXPECT_EQ(props.modelObject().handle(), propsClone.modelObject().handle());

  Model model2;

  // DLM: do we want to allow this clone?
  auto propsClone2 = props.clone(model2).cast<AdditionalProperties>();
  EXPECT_EQ(1u, model2.modelObjects().size());
  EXPECT_EQ(1u, model2.getConcreteModelObjects<AdditionalProperties>().size());

  // DLM: no way to clean up this invalid object?
  EXPECT_TRUE(propsClone2.isEmpty(OS_AdditionalPropertiesFields::ObjectName));
  EXPECT_THROW(propsClone2.modelObject(), openstudio::Exception);

  // go back to first model

  // calling additionalProperties again will remove one of the props,
  // no telling which one, so do this test last
  material.additionalProperties();
  EXPECT_EQ(1u, model.getConcreteModelObjects<AdditionalProperties>().size());
}

// check that can't add properties to a properties
TEST_F(ModelFixture, AdditionalProperties_AdditionalProperties2) {
  Model model;
  EXPECT_EQ(0u, model.getConcreteModelObjects<AdditionalProperties>().size());
  // auto size = model.modelObjects().size();
  StandardOpaqueMaterial material(model);
  AdditionalProperties props = material.additionalProperties();
  EXPECT_EQ(1u, model.getConcreteModelObjects<AdditionalProperties>().size());
  EXPECT_THROW(props.additionalProperties(), openstudio::Exception);
  EXPECT_EQ(1u, model.getConcreteModelObjects<AdditionalProperties>().size());
}

// test that setters work
TEST_F(ModelFixture, AdditionalProperties_Setters) {
  Model model;
  EXPECT_EQ(0u, model.getConcreteModelObjects<AdditionalProperties>().size());
  StandardOpaqueMaterial material(model);
  AdditionalProperties props = material.additionalProperties();
  EXPECT_EQ(1u, model.getConcreteModelObjects<AdditionalProperties>().size());

  EXPECT_EQ(0, props.featureNames().size());
  EXPECT_FALSE(props.getFeatureDataType("Feature"));

  EXPECT_TRUE(props.setFeature("Feature", 1.0));
  ASSERT_EQ(1u, props.featureNames().size());
  EXPECT_EQ("Feature", props.featureNames()[0]);
  ASSERT_TRUE(props.getFeatureDataType("Feature"));
  EXPECT_EQ("Double", props.getFeatureDataType("Feature").get());
  ASSERT_TRUE(props.getFeatureAsDouble("Feature"));
  EXPECT_EQ(1.0, props.getFeatureAsDouble("Feature").get());

  EXPECT_TRUE(props.setFeature("Feature", 2.0));
  ASSERT_EQ(1u, props.featureNames().size());
  EXPECT_EQ("Feature", props.featureNames()[0]);
  ASSERT_TRUE(props.getFeatureDataType("Feature"));
  EXPECT_EQ("Double", props.getFeatureDataType("Feature").get());
  ASSERT_TRUE(props.getFeatureAsDouble("Feature"));
  EXPECT_EQ(2.0, props.getFeatureAsDouble("Feature").get());

  EXPECT_TRUE(props.setFeature("Feature2", -2.0));
  ASSERT_EQ(2u, props.featureNames().size());
  EXPECT_EQ("Feature", props.featureNames()[0]);
  EXPECT_EQ("Feature2", props.featureNames()[1]);
  ASSERT_TRUE(props.getFeatureDataType("Feature2"));
  EXPECT_EQ("Double", props.getFeatureDataType("Feature2").get());
  ASSERT_TRUE(props.getFeatureAsDouble("Feature2"));
  EXPECT_EQ(-2.0, props.getFeatureAsDouble("Feature2").get());

  EXPECT_TRUE(props.setFeature("Feature", "Yay"));
  ASSERT_EQ(2u, props.featureNames().size());
  EXPECT_EQ("Feature", props.featureNames()[0]);
  EXPECT_EQ("Feature2", props.featureNames()[1]);
  ASSERT_TRUE(props.getFeatureDataType("Feature"));
  EXPECT_EQ("String", props.getFeatureDataType("Feature").get());
  ASSERT_TRUE(props.getFeatureAsString("Feature"));
  EXPECT_EQ("Yay", props.getFeatureAsString("Feature").get());

  EXPECT_TRUE(props.resetFeature("Feature"));
  ASSERT_EQ(1u, props.featureNames().size());
  EXPECT_EQ("Feature2", props.featureNames()[0]);
  ASSERT_TRUE(props.getFeatureDataType("Feature2"));
  EXPECT_EQ("Double", props.getFeatureDataType("Feature2").get());
  ASSERT_TRUE(props.getFeatureAsDouble("Feature2"));
  EXPECT_EQ(-2.0, props.getFeatureAsDouble("Feature2").get());
}
