/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>
#include "AirflowFixture.hpp"

#include "../contam/PrjModel.hpp"
#include "../contam/PrjAirflowElements.hpp"

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
