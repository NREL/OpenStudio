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
#include "AirflowFixture.hpp"

#include "../PrjModel.hpp"
#include "../PrjAirflowElements.hpp"

// Test adding airflow elements
TEST_F(AirflowFixture, ContamModel_AirflowElements) {
  openstudio::contam::IndexModel model;
  EXPECT_EQ(0, model.getPlrTest1().size());
  openstudio::contam::PlrTest1 afe0(OPNG, "external", "This is the average leakage element for exterior walls",
    "6.13696e-008", "0.000499082", "0.65", "75", "0.00906345");
  openstudio::contam::PlrTest1 afe1(OPNG, "internal", "This is the average leakage element for interior walls",
    1.47921e-007, 0.000998165, 0.65, 75, 0.0181269);
  openstudio::contam::PlrLeak2 afe2(OPNG, "WNOO6C_CMX", "Operable window, Building C, maximum (from C&IMisc.lb3)",
    "4.98716e-008", "0.00039745", "0.65", "1", "4", "0.000346");
  model.addAirflowElement(afe0);
  model.addAirflowElement(afe1);
  model.addAirflowElement(afe2);
  ASSERT_EQ(2, model.getPlrTest1().size());
  ASSERT_EQ(1, model.getPlrLeak2().size());
  EXPECT_EQ(1, afe0.nr());
  EXPECT_EQ(2, afe1.nr());
  EXPECT_EQ(3, afe2.nr());
  EXPECT_EQ(afe0, model.getPlrTest1()[0]);
  EXPECT_EQ(afe1, model.getPlrTest1()[1]);
  EXPECT_EQ(afe2, model.getPlrLeak2()[0]);
}

// Verify that the species/contaminant stuff works
TEST_F(AirflowFixture, ContamModel_Species) {
  openstudio::contam::IndexModel model;
  EXPECT_EQ(0, model.species().size());
  EXPECT_EQ(0, model.contaminants().size());
  // Make some species
  openstudio::contam::Species CO2(44.01, 2.0e-5, 6.0795e-4, 0.0, "CO2", "Carbon Dioxide");
  openstudio::contam::Species SF6("146.06", "2.0e-5", "0.0", "0.0", "SF6", "Sulfur Hexafluoride");
  // Add them
  model.addSpecies(CO2);
  model.addSpecies(SF6);
  ASSERT_EQ(2, model.species().size());
  ASSERT_EQ(0, model.contaminants().size());
  EXPECT_EQ(1, CO2.nr());
  EXPECT_EQ(2, SF6.nr());
  EXPECT_EQ(CO2, model.species()[0]);
  EXPECT_EQ(SF6, model.species()[1]);
  // Turn on CO2
  CO2.setSflag(true);
  ASSERT_EQ(1, model.contaminants().size());
  ASSERT_EQ(1, model.contaminants()[0]);
  // Remove CO2
  ASSERT_TRUE(model.removeSpecies(CO2));
  ASSERT_EQ(1, model.species().size());
  EXPECT_EQ(SF6, model.species()[0]);
  EXPECT_EQ(1, SF6.nr());
  EXPECT_EQ(0, model.contaminants().size());
  // Remove CO2 again
  ASSERT_FALSE(model.removeSpecies(CO2));
  // Remove SF6
  ASSERT_TRUE(model.removeSpecies(SF6));
  ASSERT_EQ(0, model.species().size());
  EXPECT_EQ(0, model.contaminants().size());
  // Remove SF6 again
  ASSERT_FALSE(model.removeSpecies(SF6));
}

// Verify level creation and addition
TEST_F(AirflowFixture, ContamModel_Levels) {
  openstudio::contam::IndexModel model;
  EXPECT_EQ(0, model.levels().size());
  openstudio::contam::Level level0(3.0, "Level_0");
  openstudio::contam::Level level1(3.5, "Level_1");
  openstudio::contam::Level level2(3.0, "Level_2");

  model.addLevel(level0);
  model.addLevel(level1);
  model.addLevel(level2);

  ASSERT_EQ(3, model.levels().size());
  EXPECT_EQ(0.0, model.levels()[0].refht());
  EXPECT_EQ(level0, model.levels()[0]);
  EXPECT_EQ(3.0, model.levels()[1].refht());
  EXPECT_EQ(level1, model.levels()[1]);
  EXPECT_EQ(6.5, model.levels()[2].refht());
  EXPECT_EQ(level2, model.levels()[2]);
}

// Verify zone creation and addition
TEST_F(AirflowFixture, ContamModel_Zones) {
  openstudio::contam::IndexModel model;
  EXPECT_EQ(0, model.zones().size());

  openstudio::contam::Zone zone0(openstudio::contam::ZoneFlags::VAR_P | openstudio::contam::ZoneFlags::VAR_C,
    "300","298.15","Zone_0");
  openstudio::contam::Zone zone1(openstudio::contam::ZoneFlags::VAR_P, 400, 293.15, "Zone_1");
  openstudio::contam::Zone zone2(openstudio::contam::ZoneFlags::SYS_N, 100, 293.15, "Zone_2");

  EXPECT_TRUE(zone0.variableContaminants());
  EXPECT_TRUE(zone0.variablePressure());

  EXPECT_FALSE(zone1.variableContaminants());
  EXPECT_TRUE(zone1.variablePressure());
  EXPECT_TRUE(zone2.system());

  model.addZone(zone0);
  model.addZone(zone1);
  model.addZone(zone2);

  ASSERT_EQ(3, model.zones().size());
  EXPECT_EQ(zone0, model.zones()[0]);
  EXPECT_EQ(zone1, model.zones()[1]);
  EXPECT_EQ(zone2, model.zones()[2]);
}
