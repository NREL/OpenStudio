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

#include "../UnitFactory.hpp"
#include "../Unit.hpp"
#include "../SIUnit.hpp"
#include "../SIUnit_Impl.hpp"
#include "../IPUnit.hpp"
#include "../IPUnit_Impl.hpp"
#include "../BTUUnit.hpp"
#include "../BTUUnit_Impl.hpp"
#include "../CFMUnit.hpp"
#include "../CFMUnit_Impl.hpp"
#include "../CelsiusUnit.hpp"
#include "../CelsiusUnit_Impl.hpp"
#include "../FahrenheitUnit.hpp"
#include "../FahrenheitUnit_Impl.hpp"

#include "../../core/Exception.hpp"

#include <boost/pointer_cast.hpp>

using openstudio::Unit;
using openstudio::OptionalUnit;
using openstudio::UnitSystem;
using openstudio::SIUnit;
using openstudio::OptionalSIUnit;
using openstudio::IPUnit;
using openstudio::OptionalIPUnit;
using openstudio::BTUUnit;
using openstudio::OptionalBTUUnit;
using openstudio::CFMUnit;
using openstudio::OptionalCFMUnit;
using openstudio::CelsiusUnit;
using openstudio::OptionalCelsiusUnit;
using openstudio::FahrenheitUnit;
using openstudio::OptionalFahrenheitUnit;
using openstudio::UnitFactory;
using openstudio::Exception;

TEST_F(UnitsFixture,UnitFactory_ParseStandardUnits)
{
  LOG(Debug,"UnitFactory_ParseStandardUnits");

  OptionalUnit l1 = UnitFactory::instance().createUnit("m");
  OptionalSIUnit siLength = l1->optionalCast<SIUnit>();
  OptionalIPUnit ipLength = l1->optionalCast<IPUnit>();
  ASSERT_TRUE(siLength);
  ASSERT_FALSE(ipLength);

  ASSERT_THROW(siLength->setBaseUnitExponent("ft",1),Exception);
  ASSERT_THROW(l1->setBaseUnitExponent("ft",1),Exception);
  testStreamOutput("m",*siLength);

  OptionalUnit f1 = UnitFactory::instance().createUnit("kg*m/s^2");
  testStreamOutput("N",*f1);

  // RESOLUTION OF SYSTEM CONFLICTS
  // dumb pass-through because of conflict between SI and IP
  OptionalUnit t1 = UnitFactory::instance().createUnit("s");
  testStreamOutput("s",*t1); // is SI unit, but not reliably

  OptionalUnit t2 = UnitFactory::instance().createUnit("s",UnitSystem::SI);
  OptionalSIUnit t2SI = t2->optionalCast<SIUnit>();
  OptionalIPUnit t2IP = t2->optionalCast<IPUnit>();
  ASSERT_TRUE(t2SI);
  ASSERT_FALSE(t2IP);

  OptionalUnit t3 = UnitFactory::instance().createUnit("s",UnitSystem::IP);
  OptionalSIUnit t3SI = t3->optionalCast<SIUnit>();
  OptionalIPUnit t3IP = t3->optionalCast<IPUnit>();
  ASSERT_FALSE(t3SI);
  ASSERT_TRUE(t3IP);

  OptionalUnit T = UnitFactory::instance().createUnit("C");
  OptionalCelsiusUnit TC = T->optionalCast<CelsiusUnit>();
  OptionalFahrenheitUnit TF = T->optionalCast<FahrenheitUnit>();
  ASSERT_TRUE(TC);
  ASSERT_FALSE(TF);
  
  T = UnitFactory::instance().createUnit("F");
  TC = T->optionalCast<CelsiusUnit>();
  TF = T->optionalCast<FahrenheitUnit>();
  ASSERT_FALSE(TC);
  ASSERT_TRUE(TF);
}

TEST_F(UnitsFixture,UnitFactory_TestForAndReturnUnitObjects) {

  LOG(Debug,"UnitFactory_TestForAndReturnUnitObjects");

  ASSERT_TRUE(openstudio::getSystem("s") == UnitSystem::SI);
  ASSERT_TRUE(openstudio::createUnit("s",UnitSystem::IP)->system() == UnitSystem::IP);
  ASSERT_TRUE(openstudio::getSystem("kg*m/s^2") == UnitSystem::SI);
  ASSERT_TRUE(openstudio::getSystem("ft") == UnitSystem::IP);
  
  OptionalUnit f1 = openstudio::createUnit("kg*m/s^2");
  ASSERT_TRUE(f1);
  ASSERT_NO_THROW(f1->cast<SIUnit>());
  EXPECT_TRUE(f1->system() == UnitSystem::SI);
  {
    SCOPED_TRACE("Creation of N from standard string.");
    testStreamOutput("N",*f1);
  }
  f1 = openstudio::createUnit("ft^2");
  EXPECT_TRUE(f1->system() == UnitSystem::IP);
  ASSERT_ANY_THROW(f1->cast<SIUnit>());
  OptionalUnit T1 = openstudio::createUnit("R");
  ASSERT_NO_THROW(T1->cast<IPUnit>());
  Unit mixedUnit = (*f1)*(*T1);
  {
    SCOPED_TRACE("Output of mixed unit multiplication.");
    testStreamOutput("ft^2*R",mixedUnit);
  }

  ASSERT_TRUE(openstudio::getSystem("Btu/h") == UnitSystem::BTU);
  OptionalUnit e1 = openstudio::createUnit("Btu/h");
  ASSERT_TRUE(e1);
  ASSERT_NO_THROW(e1->cast<BTUUnit>());
  BTUUnit btuU = e1->cast<BTUUnit>();
  EXPECT_EQ(1,btuU.baseUnitExponent("Btu"));
  EXPECT_EQ(-1,btuU.baseUnitExponent("h"));

  ASSERT_TRUE(openstudio::getSystem("CFM") == UnitSystem::CFM);
  CFMUnit cfmU = openstudio::createUnit("CFM")->cast<CFMUnit>();
  EXPECT_EQ(3,cfmU.baseUnitExponent("ft"));
  EXPECT_EQ(-1,cfmU.baseUnitExponent("min"));
  EXPECT_EQ("cfm",cfmU.prettyString());

  ASSERT_EQ(UnitSystem::Celsius,openstudio::getSystem("C").value());
  CelsiusUnit TC = openstudio::createUnit("C")->cast<CelsiusUnit>();
  ASSERT_ANY_THROW(openstudio::createUnit("W/K")->cast<CelsiusUnit>());

  ASSERT_FALSE(openstudio::isInSystem("F*ft",UnitSystem::Fahrenheit));
  FahrenheitUnit TF = openstudio::createUnit("F")->cast<FahrenheitUnit>();
}

TEST_F(UnitsFixture,UnitFactory_KitchenSink) {
  // unregistered compound units
  SIUnit eui = openstudio::createUnit("J/m^2",UnitSystem::SI)->cast<SIUnit>();
  testStreamOutput("J/m^2",eui);
  EXPECT_EQ("kg/s^2",eui.standardString(false));
  EXPECT_EQ(0,eui.scale().exponent);

  BTUUnit ipEui = openstudio::createUnit("Btu/ft^2",UnitSystem::BTU)->cast<BTUUnit>();
  testStreamOutput("Btu/ft^2",ipEui);
  EXPECT_EQ("",ipEui.prettyString());
  EXPECT_EQ(0,eui.scale().exponent);

  // scaled atomic units
  SIUnit f1 = openstudio::createUnit("\\muN",UnitSystem::SI)->cast<SIUnit>();
  testStreamOutput("\\muN",f1);
  EXPECT_EQ(-6,f1.scale().exponent);

  // parenthetical scaled units
  f1 = openstudio::createUnit("c(kg*m/s^2)",UnitSystem::SI)->cast<SIUnit>();
  testStreamOutput("cN",f1);
  EXPECT_EQ(-2,f1.scale().exponent);

  // first baseUnit scaled
  eui = openstudio::createUnit("MJ/m^2",UnitSystem::SI)->cast<SIUnit>();
  testStreamOutput("MJ/m^2",eui);
  EXPECT_EQ("kg/s^2",eui.standardString(false));
  EXPECT_EQ(6,eui.scale().exponent);

  ipEui = openstudio::createUnit("kBtu/ft^2",UnitSystem::BTU)->cast<BTUUnit>();
  testStreamOutput("kBtu/ft^2",ipEui);
  EXPECT_EQ("Btu/ft^2",ipEui.standardString(false));
  EXPECT_EQ("",ipEui.prettyString());
  EXPECT_EQ(3,ipEui.scale().exponent);

  // multiple baseUnits scaled
  SIUnit u = openstudio::createUnit("MJ/ms*kK",UnitSystem::SI)->cast<SIUnit>();
  testStreamOutput("MW/K",u);
  EXPECT_EQ(6,u.scale().exponent);
  EXPECT_EQ(1,u.baseUnitExponent("kg"));
  EXPECT_EQ(2,u.baseUnitExponent("m"));
  EXPECT_EQ(-3,u.baseUnitExponent("s"));
  EXPECT_EQ(-1,u.baseUnitExponent("K"));

  // odd bug observed. test added so can fix it. 
  std::string unitString = openstudio::extractUnitString("J/J");
  u = openstudio::createUnit(unitString,UnitSystem::SI)->cast<SIUnit>();
  EXPECT_EQ("",u.standardString(false));
  EXPECT_EQ("",u.prettyString());
}
