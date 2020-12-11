/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/ConstructionWithInternalSource.hpp"
#include "../../model/ConstructionWithInternalSource_Impl.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/Construction_InternalSource_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ConstructionWithInternalSource) {

  ForwardTranslator ft;

  Model model;

  ConstructionWithInternalSource construction(model);
  construction.setName("Construction Internal Source 1");
  construction.setTubeSpacing(0.5);
  construction.setTwoDimensionalTemperatureCalculationPosition(0.75);

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Construction_InternalSource).size());

  std::vector<WorkspaceObject> objVector(workspace.getObjectsByType(IddObjectType::Construction_InternalSource));
  ASSERT_EQ(1u, objVector.size());
  WorkspaceObject wo(objVector.at(0));

  EXPECT_EQ("Construction Internal Source 1", wo.getString(Construction_InternalSourceFields::Name).get());
  EXPECT_EQ(1, wo.getInt(Construction_InternalSourceFields::SourcePresentAfterLayerNumber).get());
  EXPECT_EQ(1, wo.getInt(Construction_InternalSourceFields::TemperatureCalculationRequestedAfterLayerNumber).get());
  EXPECT_EQ(1, wo.getInt(Construction_InternalSourceFields::DimensionsfortheCTFCalculation).get());
  EXPECT_EQ(0.5, wo.getDouble(Construction_InternalSourceFields::TubeSpacing).get());
  EXPECT_EQ(0.75, wo.getDouble(Construction_InternalSourceFields::TwoDimensionalTemperatureCalculationPosition).get());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ConstructionWithInternalSource) {

  Workspace workspace(StrictnessLevel::None, IddFileType::EnergyPlus);

  IdfObject idfObject1(IddObjectType::Construction_InternalSource);
  idfObject1.setString(Construction_InternalSourceFields::Name, "Construction Internal Source 1");
  idfObject1.setDouble(Construction_InternalSourceFields::TubeSpacing, 0.5);
  idfObject1.setDouble(Construction_InternalSourceFields::TwoDimensionalTemperatureCalculationPosition, 0.75);

  WorkspaceObject epConstr = workspace.addObject(idfObject1).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<ConstructionWithInternalSource> constructions = model.getModelObjects<ConstructionWithInternalSource>();
  ASSERT_EQ(1u, constructions.size());
  ConstructionWithInternalSource construction = constructions[0];
  EXPECT_EQ(0, construction.numLayers());
  EXPECT_EQ(0, construction.layers().size());
  EXPECT_EQ(1, construction.sourcePresentAfterLayerNumber());
  EXPECT_EQ(1, construction.temperatureCalculationRequestedAfterLayerNumber());
  EXPECT_EQ(1, construction.dimensionsForTheCTFCalculation());
  EXPECT_EQ(0.5, construction.tubeSpacing());
  EXPECT_EQ(0.75, construction.twoDimensionalTemperatureCalculationPosition());
}
