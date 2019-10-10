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

#include "ModelFixture.hpp"
#include "../EnergyManagementSystemMeteredOutputVariable.hpp"
#include "../EnergyManagementSystemSensor.hpp"
#include "../EnergyManagementSystemProgram.hpp"
#include "../EnergyManagementSystemSubroutine.hpp"
#include "../Model_Impl.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idf/ValidityReport.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/data/TimeSeries.hpp"
#include "../../utilities/core/Compare.hpp"
#include "../../utilities/core/Optional.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, EMSMeteredOutputVariable)
{
  Model model;
  // add Site Outdoor Air Drybulb Temperature
  OutputVariable siteOutdoorAirDrybulbTemperature("Site Outdoor Air Drybulb Temperature", model);
  EXPECT_EQ("*", siteOutdoorAirDrybulbTemperature.keyValue());
  EXPECT_EQ("Site Outdoor Air Drybulb Temperature", siteOutdoorAirDrybulbTemperature.variableName());

  // add sensor
  EnergyManagementSystemSensor OATdbSensor(model, siteOutdoorAirDrybulbTemperature);
  OATdbSensor.setName("OATdb Sensor");
  //OATdbSensor.setOutputVariable(siteOutdoorAirDrybulbTemperature);

  //add program
  EnergyManagementSystemProgram program_1(model);
  program_1.setName("program one");

  //add program
  EnergyManagementSystemSubroutine subroutine_1(model);
  subroutine_1.setName("subroutine one");

  // add metered output variable
  EnergyManagementSystemMeteredOutputVariable meteredoutvar(model, OATdbSensor.name().get());

  //meteredoutvar.setEMSVariableName(OATdbSensor.name().get());
  EXPECT_EQ(meteredoutvar.emsVariableName(), OATdbSensor.name().get());
  meteredoutvar.setUpdateFrequency("ZoneTimestep");
  EXPECT_EQ("ZoneTimestep",meteredoutvar.updateFrequency());
  meteredoutvar.setEMSProgramOrSubroutineName(program_1);
  EXPECT_EQ(program_1.name().get(), meteredoutvar.emsProgramOrSubroutineName());
  EXPECT_EQ(program_1, meteredoutvar.emsProgram());
  meteredoutvar.resetEMSProgramOrSubroutineName();
  EXPECT_EQ("",meteredoutvar.emsProgramOrSubroutineName());
  meteredoutvar.setEMSProgramOrSubroutineName(subroutine_1);
  EXPECT_EQ(subroutine_1.name().get(), meteredoutvar.emsProgramOrSubroutineName());
  EXPECT_EQ(subroutine_1, meteredoutvar.emsSubroutine());
  meteredoutvar.setResourceType("NaturalGas");
  EXPECT_EQ("NaturalGas", meteredoutvar.resourceType());
  meteredoutvar.setGroupType("HVAC");
  EXPECT_EQ("HVAC", meteredoutvar.groupType());
  meteredoutvar.setEndUseCategory("Heating");
  EXPECT_EQ("Heating", meteredoutvar.endUseCategory());
  meteredoutvar.setEndUseSubcategory("Madeup");
  EXPECT_EQ("Madeup", meteredoutvar.endUseSubcategory());

  model.save(toPath("./EMS_meteredoutvar.osm"), true);

  // add output variable sensor
  EnergyManagementSystemMeteredOutputVariable outvar_sen(model, OATdbSensor);
  //setname
  outvar_sen.setName("outputVar sen");
  EXPECT_EQ("outputVar sen", outvar_sen.nameString());

  ASSERT_TRUE(outvar_sen.emsVariableObject());
  ASSERT_TRUE(outvar_sen.emsSensor());
  EXPECT_EQ(outvar_sen.emsVariableObject().get(), outvar_sen.emsSensor().get());
}

