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
#include "../TimeDependentValuationFile.hpp"

#include "../../units/Quantity.hpp"
#include "../../units/BTUUnit.hpp"
#include "../../units/QuantityConverter.hpp"

#include "../../core/Containers.hpp"
#include "../../core/Optional.hpp"

#include <resources.hxx>

using namespace openstudio;

void testCostOfEnergy(boost::optional<double> retrievedValue,double valueInDollarsPerKBtu) {
  ASSERT_TRUE(retrievedValue);
  Quantity originalQuantity(valueInDollarsPerKBtu,BTUUnit(BTUExpnt(-1),-3));
  EXPECT_EQ("1/kBtu",originalQuantity.standardUnitsString());
  Quantity siQuantity = QuantityConverter::instance().convert(originalQuantity,
                                                              UnitSystem(UnitSystem::SI)).get();
  siQuantity.setScale(0);
  EXPECT_NEAR(siQuantity.value(),*retrievedValue,1.0E-6);
  EXPECT_EQ("s^2/kg*m^2",siQuantity.standardUnitsString());
  EXPECT_EQ("1/J",siQuantity.prettyUnitsString());
}

TEST(Filetypes, TimeDependentValuationFile) {
  openstudio::path p = resourcesPath()/toPath("utilities/Filetypes/TDV_2008_kBtu_CZ13.csv");

  OptionalTimeDependentValuationFile oTdvFile = TimeDependentValuationFile::load(p);
  ASSERT_TRUE(oTdvFile);
  TimeDependentValuationFile tdvFile = *oTdvFile;

  EXPECT_EQ(6, tdvFile.numColumns());

  EXPECT_EQ(p,tdvFile.path());
  EXPECT_EQ("Climate zone 13 with externalities",tdvFile.name());
  EXPECT_EQ(std::string("Created April 18, 2006"),tdvFile.description());
  {
    SCOPED_TRACE("Commercial");
    testCostOfEnergy(tdvFile.nominalCommercialCostOfEnergy(),0.145972);
  }
  {
    SCOPED_TRACE("Residential");
    testCostOfEnergy(tdvFile.nominalResidentialCostOfEnergy(),0.164171);
  }
  EXPECT_FALSE(tdvFile.checksum().empty());
  StringVector unitStrings;
  unitStrings.push_back("kBtu/kWh");
  unitStrings.push_back("kBtu/therm");
  for (unsigned i = 0, n = tdvFile.numColumns(); i < n; ++i) {
    EXPECT_FALSE(std::find(unitStrings.begin(),unitStrings.end(),tdvFile.units(i)) == unitStrings.end());
    EXPECT_EQ(8760u,tdvFile.values(i).size());
    EXPECT_EQ(8760u,tdvFile.quantities(i).size());
  }

  oTdvFile = tdvFile.convertUnits();
  ASSERT_TRUE(oTdvFile);
  EXPECT_TRUE(oTdvFile->path().empty());
  p = resourcesPath()/toPath("utilities/Filetypes/TDV_2008_kBtu_CZ13_SI.csv");
  EXPECT_TRUE(oTdvFile->save(p,true));
  EXPECT_EQ(p,oTdvFile->path());

  oTdvFile = TimeDependentValuationFile::load(p);
  ASSERT_TRUE(oTdvFile);
}
