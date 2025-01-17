/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "DataFixture.hpp"

#include "../EndUses.hpp"

using namespace openstudio;

TEST_F(DataFixture, EndUses) {

  openstudio::path xmlPath = toPath("./report.xml");
  if (openstudio::filesystem::exists(xmlPath)) {
    openstudio::filesystem::remove(xmlPath);
  }

  EndUses endUses;

  endUses.addEndUse(25.0, EndUseFuelType::Gas, EndUseCategoryType::Heating);
  endUses.addEndUse(2.0, EndUseFuelType::Electricity, EndUseCategoryType::Cooling);
  endUses.addEndUse(1.0, EndUseFuelType::Electricity, EndUseCategoryType::InteriorLights);
  endUses.addEndUse(10.0, EndUseFuelType::Electricity, EndUseCategoryType::Fans, "Fan Energy");
  endUses.addEndUse(10.0, EndUseFuelType::Water, EndUseCategoryType::Humidifier);

  EXPECT_EQ(25.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating, "General"));
  EXPECT_EQ(25.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating));
  EXPECT_EQ(25.0, endUses.getEndUseByFuelType(EndUseFuelType::Gas));
  EXPECT_EQ(25.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating));
  EXPECT_EQ(25.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating, "General"));
  EXPECT_EQ(2.0, endUses.getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Cooling, "General"));
  EXPECT_EQ(2.0, endUses.getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Cooling));
  EXPECT_EQ(13.0, endUses.getEndUseByFuelType(EndUseFuelType::Electricity));
  EXPECT_EQ(2.0, endUses.getEndUseByCategory(EndUseCategoryType::Cooling));
  EXPECT_EQ(2.0, endUses.getEndUseByCategory(EndUseCategoryType::Cooling, "General"));
  EXPECT_EQ(1.0, endUses.getEndUseByCategory(EndUseCategoryType::InteriorLights));
  EXPECT_EQ(1.0, endUses.getEndUseByCategory(EndUseCategoryType::InteriorLights, "General"));
  EXPECT_EQ(10.0, endUses.getEndUseByCategory(EndUseCategoryType::Fans));
  EXPECT_EQ(0.0, endUses.getEndUseByCategory(EndUseCategoryType::Fans, "General"));
  EXPECT_EQ(10.0, endUses.getEndUseByCategory(EndUseCategoryType::Fans, "Fan Energy"));

  // save to xml
  endUses.attribute().saveToXml(xmlPath);

  // load
  boost::optional<Attribute> testAttribute = Attribute::loadFromXml(xmlPath);
  ASSERT_TRUE(testAttribute);
  boost::optional<EndUses> testEndUses = EndUses::fromAttribute(*testAttribute);
  ASSERT_TRUE(testEndUses);

  EXPECT_EQ(25.0, testEndUses->getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating, "General"));
  EXPECT_EQ(25.0, testEndUses->getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating));
  EXPECT_EQ(25.0, testEndUses->getEndUseByFuelType(EndUseFuelType::Gas));
  EXPECT_EQ(25.0, testEndUses->getEndUseByCategory(EndUseCategoryType::Heating));
  EXPECT_EQ(25.0, testEndUses->getEndUseByCategory(EndUseCategoryType::Heating, "General"));
  EXPECT_EQ(2.0, testEndUses->getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Cooling, "General"));
  EXPECT_EQ(2.0, testEndUses->getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Cooling));
  EXPECT_EQ(13.0, testEndUses->getEndUseByFuelType(EndUseFuelType::Electricity));
  EXPECT_EQ(2.0, testEndUses->getEndUseByCategory(EndUseCategoryType::Cooling));
  EXPECT_EQ(2.0, testEndUses->getEndUseByCategory(EndUseCategoryType::Cooling, "General"));
  EXPECT_EQ(1.0, testEndUses->getEndUseByCategory(EndUseCategoryType::InteriorLights));
  EXPECT_EQ(1.0, testEndUses->getEndUseByCategory(EndUseCategoryType::InteriorLights, "General"));
  EXPECT_EQ(10.0, testEndUses->getEndUseByCategory(EndUseCategoryType::Fans));
  EXPECT_EQ(0.0, testEndUses->getEndUseByCategory(EndUseCategoryType::Fans, "General"));
  EXPECT_EQ(10.0, testEndUses->getEndUseByCategory(EndUseCategoryType::Fans, "Fan Energy"));

  // this is how you might make a chart out of this
  std::vector<EndUseFuelType> allFuelTypes = endUses.fuelTypes();
  std::vector<EndUseCategoryType> allCategories = endUses.categories();
  std::vector<std::string> subCategories = endUses.subCategories();

  EXPECT_TRUE(3 < allFuelTypes.size());
  EXPECT_TRUE(2 < allCategories.size());
  EXPECT_EQ(static_cast<unsigned>(2), subCategories.size());

  // figure out which fuel types are used
  std::vector<EndUseFuelType> fuelTypes;
  for (const EndUseFuelType& fuelType : allFuelTypes) {
    if (endUses.getEndUseByFuelType(fuelType) != 0) {
      fuelTypes.push_back(fuelType);
    }
  }

  // figure out which categories are used
  std::vector<EndUseCategoryType> categories;
  for (const EndUseCategoryType& category : allCategories) {
    if (endUses.getEndUseByCategory(category) != 0) {
      categories.push_back(category);
    }
  }

  EXPECT_EQ(static_cast<unsigned>(3), fuelTypes.size());
  EXPECT_EQ(static_cast<unsigned>(5), categories.size());
  EXPECT_EQ(static_cast<unsigned>(2), subCategories.size());

  for (const EndUseFuelType& fuelType : fuelTypes) {
    // std::string units = endUses.getUnitsForFuelType(fuelType);
    for (const EndUseCategoryType& category : categories) {
      for (const std::string& subCategory : subCategories) {
        /* double value =*/endUses.getEndUse(fuelType, category, subCategory);
        // barchart(value, units) ??
      }
    }
  }
}

TEST_F(DataFixture, EndUses2) {

  EndUses endUses;

  EXPECT_EQ(0.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating));
  EXPECT_EQ(0.0, endUses.getEndUseByFuelType(EndUseFuelType::Gas));
  EXPECT_EQ(0.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating));
  EXPECT_EQ(0.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating, "General"));
  EXPECT_EQ(0.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating, "Outdoor"));
  EXPECT_EQ(0.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating, "Not a sub category"));
  EXPECT_EQ(0.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating, "General"));
  EXPECT_EQ(0.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating, "Outdoor"));
  EXPECT_EQ(0.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating, "Not a sub category"));

  EXPECT_EQ(0.0, endUses.getEndUseByFuelType(EndUseFuelType::Electricity));
  EXPECT_EQ(0.0, endUses.getEndUseByCategory(EndUseCategoryType::Cooling));
  EXPECT_EQ(0.0, endUses.getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Heating, "General"));
  EXPECT_EQ(0.0, endUses.getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Heating, "Outdoor"));
  EXPECT_EQ(0.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Cooling, "General"));
  EXPECT_EQ(0.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Cooling, "Outdoor"));

  endUses.addEndUse(5.0, EndUseFuelType::Gas, EndUseCategoryType::Heating);

  EXPECT_EQ(5.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating));
  EXPECT_EQ(5.0, endUses.getEndUseByFuelType(EndUseFuelType::Gas));
  EXPECT_EQ(5.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating));
  EXPECT_EQ(5.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating, "General"));
  EXPECT_EQ(0.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating, "Outdoor"));
  EXPECT_EQ(0.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating, "Not a sub category"));
  EXPECT_EQ(5.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating, "General"));
  EXPECT_EQ(0.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating, "Outdoor"));
  EXPECT_EQ(0.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating, "Not a sub category"));

  endUses.addEndUse(5.0, EndUseFuelType::Gas, EndUseCategoryType::Heating);

  EXPECT_EQ(10.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating));
  EXPECT_EQ(10.0, endUses.getEndUseByFuelType(EndUseFuelType::Gas));
  EXPECT_EQ(10.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating));
  EXPECT_EQ(10.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating, "General"));
  EXPECT_EQ(0.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating, "Outdoor"));
  EXPECT_EQ(0.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating, "Not a sub category"));
  EXPECT_EQ(10.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating, "General"));
  EXPECT_EQ(0.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating, "Outdoor"));
  EXPECT_EQ(0.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating, "Not a sub category"));

  endUses.addEndUse(5.0, EndUseFuelType::Gas, EndUseCategoryType::Heating, "Outdoor");

  EXPECT_EQ(15.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating));
  EXPECT_EQ(15.0, endUses.getEndUseByFuelType(EndUseFuelType::Gas));
  EXPECT_EQ(15.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating));
  EXPECT_EQ(10.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating, "General"));
  EXPECT_EQ(5.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating, "Outdoor"));
  EXPECT_EQ(0.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating, "Not a sub category"));
  EXPECT_EQ(10.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating, "General"));
  EXPECT_EQ(5.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating, "Outdoor"));
  EXPECT_EQ(0.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating, "Not a sub category"));

  endUses.addEndUse(5.0, EndUseFuelType::Gas, EndUseCategoryType::Heating, "Outdoor");

  EXPECT_EQ(20.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating));
  EXPECT_EQ(20.0, endUses.getEndUseByFuelType(EndUseFuelType::Gas));
  EXPECT_EQ(20.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating));
  EXPECT_EQ(10.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating, "General"));
  EXPECT_EQ(10.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating, "Outdoor"));
  EXPECT_EQ(0.0, endUses.getEndUseByCategory(EndUseCategoryType::Heating, "Not a sub category"));
  EXPECT_EQ(10.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating, "General"));
  EXPECT_EQ(10.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating, "Outdoor"));
  EXPECT_EQ(0.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Heating, "Not a sub category"));

  EXPECT_EQ(0.0, endUses.getEndUseByFuelType(EndUseFuelType::Electricity));
  EXPECT_EQ(0.0, endUses.getEndUseByCategory(EndUseCategoryType::Cooling));
  EXPECT_EQ(0.0, endUses.getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Heating, "General"));
  EXPECT_EQ(0.0, endUses.getEndUse(EndUseFuelType::Electricity, EndUseCategoryType::Heating, "Outdoor"));
  EXPECT_EQ(0.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Cooling, "General"));
  EXPECT_EQ(0.0, endUses.getEndUse(EndUseFuelType::Gas, EndUseCategoryType::Cooling, "Outdoor"));
}

TEST_F(DataFixture, EndUses3) {
  // EndUseCategoryType is a subset of EndUseType
  for (int i : EndUseCategoryType::getValues()) {
    EndUseCategoryType endUseCategoryType(i);
    EXPECT_NO_THROW(EndUseType(endUseCategoryType.valueName())) << endUseCategoryType.valueName();
  }
}

TEST_F(DataFixture, EndUses_Conversions) {

  openstudio::path xmlPath = toPath("./report.xml");
  if (openstudio::filesystem::exists(xmlPath)) {
    openstudio::filesystem::remove(xmlPath);
  }

  EndUses endUses;
  Attribute attribute1("NotEndUses", 1.0);
  Attribute attribute2("EndUses", 1.0);

  endUses.attribute().saveToXml(xmlPath);
  boost::optional<Attribute> testAttribute = Attribute::loadFromXml(xmlPath);
  ASSERT_TRUE(testAttribute);

  EXPECT_TRUE(EndUses::fromAttribute(endUses.attribute()));
  EXPECT_FALSE(EndUses::fromAttribute(attribute1));
  EXPECT_FALSE(EndUses::fromAttribute(attribute2));
  EXPECT_TRUE(EndUses::fromAttribute(*testAttribute));
}
