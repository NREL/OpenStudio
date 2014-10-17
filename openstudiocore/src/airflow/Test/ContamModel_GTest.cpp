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
  openstudio::contam::Species SF6(146.06, 2.0e-5, 0.0, 0.0, "SF6", "Sulfur Hexafluoride");
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


