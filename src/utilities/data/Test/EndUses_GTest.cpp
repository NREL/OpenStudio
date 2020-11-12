/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    std::string units = endUses.getUnitsForFuelType(fuelType);
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
