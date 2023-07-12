/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

  ASSERT_EQ(1u, model.getConcreteModelObjects<Construction>().size());
  Construction mConstruction = model.getConcreteModelObjects<Construction>()[0];
  EXPECT_EQ(3u, mConstruction.layers().size());
  EXPECT_EQ(2u, model.getModelObjects<FenestrationMaterial>().size());
}
