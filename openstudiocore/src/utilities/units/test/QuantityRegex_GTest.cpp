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
#include "UnitsFixture.hpp"

#include "../QuantityRegex.hpp"

#include "../../core/Exception.hpp"

using openstudio::isFixedPrecisionValue;
using openstudio::containsFixedPrecisionValue;
using openstudio::isScientificNotationValue;
using openstudio::containsScientificNotationValue;
using openstudio::isAtomicUnit;
using openstudio::containsAtomicUnit;
using openstudio::isCompoundUnit;
using openstudio::containsCompoundUnit;
using openstudio::isScaledUnit;
using openstudio::containsScaledUnit;
using openstudio::regexEmbeddedDirectScaledUnit;
using openstudio::isDirectScaledUnit;
using openstudio::containsDirectScaledUnit;
using openstudio::decomposeDirectScaledUnit;
using openstudio::isUnit;
using openstudio::containsUnit;
using openstudio::isQuantity;
using openstudio::containsQuantity;
using openstudio::decomposeQuantityString;
using openstudio::decomposeScaledUnitString;
using openstudio::decomposeCompoundUnitString;
using openstudio::decomposeAtomicUnitString;

TEST_F(UnitsFixture,QuantityRegex_Values) {

  // strings that should be FixedPrecisionValues
  std::string fixedValue("0.321"); 
  EXPECT_TRUE(isFixedPrecisionValue(fixedValue));
  fixedValue = "20"; EXPECT_TRUE(isFixedPrecisionValue(fixedValue));
  fixedValue = ".3120"; EXPECT_TRUE(isFixedPrecisionValue(fixedValue));
  fixedValue = "1020.185961"; EXPECT_TRUE(isFixedPrecisionValue(fixedValue));
  fixedValue = ".1"; EXPECT_TRUE(isFixedPrecisionValue(fixedValue));
  fixedValue = "3.1"; EXPECT_TRUE(isFixedPrecisionValue(fixedValue));
  fixedValue = "-0.12"; EXPECT_TRUE(isFixedPrecisionValue(fixedValue));

  // strings that should not be FixedPrecisionValues
  std::string notFixedValue("0 ");
  EXPECT_FALSE(isFixedPrecisionValue(notFixedValue));
  notFixedValue = "."; EXPECT_FALSE(isFixedPrecisionValue(notFixedValue));
  notFixedValue = "1.1 "; EXPECT_FALSE(isFixedPrecisionValue(notFixedValue));
  notFixedValue = "1.0E000"; EXPECT_FALSE(isFixedPrecisionValue(notFixedValue));
  notFixedValue = ".3D-32"; EXPECT_FALSE(isFixedPrecisionValue(notFixedValue));
  notFixedValue = "5 N"; EXPECT_FALSE(isFixedPrecisionValue(notFixedValue));
  notFixedValue = "5,000"; EXPECT_FALSE(isFixedPrecisionValue(notFixedValue));
  notFixedValue = "5000."; EXPECT_FALSE(isFixedPrecisionValue(notFixedValue));

  // strings that should contain FixedPrecisionValues
  std::string containsFixedValue("5 ");
  EXPECT_TRUE(containsFixedPrecisionValue(containsFixedValue));
  containsFixedValue = "23856.21865 kN*m/s^2 more text"; EXPECT_TRUE(containsFixedPrecisionValue(containsFixedValue));
  containsFixedValue = ".1 J"; EXPECT_TRUE(containsFixedPrecisionValue(containsFixedValue));
  containsFixedValue = "32 farkles"; EXPECT_TRUE(containsFixedPrecisionValue(containsFixedValue));
  containsFixedValue = "text from an article. 532 elephants."; EXPECT_TRUE(containsFixedPrecisionValue(containsFixedValue));
  containsFixedValue = "0"; EXPECT_TRUE(containsFixedPrecisionValue(containsFixedValue));
  containsFixedValue = "-3200.1"; EXPECT_TRUE(containsFixedPrecisionValue(containsFixedValue));
  containsFixedValue = "The number of elements is 532."; EXPECT_TRUE(containsFixedPrecisionValue(containsFixedValue));
  containsFixedValue = "321.225;-32.2896"; EXPECT_TRUE(containsFixedPrecisionValue(containsFixedValue));
  containsFixedValue = "Some text 321.25."; EXPECT_TRUE(containsFixedPrecisionValue(containsFixedValue));
 
  // strings that should not contain FixedPrecisionValues
  std::string doesNotContainFixedValue("5.0E32");
  EXPECT_FALSE(containsFixedPrecisionValue(doesNotContainFixedValue));
  doesNotContainFixedValue = "32kN"; EXPECT_FALSE(containsFixedPrecisionValue(doesNotContainFixedValue));
  doesNotContainFixedValue = "1.0E2 Btu/h"; EXPECT_FALSE(containsFixedPrecisionValue(doesNotContainFixedValue));
  doesNotContainFixedValue = "myFunction_32"; EXPECT_FALSE(containsFixedPrecisionValue(doesNotContainFixedValue));
  doesNotContainFixedValue = "12.Text"; EXPECT_FALSE(containsFixedPrecisionValue(doesNotContainFixedValue));


  // should be ScientificNotationValues
  std::string scientificValue("1.0E0");
  EXPECT_TRUE(isScientificNotationValue(scientificValue));
  scientificValue = "1.0D+000"; EXPECT_TRUE(isScientificNotationValue(scientificValue));
  scientificValue = "32E1"; EXPECT_TRUE(isScientificNotationValue(scientificValue));
  scientificValue = ".3298D-3"; EXPECT_TRUE(isScientificNotationValue(scientificValue));
  scientificValue = "3.2E13"; EXPECT_TRUE(isScientificNotationValue(scientificValue));
  scientificValue = "0.111198766D+010"; EXPECT_TRUE(isScientificNotationValue(scientificValue));
  scientificValue = "1867.201E-001"; EXPECT_TRUE(isScientificNotationValue(scientificValue));
  scientificValue = "-3.2E-002"; EXPECT_TRUE(isScientificNotationValue(scientificValue));
  scientificValue = "-.01E3"; EXPECT_TRUE(isScientificNotationValue(scientificValue));
  scientificValue = "2.0e+002"; EXPECT_TRUE(isScientificNotationValue(scientificValue));
  scientificValue = "-1.298619d-003"; EXPECT_TRUE(isScientificNotationValue(scientificValue));

  // should not be ScientificNotationValues
  std::string notSciValue("2.1");
  EXPECT_FALSE(isScientificNotationValue(notSciValue));
  notSciValue = "text"; EXPECT_FALSE(isScientificNotationValue(notSciValue));
  notSciValue = "3.1E-1 "; EXPECT_FALSE(isScientificNotationValue(notSciValue));
  notSciValue = "s3.2D+000"; EXPECT_FALSE(isScientificNotationValue(notSciValue));
  notSciValue = ".1.E+01"; EXPECT_FALSE(isScientificNotationValue(notSciValue));
  notSciValue = "hi 1.2D0"; EXPECT_FALSE(isScientificNotationValue(notSciValue));
  notSciValue = ".E-1"; EXPECT_FALSE(isScientificNotationValue(notSciValue));

  // should contain ScientificNotationValues
  std::string containsSciValue("3.1E-1 ");
  EXPECT_TRUE(containsScientificNotationValue(containsSciValue));
  containsSciValue = "0E0 J"; EXPECT_TRUE(containsScientificNotationValue(containsSciValue));
  containsSciValue = "500D-3 m^2/s"; EXPECT_TRUE(containsScientificNotationValue(containsSciValue));
  containsSciValue = " 12657.0E+0001 kBtu/ft^2"; EXPECT_TRUE(containsScientificNotationValue(containsSciValue));
  containsSciValue = ".1E-2 text"; EXPECT_TRUE(containsScientificNotationValue(containsSciValue));
  containsSciValue = "1.0E-3; 2.1D+001"; EXPECT_TRUE(containsScientificNotationValue(containsSciValue));
 
  // should not contain ScientificNotationValues
  std::string doesNotContainSciValue("some text");
  EXPECT_FALSE(containsScientificNotationValue(doesNotContainSciValue));
  doesNotContainSciValue = "3 N"; EXPECT_FALSE(containsScientificNotationValue(doesNotContainSciValue));
  doesNotContainSciValue = "3.0D01kN"; EXPECT_FALSE(containsScientificNotationValue(doesNotContainSciValue));
  doesNotContainSciValue = "fruity5.0E-3"; EXPECT_FALSE(containsScientificNotationValue(doesNotContainSciValue));
  doesNotContainSciValue = "186967.19867"; EXPECT_FALSE(containsScientificNotationValue(doesNotContainSciValue));

}

TEST_F(UnitsFixture,QuantityRegex_Units) {

  // should be AtomicUnits
  std::string atomicUnit("kg");
  EXPECT_TRUE(isAtomicUnit(atomicUnit));
  atomicUnit = "MJ"; EXPECT_TRUE(isAtomicUnit(atomicUnit));
  atomicUnit = "kBtu^3"; EXPECT_TRUE(isAtomicUnit(atomicUnit));
  atomicUnit = "s^{-1}"; EXPECT_TRUE(isAtomicUnit(atomicUnit));
  atomicUnit = "ms^5"; EXPECT_TRUE(isAtomicUnit(atomicUnit));
  atomicUnit = "aioranb^32"; EXPECT_TRUE(isAtomicUnit(atomicUnit));
  atomicUnit = "\\muN"; EXPECT_TRUE(isAtomicUnit(atomicUnit));
  atomicUnit = "lb_f^2"; EXPECT_TRUE(isAtomicUnit(atomicUnit));

  // should not be AtomicUnits
  std::string notAtomicUnit("m/s");
  EXPECT_FALSE(isAtomicUnit(notAtomicUnit));
  notAtomicUnit = " m "; EXPECT_FALSE(isAtomicUnit(notAtomicUnit));
  notAtomicUnit = "kBtu/h"; EXPECT_FALSE(isAtomicUnit(notAtomicUnit));
  notAtomicUnit = "kg*m"; EXPECT_FALSE(isAtomicUnit(notAtomicUnit));
  notAtomicUnit = "3 s"; EXPECT_FALSE(isAtomicUnit(notAtomicUnit));
  notAtomicUnit = "kN*m"; EXPECT_FALSE(isAtomicUnit(notAtomicUnit));
  notAtomicUnit = "hippopotamus^3"; EXPECT_FALSE(isAtomicUnit(notAtomicUnit));
  notAtomicUnit = "ft^2,"; EXPECT_FALSE(isAtomicUnit(notAtomicUnit));

  // should contain AtomicUnits
  std::string includesUnit("kg*m");
  EXPECT_TRUE(containsAtomicUnit(includesUnit));
  includesUnit = "short words"; EXPECT_TRUE(containsAtomicUnit(includesUnit));
  includesUnit = "kg^2/ms"; EXPECT_TRUE(containsAtomicUnit(includesUnit));
  includesUnit = "hello s^{-2}"; EXPECT_TRUE(containsAtomicUnit(includesUnit));

  // should not contain AtomicUnits
  std::string doesNotContainUnit("kg^^3");
  EXPECT_FALSE(containsAtomicUnit(doesNotContainUnit));
  doesNotContainUnit = "3"; EXPECT_FALSE(containsAtomicUnit(doesNotContainUnit));
  doesNotContainUnit = "m^-3"; EXPECT_FALSE(containsAtomicUnit(doesNotContainUnit));
  doesNotContainUnit = "*2/"; EXPECT_FALSE(containsAtomicUnit(doesNotContainUnit));

  // compoundUnits
  std::string compoundUnit("kg/s");
  EXPECT_TRUE(isCompoundUnit(compoundUnit));
  compoundUnit = "ft^2"; EXPECT_TRUE(isCompoundUnit(compoundUnit));
  compoundUnit = "1/s"; EXPECT_TRUE(isCompoundUnit(compoundUnit));
  compoundUnit = "1/s^2"; EXPECT_TRUE(isCompoundUnit(compoundUnit));
  compoundUnit = "kg*m^2/s^2"; EXPECT_TRUE(isCompoundUnit(compoundUnit));
  compoundUnit = "MJ^{-3}"; EXPECT_TRUE(isCompoundUnit(compoundUnit));
  compoundUnit = "s^{-1}/m^{-1}"; EXPECT_TRUE(isCompoundUnit(compoundUnit));

  // not compoundUnits
  std::string notCompoundUnit("k(kg/s)");
  EXPECT_FALSE(isCompoundUnit(notCompoundUnit));
  notCompoundUnit = " m/s^2"; EXPECT_FALSE(isCompoundUnit(notCompoundUnit));
  notCompoundUnit = "31"; EXPECT_FALSE(isCompoundUnit(notCompoundUnit));
  notCompoundUnit = "0.12 kg"; EXPECT_FALSE(isCompoundUnit(notCompoundUnit));
  notCompoundUnit = "hiccocampus"; EXPECT_FALSE(isCompoundUnit(notCompoundUnit));
  notCompoundUnit = "ft^^2"; EXPECT_FALSE(isCompoundUnit(notCompoundUnit));
  notCompoundUnit = "ft^{2}"; EXPECT_FALSE(isCompoundUnit(notCompoundUnit));
  notCompoundUnit = "ft/s,"; EXPECT_FALSE(isCompoundUnit(notCompoundUnit));

  // contains compountUnit
  std::string includesCompoundUnit("3 kg");
  EXPECT_TRUE(containsCompoundUnit(includesCompoundUnit));
  includesCompoundUnit = "3.0E-3 k(kg*m/s^2)"; EXPECT_TRUE(containsCompoundUnit(includesCompoundUnit));
  includesCompoundUnit = "MJ"; EXPECT_TRUE(containsCompoundUnit(includesCompoundUnit));
  includesCompoundUnit = "short words count, unfortunately"; EXPECT_TRUE(containsCompoundUnit(includesCompoundUnit));
  includesCompoundUnit = "1 1/s"; EXPECT_TRUE(containsCompoundUnit(includesCompoundUnit));
  includesCompoundUnit = "ft/s,"; EXPECT_TRUE(containsCompoundUnit(includesCompoundUnit));
  includesCompoundUnit = "J^2/s;"; EXPECT_TRUE(containsCompoundUnit(includesCompoundUnit));

  // does not contain compoundUnit
  std::string noCompoundUnitsHere("hippocampus");
  EXPECT_FALSE(containsCompoundUnit(noCompoundUnitsHere));
  noCompoundUnitsHere = "32,36819"; EXPECT_FALSE(containsCompoundUnit(noCompoundUnitsHere));
  noCompoundUnitsHere = "389ft1986"; EXPECT_FALSE(containsCompoundUnit(noCompoundUnitsHere));
  noCompoundUnitsHere = "ft^^2"; EXPECT_FALSE(containsCompoundUnit(noCompoundUnitsHere));
  noCompoundUnitsHere = "ft^{2}"; EXPECT_FALSE(containsCompoundUnit(noCompoundUnitsHere));


  // scaledUnit
  std::string scaledUnit("k(m)");
  EXPECT_TRUE(isScaledUnit(scaledUnit));
  scaledUnit = "M(kg^3)"; EXPECT_TRUE(isScaledUnit(scaledUnit));
  scaledUnit = "\\mu(ft^3/s^2)"; EXPECT_TRUE(isScaledUnit(scaledUnit));
  scaledUnit = "aaaaa(kg^3)"; EXPECT_TRUE(isScaledUnit(scaledUnit));
  scaledUnit = "M(1/s^2*K)"; EXPECT_TRUE(isScaledUnit(scaledUnit));

  // not a scaledUnit
  std::string notScaledUnit("m");
  EXPECT_FALSE(isScaledUnit(notScaledUnit));
  notScaledUnit = "1/s"; EXPECT_FALSE(isScaledUnit(notScaledUnit));
  notScaledUnit = "k(m/s"; EXPECT_FALSE(isScaledUnit(notScaledUnit));
  notScaledUnit = "\\mu\\mu(kg*m)"; EXPECT_FALSE(isScaledUnit(notScaledUnit));
  notScaledUnit = "km^2)"; EXPECT_FALSE(isScaledUnit(notScaledUnit));

  // contains a scaledUnit
  std::string includesScaledUnit("3 M(kg^3/s^2*K)");
  EXPECT_TRUE(containsScaledUnit(includesScaledUnit));
  includesScaledUnit = ".1D-38 k(m)"; EXPECT_TRUE(containsScaledUnit(includesScaledUnit));
  includesScaledUnit = "surrounding text 32 n(m^2/s) more text"; EXPECT_TRUE(containsScaledUnit(includesScaledUnit));
  includesScaledUnit = "32 \\mu(W/s^2), is a funny quantity"; EXPECT_TRUE(containsScaledUnit(includesScaledUnit));

  // does not contain a scaledUnit
  std::string noScaledUnitsHere("3 m");
  EXPECT_FALSE(containsScaledUnit(noScaledUnitsHere));
  noScaledUnitsHere = "kg*m^2/s^2"; EXPECT_FALSE(containsScaledUnit(noScaledUnitsHere));
  noScaledUnitsHere = "1200"; EXPECT_FALSE(containsScaledUnit(noScaledUnitsHere));
  noScaledUnitsHere = "1.0E-2 MJ"; EXPECT_FALSE(containsScaledUnit(noScaledUnitsHere));
  noScaledUnitsHere = "some text"; EXPECT_FALSE(containsScaledUnit(noScaledUnitsHere));


  // unit
  std::string aUnit("kg*m/s^2");
  EXPECT_TRUE(isUnit(aUnit));
  aUnit = "km/s"; EXPECT_TRUE(isUnit(aUnit));
  aUnit = "M(ft^3/s)"; EXPECT_TRUE(isUnit(aUnit));
  aUnit = "nJ"; EXPECT_TRUE(isUnit(aUnit));
  aUnit = "\\muN"; EXPECT_TRUE(isUnit(aUnit));
  aUnit = "s^{-3}/lb_m^{-2}*h"; EXPECT_TRUE(isUnit(aUnit));
  aUnit = "ft*m^2*K/K*lb_f"; EXPECT_TRUE(isUnit(aUnit));

  // not a unit
  std::string notAUnit("(ft/s)");
  EXPECT_FALSE(isUnit(notAUnit));
  notAUnit = " kN"; EXPECT_FALSE(isUnit(notAUnit));
  notAUnit = "m/s^2)"; EXPECT_FALSE(isUnit(notAUnit));
  notAUnit = "kW^^2"; EXPECT_FALSE(isUnit(notAUnit));
  notAUnit = "cm^-2"; EXPECT_FALSE(isUnit(notAUnit));
  notAUnit = "2 m/s"; EXPECT_FALSE(isUnit(notAUnit));
  notAUnit = "kg*2"; EXPECT_FALSE(isUnit(notAUnit));
  
  // contains a unit
  std::string includesAUnit("2 m");
  EXPECT_TRUE(containsUnit(includesAUnit));
  includesAUnit = "3.2E-2 k(kg^2/s)"; EXPECT_TRUE(containsUnit(includesAUnit));
  includesAUnit = " km/s "; EXPECT_TRUE(containsUnit(includesAUnit));
  includesAUnit = "short, hairy elephant"; EXPECT_TRUE(containsUnit(includesAUnit));
  includesAUnit = " 0.1 m \n 21.3 \\muN"; EXPECT_TRUE(containsUnit(includesAUnit));
  includesAUnit = "km/s; "; EXPECT_TRUE(containsUnit(includesAUnit));
  includesAUnit = "(ft/s) "; EXPECT_TRUE(containsUnit(includesAUnit));
  includesAUnit = "Height [ft]"; EXPECT_TRUE(containsUnit(includesAUnit));

  // does not contain a unit
  std::string noUnitHere("2 m/s)");
  EXPECT_FALSE(containsUnit(noUnitHere));
  noUnitHere = "kN**ps/J"; EXPECT_FALSE(containsUnit(noUnitHere));
  noUnitHere = "32 hippopotamuses"; EXPECT_FALSE(containsUnit(noUnitHere));
  noUnitHere = "2.0 mph//J"; EXPECT_FALSE(containsUnit(noUnitHere));
  noUnitHere = "2.0 m*2.0"; EXPECT_FALSE(containsUnit(noUnitHere));

}

TEST_F(UnitsFixture,QuantityRegex_Quantities) {

  // quantity
  std::string aQuantity("3 m");
  EXPECT_TRUE(isQuantity(aQuantity));
  aQuantity = ".1E-4 kJ*m/s"; EXPECT_TRUE(isQuantity(aQuantity));
  aQuantity = "0.321 \\mu(ft^{-2}/ms*PJ)"; EXPECT_TRUE(isQuantity(aQuantity));
  aQuantity = "19856D19867 kN/short"; EXPECT_TRUE(isQuantity(aQuantity));
  aQuantity = "141.21 kBtu/ft^2*yr"; EXPECT_TRUE(isQuantity(aQuantity));
  aQuantity = "1.1 MJ/s"; EXPECT_TRUE(isQuantity(aQuantity));
  aQuantity = "61 M(J)"; EXPECT_TRUE(isQuantity(aQuantity));

  // not a quantity
  std::string notAQuantity("3 m^^2");
  EXPECT_FALSE(isQuantity(notAQuantity));
  notAQuantity = "300. ft"; EXPECT_FALSE(isQuantity(notAQuantity));
  notAQuantity = "21E-2 ft^21/s)"; EXPECT_FALSE(isQuantity(notAQuantity));
  notAQuantity = " 300 m"; EXPECT_FALSE(isQuantity(notAQuantity));
  notAQuantity = "2 ft "; EXPECT_FALSE(isQuantity(notAQuantity));
  notAQuantity = "k300/s"; EXPECT_FALSE(isQuantity(notAQuantity));
  notAQuantity = "text only"; EXPECT_FALSE(isQuantity(notAQuantity));

  // contains a quantity
  std::string includesQuantity("The building has a floor area of 3000 ft^2.");
  EXPECT_TRUE(containsQuantity(includesQuantity));
  includesQuantity = "2.0E-4 kN"; EXPECT_TRUE(containsQuantity(includesQuantity));
  includesQuantity = "     91.38967D-002 bats/s; "; EXPECT_TRUE(containsQuantity(includesQuantity));
  includesQuantity = "A 2 var model is parsimonious."; EXPECT_TRUE(containsQuantity(includesQuantity));
  includesQuantity = "Down to 31 bottles of beer on the wall."; EXPECT_TRUE(containsQuantity(includesQuantity));
  includesQuantity = "Energy use: 250 kWh/yr"; EXPECT_TRUE(containsQuantity(includesQuantity));

  // does not contain a quantity
  std::string noQuantityHere("The building has a floor area of 3,000 ft^2.");
  EXPECT_FALSE(containsQuantity(noQuantityHere));
  noQuantityHere = "just text"; EXPECT_FALSE(containsQuantity(noQuantityHere));
  noQuantityHere = "kJ/s"; EXPECT_FALSE(containsQuantity(noQuantityHere));
  noQuantityHere = "32~0 kJ/"; EXPECT_FALSE(containsQuantity(noQuantityHere));
  noQuantityHere = "3 m+"; EXPECT_FALSE(containsQuantity(noQuantityHere));
}

TEST_F(UnitsFixture,QuantityRegex_DecomposeQuantities) {
  std::string qStr("3 m");
  std::pair<std::string,std::string> result;
  
  result = decomposeQuantityString(qStr);
  EXPECT_EQ("3",result.first); EXPECT_EQ("m",result.second);

  qStr = "-.1D-3 kg*m/s^2";
  result = decomposeQuantityString(qStr);
  EXPECT_EQ("-.1D-3",result.first); EXPECT_EQ("kg*m/s^2",result.second);

  qStr = "3200 lb_f/s^2";
  result = decomposeQuantityString(qStr);
  EXPECT_EQ("3200",result.first); EXPECT_EQ("lb_f/s^2",result.second);

  qStr = "3.01E2 M(lb_m*kg/K*s)";
  result = decomposeQuantityString(qStr);
  EXPECT_EQ("3.01E2",result.first); EXPECT_EQ("M(lb_m*kg/K*s)",result.second);

  qStr = "21 shorts";
  result = decomposeQuantityString(qStr);
  EXPECT_EQ("21",result.first); EXPECT_EQ("shorts",result.second);

  qStr = "0.0002658928196837 kN";
  result = decomposeQuantityString(qStr);
  EXPECT_EQ("0.0002658928196837",result.first); EXPECT_EQ("kN",result.second);

  EXPECT_THROW(decomposeQuantityString("A nice, short sentence."),openstudio::Exception);
  EXPECT_THROW(decomposeQuantityString("5 m."),openstudio::Exception);
  EXPECT_THROW(decomposeQuantityString("2.0kg*m"),openstudio::Exception);
  EXPECT_THROW(decomposeQuantityString("9.12E-32 lb_f/s)"),openstudio::Exception);
  EXPECT_THROW(decomposeQuantityString("t3.0 s"),openstudio::Exception);
  EXPECT_THROW(decomposeQuantityString("5,000 W/m^2"),openstudio::Exception);
}

TEST_F(UnitsFixture,QuantityRegex_DecomposeScaledUnits) {
  std::string uStr("k(m/s)");
  std::pair<std::string,std::string> result;
  
  result = decomposeScaledUnitString(uStr);
  EXPECT_EQ("k",result.first); EXPECT_EQ("m/s",result.second);

  uStr = "c(kg^2*m^2/s^4)";
  result = decomposeScaledUnitString(uStr);
  EXPECT_EQ("c",result.first); EXPECT_EQ("kg^2*m^2/s^4",result.second);

  uStr = "M(lb_m/s)";
  result = decomposeScaledUnitString(uStr);
  EXPECT_EQ("M",result.first); EXPECT_EQ("lb_m/s",result.second);

  uStr = "\\mu(N/m)";
  result = decomposeScaledUnitString(uStr);
  EXPECT_EQ("\\mu",result.first); EXPECT_EQ("N/m",result.second);

  EXPECT_THROW(decomposeScaledUnitString("k(m"),openstudio::Exception);
  EXPECT_THROW(decomposeScaledUnitString("m/s"),openstudio::Exception);
  EXPECT_THROW(decomposeScaledUnitString(" \\mu(N/m)"),openstudio::Exception);
  EXPECT_THROW(decomposeScaledUnitString("32"),openstudio::Exception);
}

TEST_F(UnitsFixture,QuantityRegex_DecomposeCompoundUnits) {
  std::string uStr("1/s");
  std::pair< std::vector<std::string>,std::vector<std::string> > result;
  result = decomposeCompoundUnitString(uStr);
  ASSERT_EQ(static_cast<size_t>(0),result.first.size()); 
  ASSERT_EQ(static_cast<size_t>(1),result.second.size());
  EXPECT_EQ("s",result.second[0]);

  uStr = "kg*m/s^2"; result = decomposeCompoundUnitString(uStr);
  ASSERT_EQ(static_cast<size_t>(2),result.first.size()); 
  ASSERT_EQ(static_cast<size_t>(1),result.second.size());
  EXPECT_EQ("kg",result.first[0]); EXPECT_EQ("m",result.first[1]);
  EXPECT_EQ("s^2",result.second[0]);

  uStr = "s^{-1}/ft^{-1}"; result = decomposeCompoundUnitString(uStr);
  ASSERT_EQ(static_cast<size_t>(1),result.first.size()); 
  ASSERT_EQ(static_cast<size_t>(1),result.second.size());
  EXPECT_EQ("s^{-1}",result.first[0]);
  EXPECT_EQ("ft^{-1}",result.second[0]);

  uStr = "kg"; result = decomposeCompoundUnitString(uStr);
  ASSERT_EQ(static_cast<size_t>(1),result.first.size()); 
  ASSERT_EQ(static_cast<size_t>(0),result.second.size());
  EXPECT_EQ("kg",result.first[0]);

  uStr = "1/s^2*K"; result = decomposeCompoundUnitString(uStr);
  ASSERT_EQ(static_cast<size_t>(0),result.first.size()); 
  ASSERT_EQ(static_cast<size_t>(2),result.second.size());
  EXPECT_EQ("s^2",result.second[0]); EXPECT_EQ("K",result.second[1]);

  EXPECT_THROW(decomposeCompoundUnitString(" kg*m"),openstudio::Exception);
  EXPECT_THROW(decomposeCompoundUnitString("2 1/s"),openstudio::Exception);
}

TEST_F(UnitsFixture,QuantityRegex_DecomposeAtomicUnits) {
  std::string uStr("m");
  std::pair<std::string,int> result;
  result = decomposeAtomicUnitString(uStr);
  EXPECT_EQ("m",result.first); EXPECT_EQ(1,result.second);

  uStr = "kg^3"; result = decomposeAtomicUnitString(uStr);
  EXPECT_EQ("kg",result.first); EXPECT_EQ(3,result.second);

  uStr = "short^{-3291}"; result = decomposeAtomicUnitString(uStr);
  EXPECT_EQ("short",result.first); EXPECT_EQ(-3291,result.second);

  EXPECT_THROW(decomposeAtomicUnitString("kg^{2}"),openstudio::Exception);
  EXPECT_THROW(decomposeAtomicUnitString("kg/s"),openstudio::Exception);
}

TEST_F(UnitsFixture,QuantityRegex_DirectScaledUnit) {

  // direct scaled unit
  std::string aMatch("people/1000 ft^2");
  EXPECT_TRUE(isDirectScaledUnit(aMatch));
  aMatch = "1/10kg"; EXPECT_TRUE(isDirectScaledUnit(aMatch));
  aMatch = "people/100*m^2"; EXPECT_TRUE(isDirectScaledUnit(aMatch));

  // not a direct scaled unit
  std::string notAMatch("m^2");
  EXPECT_FALSE(isDirectScaledUnit(notAMatch));
  notAMatch = "kg/10**m"; EXPECT_FALSE(isDirectScaledUnit(notAMatch));

  // contains a direct scaled unit
  std::string includesMatch("32 people/100 m^2");
  EXPECT_TRUE(containsDirectScaledUnit(includesMatch));
  boost::smatch m;
  boost::regex_search(includesMatch,m,regexEmbeddedDirectScaledUnit());
  std::string unitStr = std::string(m[1].first,m[1].second);
  EXPECT_EQ("people/100 m^2",unitStr);
  std::pair<std::string,std::pair<unsigned,std::string> > result;
  result = decomposeDirectScaledUnit(unitStr);
  EXPECT_EQ("people/",result.first);
  EXPECT_EQ(static_cast<unsigned>(2),result.second.first);
  EXPECT_EQ("m^2",result.second.second);
  
  includesMatch = "Occupancy (people/1000*ft^2)";
  EXPECT_TRUE(containsDirectScaledUnit(includesMatch));
  boost::regex_search(includesMatch,m,regexEmbeddedDirectScaledUnit());
  unitStr = std::string(m[5].first,m[5].second);
  EXPECT_EQ("people/1000*ft^2",unitStr);
  result = decomposeDirectScaledUnit(unitStr);
  EXPECT_EQ("people/",result.first);
  EXPECT_EQ(static_cast<unsigned>(3),result.second.first);
  EXPECT_EQ("ft^2",result.second.second);

  includesMatch = "1/10kg";
  EXPECT_TRUE(containsDirectScaledUnit(includesMatch));
  boost::regex_search(includesMatch,m,regexEmbeddedDirectScaledUnit());
  unitStr = std::string(m[1].first,m[1].second);
  EXPECT_EQ("1/10kg",unitStr);
  result = decomposeDirectScaledUnit(unitStr);
  EXPECT_EQ("1/",result.first);
  EXPECT_EQ(static_cast<unsigned>(1),result.second.first);
  EXPECT_EQ("kg",result.second.second);

}
