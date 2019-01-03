/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "UnitsFixture.hpp"

#include "../QuantityConverter.hpp"
#include "../Quantity.hpp"
#include "../OSQuantityVector.hpp"
#include "../UnitFactory.hpp"

#include "../BTUUnit.hpp"
#include "../CelsiusUnit.hpp"
#include "../CFMUnit.hpp"
#include "../FahrenheitUnit.hpp"
#include "../IPUnit.hpp"
#include "../SIUnit.hpp"
#include "../Unit.hpp"

using namespace openstudio;

TEST_F(UnitsFixture, QuantityConverter_IPandSIUsingSystem)
{
  LOG(Debug, "QuantityConvert_ConvertUsingSystem");
  UnitSystem siSys(UnitSystem::SI);
  UnitSystem ipSys(UnitSystem::IP);

  IPUnit ipu1(openstudio::IPExpnt(0,1));
  Quantity ipq1(110.0, ipu1);
  OptionalQuantity siq1 = QuantityConverter::instance().convert(ipq1, siSys);
  EXPECT_TRUE(siq1);
  if (siq1) {
    SCOPED_TRACE("ipu1 to SI");
    testNumbersEqual(33.528,siq1->value());
    testStreamOutput("33.5 m", *siq1, 1);
  }

  IPUnit ftSquared(openstudio::IPExpnt(0,2));
  Quantity ipqFtSquared( 100.0, ftSquared );
  OptionalQuantity siqMSquared = QuantityConverter::instance().convert(ipqFtSquared, siSys);
  EXPECT_TRUE(siqMSquared);
  if (siqMSquared) {
    SCOPED_TRACE("ipqFtSquared to SI");
    testNumbersEqual(9.290304, siqMSquared->value());
    testStreamOutput("9.29 m^2", *siqMSquared, 2);
  }

  IPUnit ipu2(openstudio::IPExpnt(0,0,0,0,0,0,0,1));
  Quantity ipq2( 130.0, ipu2 );
  OptionalQuantity siq2 = QuantityConverter::instance().convert(ipq2, siSys);
  EXPECT_TRUE(siq2);
  if (siq2) {
    SCOPED_TRACE("ipu2 to SI");
    testNumbersEqual(578.268809984, siq2->value());
    testStreamOutput("578.3 N", *siq2, 1);
  }

  IPUnit ipu3(openstudio::IPExpnt(0,0,0,1));
  Quantity ipq3( 529.67, ipu3 );
  OptionalQuantity siq3 = QuantityConverter::instance().convert(ipq3, siSys);
  EXPECT_TRUE(siq3);
  if (siq3) {
    SCOPED_TRACE("ipu3 to SI");
    testNumbersEqual(294.261111111, siq3->value());
    testStreamOutput("294 K", *siq3, 0);
  }

  SIUnit siu4(openstudio::SIExpnt(1,2,-2));
  Quantity siq4( 1300.0, siu4 );
  OptionalQuantity ipq4 = QuantityConverter::instance().convert(siq4, ipSys);
  EXPECT_TRUE(ipq4);
  if (ipq4) {
    SCOPED_TRACE("siu4 to IP");
    testNumbersEqual(30849.4685255, ipq4->value());
    testStreamOutput("30849.47 lb_m*ft^2/s^2", *ipq4, 2);
  }

  SIUnit siu5(openstudio::SIExpnt(0,0,0,1));
  Quantity siq5( 294.0, siu5 );
  OptionalQuantity ipq5 = QuantityConverter::instance().convert(siq5, ipSys);
  EXPECT_TRUE(ipq5);
  if (ipq5) {
    SCOPED_TRACE("siu5 to IP");
    testNumbersEqual(529.2, ipq5->value());
    testStreamOutput("529.2 R", *ipq5, 1);
  }
}

TEST_F(UnitsFixture,QuantityConverter_BTUandIPUsingSystem)
{
  LOG(Debug, "QuantityConverter_BTUandIPUsingSystem");

  UnitSystem siSys(UnitSystem::SI);
  UnitSystem ipSys(UnitSystem::IP);
  UnitSystem btuSys(UnitSystem::BTU);

  // uses BTU to SI, SI to IP
  BTUUnit btuu1(openstudio::BTUExpnt(1,-2,0,0), 3);
  Quantity bQ( 67.5, btuu1 );
  testStreamOutput("67.5 kBtu/ft^2",bQ);
  OptionalQuantity ipQ = QuantityConverter::instance().convert( bQ, ipSys);
  EXPECT_TRUE(ipQ);
  if (ipQ) {
    EXPECT_EQ("lb_m/s^2",ipQ->standardUnitsString(false));
    EXPECT_EQ("klb_m/s^2",ipQ->standardUnitsString());
    SCOPED_TRACE("btu1 to IP");
    testNumbersEqual(1689985.20448, ipQ->value());
  }

  BTUUnit btuu2(openstudio::BTUExpnt(0,0,-1));
  bQ = Quantity(5000.0, btuu2);
  ipQ = QuantityConverter::instance().convert( bQ, ipSys);
  EXPECT_TRUE(ipQ);
  if (ipQ) {
    SCOPED_TRACE("bQ to IP, 1");
    testStreamOutput("1.3889 1/s",*ipQ,4);
  }

  bQ *= bQ; // 25E6/h^2
  ipQ = QuantityConverter::instance().convert( bQ, ipSys);
  EXPECT_TRUE(ipQ);
  if (ipQ) {
    SCOPED_TRACE("bQ to IP, 2");
    testStreamOutput("1.9290 1/s^2",*ipQ,4);
  }

  BTUUnit btuu3(openstudio::BTUExpnt(-1),-3);
  bQ = Quantity(1.0, btuu3);
  testStreamOutput("1 1/kBtu",bQ);
  OptionalQuantity siQ = QuantityConverter::instance().convert(bQ,siSys);
  EXPECT_TRUE(siQ);
  if (siQ) {
    EXPECT_EQ("1/J",siQ->prettyUnitsString(false));
    SCOPED_TRACE("btu3 to SI");
    testNumbersEqual(9.478171203133172e-4,siQ->value());
    siQ->setScale(0);
    SCOPED_TRACE("rescaled btu3 to SI");
    testNumbersEqual(9.478171203133172e-7,siQ->value());
  }

  // uses IP to SI, SI to BTU
  IPUnit ipu1(openstudio::IPExpnt(0,1,0,-1,0,0,0,1), -2);
  Quantity ipQ2( 2.0, ipu1);
  OptionalQuantity bQ2 = QuantityConverter::instance().convert( ipQ2, btuSys);
  EXPECT_TRUE(bQ2);
  if (bQ2) {
    EXPECT_EQ("Btu/R", bQ2->standardUnitsString(false));
    EXPECT_EQ("cBtu/R", bQ2->standardUnitsString());
    SCOPED_TRACE("ipu1 to BTU");
    testNumbersEqual(0.002570134927, bQ2->value(),1.0e-5);
    bQ2->setScale(0);
    EXPECT_EQ("Btu/R", bQ2->standardUnitsString());
    testNumbersEqual(2.570134927e-5, bQ2->value(),1.0e-5);
  }
}

TEST_F(UnitsFixture,QuantityConverter_CFMandSIUsingSystem)
{
  LOG(Debug, "QuantityConverter_CFMandSIUsingSystem");

  SIUnit siu1(openstudio::SIExpnt(1));
  Quantity oneKg(1.0, siu1);
  OptionalQuantity intermediate = QuantityConverter::instance().convert(oneKg, UnitSystem(UnitSystem::CFM));
  ASSERT_TRUE(intermediate);
  OptionalQuantity copyOfCopy = QuantityConverter::instance().convert(*intermediate, UnitSystem(UnitSystem::SI));
  ASSERT_TRUE(copyOfCopy);
  EXPECT_NEAR(oneKg.value(),copyOfCopy->value(),tol);
  EXPECT_EQ(oneKg.standardUnitsString(false),copyOfCopy->standardUnitsString(false));
}

TEST_F(UnitsFixture,QuantityConverter_PowerDensity) {
  Quantity siLpd(10.0,createSIPowerDensity());
  Unit ipPowerDensity = createUnit("W/ft^2").get();
  OptionalQuantity ipLpd = convert(siLpd,ipPowerDensity);
  Quantity siArea(1.0,pow(createSILength(),2));
  OptionalQuantity ipArea = convert(siArea,UnitSystem(UnitSystem::IP));
  ASSERT_TRUE(ipLpd);
  ASSERT_TRUE(ipArea);
  EXPECT_NEAR(10.0/ipArea->value(),ipLpd->value(),tol);
  EXPECT_EQ("W/ft^2",ipLpd->prettyUnitsString());
}

TEST_F(UnitsFixture,QuantityConverter_OSQuantityVector) {
  // basic conversion
  OSQuantityVector testVec(createIPEnergy(),2u,100.0);
  Quantity testQ(100.0,createIPEnergy());

  OSQuantityVector resultVec = convert(testVec,UnitSystem(UnitSystem::Wh));
  OptionalQuantity resultQ = convert(testQ,UnitSystem(UnitSystem::Wh));
  ASSERT_EQ(2u,resultVec.size());
  ASSERT_TRUE(resultQ);
  EXPECT_EQ(resultQ.get(),resultVec.getQuantity(0));
  EXPECT_EQ(resultQ.get(),resultVec.getQuantity(1));
  EXPECT_EQ(resultQ->system(),resultVec.system());

  resultVec = convert(testVec,resultQ->units());
  resultQ = convert(testQ,resultQ->units());
  ASSERT_EQ(2u,resultVec.size());
  ASSERT_TRUE(resultQ);
  EXPECT_EQ(resultQ.get(),resultVec.getQuantity(0));
  EXPECT_EQ(resultQ.get(),resultVec.getQuantity(1));
  EXPECT_EQ(resultQ->system(),resultVec.system());

  // temperature conversion
  testVec = OSQuantityVector(createCelsiusTemperature(),2u,20.0);
  testQ = Quantity(20.0,createCelsiusTemperature());
  resultVec = convert(testVec,UnitSystem(UnitSystem::Fahrenheit));
  resultQ = convert(testQ,UnitSystem(UnitSystem::Fahrenheit));
  ASSERT_EQ(2u,resultVec.size());
  ASSERT_TRUE(resultQ);
  EXPECT_EQ(resultQ.get().units(),resultVec.units());
  // not sure why these aren't quite getting the precision we would like
  EXPECT_NEAR(resultQ.get().value(),resultVec.getQuantity(0).value(),1.0E-12);
  EXPECT_NEAR(resultQ.get().value(),resultVec.getQuantity(1).value(),1.0E-12);
  EXPECT_EQ(resultQ->system(),resultVec.system());
  EXPECT_TRUE(resultVec.isAbsolute());
  EXPECT_TRUE(resultQ->isAbsolute());

  testVec.setAsRelative();
  testQ.setAsRelative();
  resultVec = convert(testVec,UnitSystem(UnitSystem::Fahrenheit));
  resultQ = convert(testQ,UnitSystem(UnitSystem::Fahrenheit));
  ASSERT_EQ(2u,resultVec.size());
  ASSERT_TRUE(resultQ);
  EXPECT_EQ(resultQ.get().units(),resultVec.units());
  // not sure why these aren't quite getting the precision we would like
  EXPECT_NEAR(resultQ.get().value(),resultVec.getQuantity(0).value(),1.0E-12);
  EXPECT_NEAR(resultQ.get().value(),resultVec.getQuantity(1).value(),1.0E-12);
  EXPECT_EQ(resultQ->system(),resultVec.system());
  EXPECT_TRUE(resultVec.isRelative());
  EXPECT_TRUE(resultQ->isRelative());
}

TEST_F(UnitsFixture,QuantityConverter_Profiling_QuantityVectorBaseCase) {
  QuantityVector result(testQuantityVector);
  for (auto & elem : result) {
    elem = convert(elem,UnitSystem(UnitSystem::Wh)).get();
  }
}

TEST_F(UnitsFixture,QuantityConverter_Profiling_OSQuantityVector) {
  OSQuantityVector result = convert(testOSQuantityVector,UnitSystem(UnitSystem::Wh));
}
