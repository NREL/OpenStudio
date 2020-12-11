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

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/FenestrationMaterial.hpp"
#include "../../model/FenestrationMaterial_Impl.hpp"
#include "../../model/StandardGlazing.hpp"
#include "../../model/StandardGlazing_Impl.hpp"
#include "../../model/Gas.hpp"
#include "../../model/Gas_Impl.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/UUID.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Containers.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Construction_FieldEnums.hxx>
#include <utilities/idd/WindowMaterial_Glazing_FieldEnums.hxx>
#include <utilities/idd/WindowMaterial_Gas_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::energyplus;

TEST_F(EnergyPlusFixture, ReverseTranslator_WindowConstruction) {
  StrictnessLevel level(StrictnessLevel::Draft);
  IddFileType iddFileType(IddFileType::EnergyPlus);
  Workspace workspace(level, iddFileType);

  IdfObject glazing(IddObjectType::WindowMaterial_Glazing);
  IdfObject gas(IddObjectType::WindowMaterial_Gas);
  glazing.setName("Glazing Material");
  gas.setName("Gas Material");
  IdfObject construction(IddObjectType::Construction);
  ASSERT_EQ(0u, construction.numExtensibleGroups());
  EXPECT_FALSE(construction.pushExtensibleGroup(StringVector(1u, glazing.name().get())).empty());
  EXPECT_FALSE(construction.pushExtensibleGroup(StringVector(1u, gas.name().get())).empty());
  EXPECT_FALSE(construction.pushExtensibleGroup(StringVector(1u, glazing.name().get())).empty());
  IdfObjectVector objects;
  objects.push_back(glazing);
  objects.push_back(gas);
  objects.push_back(construction);
  EXPECT_EQ(3u, workspace.addObjects(objects).size());

  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(workspace);

  ASSERT_EQ(1u, model.getModelObjects<Construction>().size());
  Construction mConstruction = model.getModelObjects<Construction>()[0];
  EXPECT_EQ(3u, mConstruction.layers().size());
  EXPECT_EQ(2u, model.getModelObjects<FenestrationMaterial>().size());
}
