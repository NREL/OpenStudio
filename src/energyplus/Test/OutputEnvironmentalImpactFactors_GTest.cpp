/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OutputEnvironmentalImpactFactors.hpp"
#include "../../model/OutputEnvironmentalImpactFactors_Impl.hpp"

#include "../../model/FuelFactors.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/Output_EnvironmentalImpactFactors_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_OutputEnvironmentalImpactFactors) {

  ForwardTranslator ft;

  Model m;

  OutputEnvironmentalImpactFactors o(m);
  EXPECT_TRUE(o.setReportingFrequency("Hourly"));

  // I also made it required to actually have at least one FuelFactor
  {
    Workspace w = ft.translateModel(m);
    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_EnvironmentalImpactFactors);
    EXPECT_EQ(0u, idfObjs.size());
  }

  FuelFactors fuelFactors(m);
  {
    Workspace w = ft.translateModel(m);
    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_EnvironmentalImpactFactors);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idf_o(idfObjs[0]);
    EXPECT_EQ("Hourly", idf_o.getString(Output_EnvironmentalImpactFactorsFields::ReportingFrequency).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_OutputEnvironmentalImpactFactors) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  // Not there, Model shouldn't have it either
  {
    Model m = rt.translateWorkspace(w);
    auto outputEnvironmentalImpactFactors = m.getConcreteModelObjects<openstudio::model::OutputEnvironmentalImpactFactors>();
    EXPECT_EQ(0, outputEnvironmentalImpactFactors.size());
  }

  OptionalWorkspaceObject _i_outputEnvironmentalImpactFactors = w.addObject(IdfObject(IddObjectType::Output_EnvironmentalImpactFactors));
  ASSERT_TRUE(_i_outputEnvironmentalImpactFactors);
  EXPECT_TRUE(_i_outputEnvironmentalImpactFactors->setString(Output_EnvironmentalImpactFactorsFields::ReportingFrequency, "Hourly"));

  {
    Model m = rt.translateWorkspace(w);
    auto outputEnvironmentalImpactFactors = m.getConcreteModelObjects<openstudio::model::OutputEnvironmentalImpactFactors>();
    ASSERT_EQ(1, outputEnvironmentalImpactFactors.size());
    auto outputEnvironmentalImpactFactor = outputEnvironmentalImpactFactors[0];
    EXPECT_EQ("Hourly", outputEnvironmentalImpactFactor.reportingFrequency());
  }
}
