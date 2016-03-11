/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "ModelFixture.hpp"
#include "../ElectricLoadCenterDistribution.hpp"
#include "../GeneratorPhotovoltaic.hpp"
#include "../ElectricLoadCenterInverterSimple.hpp"
#include "../ElectricLoadCenterInverterLookUpTable.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, ElectricLoadCenterDistribution) {
  Model model;

  ElectricLoadCenterDistribution elcd(model);
  EXPECT_EQ(0u, elcd.generators().size());
  EXPECT_EQ("Baseload", elcd.generatorOperationSchemeType());
  EXPECT_TRUE(elcd.isGeneratorOperationSchemeTypeDefaulted());
  EXPECT_EQ("DirectCurrentWithInverter", elcd.electricalBussType());
  EXPECT_TRUE(elcd.isElectricalBussTypeDefaulted());
  EXPECT_FALSE(elcd.inverter());

  EXPECT_TRUE(elcd.setGeneratorOperationSchemeType("Baseload"));
  EXPECT_EQ("Baseload", elcd.generatorOperationSchemeType());
  EXPECT_FALSE(elcd.isGeneratorOperationSchemeTypeDefaulted());
  elcd.resetGeneratorOperationSchemeType();

  GeneratorPhotovoltaic panel1 = GeneratorPhotovoltaic::simple(model);
  GeneratorPhotovoltaic panel2 = GeneratorPhotovoltaic::simple(model);
  EXPECT_TRUE(elcd.addGenerator(panel1));
  ASSERT_EQ(1u, elcd.generators().size());
  EXPECT_EQ(panel1.handle(), elcd.generators()[0].handle());
  EXPECT_TRUE(elcd.addGenerator(panel2));
  ASSERT_EQ(2u, elcd.generators().size());
  EXPECT_EQ(panel1.handle(), elcd.generators()[0].handle());
  EXPECT_EQ(panel2.handle(), elcd.generators()[1].handle());
  EXPECT_TRUE(elcd.removeGenerator(panel1));
  ASSERT_EQ(1u, elcd.generators().size());
  EXPECT_EQ(panel2.handle(), elcd.generators()[0].handle());
  EXPECT_TRUE(elcd.removeGenerator(panel2));
  ASSERT_EQ(0u, elcd.generators().size());

  // can't remove something not already there
  EXPECT_FALSE(elcd.removeGenerator(panel2));

  // can't add something twice
  EXPECT_TRUE(elcd.addGenerator(panel1));
  EXPECT_FALSE(elcd.addGenerator(panel1));

  ElectricLoadCenterInverterSimple inverter(model);
  EXPECT_TRUE(inverter.name());
  EXPECT_TRUE(elcd.setInverter(inverter));
  ASSERT_TRUE(elcd.inverter());
  EXPECT_EQ(inverter.handle(), elcd.inverter()->handle());
  elcd.resetInverter();
}

TEST_F(ModelFixture, ElectricLoadCenterDistribution_Panels) {
  Model model;

  ElectricLoadCenterDistribution elcd(model);
  GeneratorPhotovoltaic panel1 = GeneratorPhotovoltaic::simple(model);
  GeneratorPhotovoltaic panel2 = GeneratorPhotovoltaic::simple(model);

  EXPECT_TRUE(elcd.addGenerator(panel1));
  EXPECT_TRUE(elcd.addGenerator(panel2));
  ASSERT_EQ(2u, elcd.generators().size());
  EXPECT_EQ(panel1.handle(), elcd.generators()[0].handle());
  EXPECT_EQ(panel2.handle(), elcd.generators()[1].handle());
  EXPECT_TRUE(panel1.electricLoadCenterDistribution());
  EXPECT_TRUE(panel2.electricLoadCenterDistribution());

  panel2.remove();

  ASSERT_EQ(1u, elcd.generators().size());
  EXPECT_EQ(panel1.handle(), elcd.generators()[0].handle());

  panel1.remove();

  EXPECT_EQ(0, elcd.generators().size());
}

TEST_F(ModelFixture, ElectricLoadCenterDistribution_Panels2) {
  Model model;

  ElectricLoadCenterDistribution elcd1(model);
  ElectricLoadCenterDistribution elcd2(model);
  GeneratorPhotovoltaic panel = GeneratorPhotovoltaic::simple(model);

  EXPECT_TRUE(elcd1.addGenerator(panel));
  EXPECT_EQ(1u, elcd1.generators().size());
  EXPECT_EQ(0, elcd2.generators().size());
  ASSERT_TRUE(panel.electricLoadCenterDistribution());
  EXPECT_EQ(elcd1.handle(), panel.electricLoadCenterDistribution()->handle());

  EXPECT_TRUE(elcd2.addGenerator(panel));
  EXPECT_EQ(0, elcd1.generators().size());
  EXPECT_EQ(1u, elcd2.generators().size());
  ASSERT_TRUE(panel.electricLoadCenterDistribution());
  EXPECT_EQ(elcd2.handle(), panel.electricLoadCenterDistribution()->handle());

  elcd2.remove();
  EXPECT_FALSE(panel.electricLoadCenterDistribution());
}

TEST_F(ModelFixture, ElectricLoadCenterDistribution_Inverters) {
  Model model;

  ElectricLoadCenterDistribution elcd(model);
  ElectricLoadCenterInverterSimple inverter1(model);
  ElectricLoadCenterInverterLookUpTable inverter2(model);

  EXPECT_FALSE(elcd.inverter());
  EXPECT_FALSE(inverter1.electricLoadCenterDistribution());
  EXPECT_FALSE(inverter2.electricLoadCenterDistribution());

  EXPECT_TRUE(elcd.setInverter(inverter1));
  ASSERT_TRUE(elcd.inverter());
  EXPECT_EQ(inverter1.handle(), elcd.inverter()->handle());
  EXPECT_TRUE(inverter1.electricLoadCenterDistribution());
  EXPECT_FALSE(inverter2.electricLoadCenterDistribution());

  inverter1.remove();

  EXPECT_FALSE(elcd.inverter());
  EXPECT_FALSE(inverter2.electricLoadCenterDistribution());

  EXPECT_TRUE(elcd.setInverter(inverter2));
  ASSERT_TRUE(elcd.inverter());
  EXPECT_EQ(inverter2.handle(), elcd.inverter()->handle());
  EXPECT_TRUE(inverter2.electricLoadCenterDistribution());

  elcd.remove();
  EXPECT_FALSE(inverter2.electricLoadCenterDistribution());
}

TEST_F(ModelFixture, ElectricLoadCenterDistribution_Inverters2) {
  Model model;

  ElectricLoadCenterDistribution elcd1(model);
  ElectricLoadCenterDistribution elcd2(model);
  ElectricLoadCenterInverterSimple inverter(model);

  EXPECT_FALSE(elcd1.inverter());
  EXPECT_FALSE(elcd1.inverter());
  EXPECT_FALSE(inverter.electricLoadCenterDistribution());

  EXPECT_TRUE(elcd1.setInverter(inverter));
  EXPECT_TRUE(elcd1.inverter());
  EXPECT_FALSE(elcd2.inverter());
  ASSERT_TRUE(inverter.electricLoadCenterDistribution());
  EXPECT_EQ(elcd1.handle(), inverter.electricLoadCenterDistribution()->handle());

  EXPECT_TRUE(elcd2.setInverter(inverter));
  EXPECT_FALSE(elcd1.inverter());
  EXPECT_TRUE(elcd2.inverter());
  ASSERT_TRUE(inverter.electricLoadCenterDistribution());
  EXPECT_EQ(elcd2.handle(), inverter.electricLoadCenterDistribution()->handle());
}