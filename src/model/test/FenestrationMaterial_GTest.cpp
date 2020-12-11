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

#include "ModelFixture.hpp"
#include "../Model.hpp"
#include "../Blind.hpp"
#include "../Blind_Impl.hpp"
#include "../Gas.hpp"
#include "../Gas_Impl.hpp"
#include "../GasMixture.hpp"
#include "../GasMixture_Impl.hpp"
#include "../RefractionExtinctionGlazing.hpp"
#include "../RefractionExtinctionGlazing_Impl.hpp"
#include "../Screen.hpp"
#include "../Screen_Impl.hpp"
#include "../Shade.hpp"
#include "../Shade_Impl.hpp"
#include "../SimpleGlazing.hpp"
#include "../SimpleGlazing_Impl.hpp"
#include "../StandardGlazing.hpp"
#include "../StandardGlazing_Impl.hpp"
#include "../ThermochromicGlazing.hpp"
#include "../ThermochromicGlazing_Impl.hpp"
#include "../Model_Impl.hpp"

#include "../../energyplus/ReverseTranslator.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idd/IddKey.hpp"
#include <utilities/idd/OS_WindowMaterial_Blind_FieldEnums.hxx>
#include <utilities/idd/OS_WindowMaterial_Gas_FieldEnums.hxx>
#include <utilities/idd/OS_WindowMaterial_GasMixture_FieldEnums.hxx>
#include <utilities/idd/OS_WindowMaterial_Glazing_RefractionExtinctionMethod_FieldEnums.hxx>
#include <utilities/idd/OS_WindowMaterial_Screen_FieldEnums.hxx>
#include <utilities/idd/OS_WindowMaterial_Shade_FieldEnums.hxx>
#include <utilities/idd/OS_WindowMaterial_SimpleGlazingSystem_FieldEnums.hxx>
#include <utilities/idd/OS_WindowMaterial_Glazing_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;
/*
TEST_F(ModelFixture, FenestrationMaterial_Daylighting_School_1)
{
  // translate energyplus simulation to building model
  Workspace inWorkspace(daylightingIdfFile);
  energyplus::ReverseTranslator reverseTranslator(inWorkspace);
  OptionalModel optionalModel = reverseTranslator.convert();
  ASSERT_TRUE(optionalModel);
  Model model(*optionalModel);

  // get fenestration materials, sort by type, and log usage
  FenestrationMaterialVector fenestrationMaterials = model.getModelObjects<FenestrationMaterial>();
  ASSERT_FALSE(fenestrationMaterials.empty());
  IdfObjectTypeLess comparator;
  std::sort(fenestrationMaterials.begin(),fenestrationMaterials.end(),comparator);
  LOG(Info,"List of FenestrationMaterials in Daylighting School model, each "
      << "followed by their directUseCount(), nonResourceObjectUseCount().");
  for (const FenestrationMaterial& material : fenestrationMaterials) {
    unsigned absCount = material.directUseCount();
    unsigned mainCount = material.nonResourceObjectUseCount();
    std::string name = material.name().get();
    LOG(Info,material.iddObject().type() << ", '" << name << "': "
        << absCount << ", " << mainCount);
  }
}
*/
TEST_F(ModelFixture, FenestrationMaterial_Blind_Constructors) {
  // construct from scratch
  Model model;
  Blind blind(model);
  EXPECT_EQ(static_cast<unsigned>(1), model.objects().size());
  BlindVector blinds = model.getModelObjects<Blind>();
  ASSERT_EQ(static_cast<unsigned>(1), blinds.size());
  EXPECT_TRUE(blind == blinds[0]);
  EXPECT_TRUE(blind.iddObject().type() == IddObjectType::OS_WindowMaterial_Blind);

  // construct by clone
  Model modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1), modelClone.objects().size());
  blinds = modelClone.getModelObjects<Blind>();
  ASSERT_EQ(static_cast<unsigned>(1), blinds.size());
  blind = blinds[0];
  EXPECT_FALSE(blind.model() == model);
  EXPECT_TRUE(blind.iddObject().type() == IddObjectType::OS_WindowMaterial_Blind);
}

TEST_F(ModelFixture, FenestrationMaterial_Gas_Constructors) {
  // construct from scratch
  Model model;
  Gas gas(model);
  EXPECT_EQ(static_cast<unsigned>(1), model.objects().size());
  GasVector gases = model.getModelObjects<Gas>();
  ASSERT_EQ(static_cast<unsigned>(1), gases.size());
  EXPECT_TRUE(gas == gases[0]);
  EXPECT_TRUE(gas.iddObject().type() == IddObjectType::OS_WindowMaterial_Gas);

  // construct by clone
  Model modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1), modelClone.objects().size());
  gases = modelClone.getModelObjects<Gas>();
  ASSERT_EQ(static_cast<unsigned>(1), gases.size());
  gas = gases[0];
  EXPECT_FALSE(gas.model() == model);
  EXPECT_TRUE(gas.iddObject().type() == IddObjectType::OS_WindowMaterial_Gas);
}

// written to fail if Gas object's Gas Type field changes
TEST_F(ModelFixture, FenestrationMaterial_Gas_KeyFields) {

  Model model;
  Gas gas(model);

  IddField gasTypeIddField = gas.iddObject().getField(OS_WindowMaterial_GasFields::GasType).get();
  IddKeyVector gasTypeKeys = gasTypeIddField.keys();
  ASSERT_EQ(5u, gasTypeKeys.size());
  ASSERT_TRUE(gasTypeIddField.getKey("Air"));
  ASSERT_TRUE(gasTypeIddField.getKey("Argon"));
  ASSERT_TRUE(gasTypeIddField.getKey("Krypton"));
  ASSERT_TRUE(gasTypeIddField.getKey("Xenon"));
  ASSERT_TRUE(gasTypeIddField.getKey("Custom"));
}

TEST_F(ModelFixture, FenestrationMaterial_Gas_Validity) {
  Model model;
  Gas gas(model);

  EXPECT_TRUE(gas.isValid(StrictnessLevel::Draft));
  EXPECT_TRUE(gas.isValid(StrictnessLevel::Final));
  EXPECT_TRUE(model.isValid(StrictnessLevel::Draft));
  EXPECT_FALSE(model.isValid(StrictnessLevel::Final));

  EXPECT_TRUE(gas.name());
  EXPECT_TRUE(gas.setGasType("air"));
  EXPECT_TRUE(gas.setThickness(0.003));
  EXPECT_EQ("air", gas.gasType());
  EXPECT_TRUE(gas.isValid(StrictnessLevel::Draft));
  EXPECT_TRUE(gas.isValid(StrictnessLevel::Final));
  EXPECT_TRUE(model.isValid(StrictnessLevel::Draft));
  EXPECT_FALSE(model.isValid(StrictnessLevel::Final));

  EXPECT_TRUE(gas.setGasType("CusTom"));
  EXPECT_TRUE(gas.isValid(StrictnessLevel::Draft));
  EXPECT_TRUE(gas.isValid(StrictnessLevel::Final));
  EXPECT_TRUE(model.isValid(StrictnessLevel::Draft));
  EXPECT_FALSE(model.isValid(StrictnessLevel::Final));

  // make valid at level final with custom gas
  std::vector<double> coeffs = FenestrationMaterial::argonThermalConductivityCoefficients();
  EXPECT_TRUE(gas.setCustomConductivity(coeffs[0], coeffs[1], coeffs[2]));
  coeffs = FenestrationMaterial::xenonViscosityCoefficients();
  EXPECT_TRUE(gas.setCustomViscosity(coeffs[0], coeffs[1], coeffs[2]));
  coeffs = FenestrationMaterial::airSpecificHeatCoefficients();
  EXPECT_TRUE(gas.setCustomSpecificHeat(coeffs[0], coeffs[1], coeffs[2]));
  EXPECT_TRUE(gas.setCustomMolecularWeight(55.0));

  // verify expected validity
  EXPECT_TRUE(gas.isValid(StrictnessLevel::Draft));
  EXPECT_TRUE(gas.isValid(StrictnessLevel::Final));
  EXPECT_TRUE(model.isValid(StrictnessLevel::Draft));
  EXPECT_FALSE(model.isValid(StrictnessLevel::Final));
}

TEST_F(ModelFixture, FenestrationMaterial_GasMixture_Constructors) {
  // construct from scratch
  Model model;
  GasMixture gasMixture(model);
  EXPECT_EQ(static_cast<unsigned>(1), model.objects().size());
  GasMixtureVector gasMixtures = model.getModelObjects<GasMixture>();
  ASSERT_EQ(static_cast<unsigned>(1), gasMixtures.size());
  EXPECT_TRUE(gasMixture == gasMixtures[0]);
  EXPECT_TRUE(gasMixture.iddObject().type() == IddObjectType::OS_WindowMaterial_GasMixture);

  // construct by clone
  Model modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1), modelClone.objects().size());
  gasMixtures = modelClone.getModelObjects<GasMixture>();
  ASSERT_EQ(static_cast<unsigned>(1), gasMixtures.size());
  gasMixture = gasMixtures[0];
  EXPECT_FALSE(gasMixture.model() == model);
  EXPECT_TRUE(gasMixture.iddObject().type() == IddObjectType::OS_WindowMaterial_GasMixture);
}

// written to fail if GasMixture's IddObject changes in ways that will break the ModelObject
TEST_F(ModelFixture, FenestrationMaterial_GasMixture_IddAssumptions) {

  Model model;
  GasMixture gasMixture(model);

  IddField gasTypeIddField = gasMixture.iddObject().getField(OS_WindowMaterial_GasMixtureFields::Gas1Type).get();
  IddKeyVector gasTypeKeys = gasTypeIddField.keys();
  ASSERT_EQ(4u, gasTypeKeys.size());
  ASSERT_TRUE(gasTypeIddField.getKey("Air"));
  ASSERT_TRUE(gasTypeIddField.getKey("Argon"));
  ASSERT_TRUE(gasTypeIddField.getKey("Krypton"));
  ASSERT_TRUE(gasTypeIddField.getKey("Xenon"));

  ASSERT_EQ(12u, gasMixture.iddObject().numFields());
}

TEST_F(ModelFixture, FenestrationMaterial_RefractionExtinctionGlazing_Constructors) {
  // construct from scratch
  Model model;
  RefractionExtinctionGlazing glazing(model);
  EXPECT_EQ(static_cast<unsigned>(1), model.objects().size());
  RefractionExtinctionGlazingVector glazings = model.getModelObjects<RefractionExtinctionGlazing>();
  ASSERT_EQ(static_cast<unsigned>(1), glazings.size());
  EXPECT_TRUE(glazing == glazings[0]);
  EXPECT_TRUE(glazing.iddObject().type() == IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod);

  // construct by clone
  Model modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1), modelClone.objects().size());
  glazings = modelClone.getModelObjects<RefractionExtinctionGlazing>();
  ASSERT_EQ(static_cast<unsigned>(1), glazings.size());
  glazing = glazings[0];
  EXPECT_FALSE(glazing.model() == model);
  EXPECT_TRUE(glazing.iddObject().type() == IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod);
}

TEST_F(ModelFixture, FenestrationMaterial_Screen_Constructors) {
  // construct from scratch
  Model model;
  Screen screen(model);
  EXPECT_EQ(static_cast<unsigned>(1), model.objects().size());
  ScreenVector screens = model.getModelObjects<Screen>();
  ASSERT_EQ(static_cast<unsigned>(1), screens.size());
  EXPECT_TRUE(screen == screens[0]);
  EXPECT_TRUE(screen.iddObject().type() == IddObjectType::OS_WindowMaterial_Screen);

  // construct by clone
  Model modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1), modelClone.objects().size());
  screens = modelClone.getModelObjects<Screen>();
  ASSERT_EQ(static_cast<unsigned>(1), screens.size());
  screen = screens[0];
  EXPECT_FALSE(screen.model() == model);
  EXPECT_TRUE(screen.iddObject().type() == IddObjectType::OS_WindowMaterial_Screen);
}

TEST_F(ModelFixture, FenestrationMaterial_Shade_Constructors) {
  // construct from scratch
  Model model;
  Shade shade(model);
  EXPECT_EQ(static_cast<unsigned>(1), model.objects().size());
  ShadeVector shades = model.getModelObjects<Shade>();
  ASSERT_EQ(static_cast<unsigned>(1), shades.size());
  EXPECT_TRUE(shade == shades[0]);
  EXPECT_TRUE(shade.iddObject().type() == IddObjectType::OS_WindowMaterial_Shade);

  // construct by clone
  Model modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1), modelClone.objects().size());
  shades = modelClone.getModelObjects<Shade>();
  ASSERT_EQ(static_cast<unsigned>(1), shades.size());
  shade = shades[0];
  EXPECT_FALSE(shade.model() == model);
  EXPECT_TRUE(shade.iddObject().type() == IddObjectType::OS_WindowMaterial_Shade);
}

TEST_F(ModelFixture, FenestrationMaterial_SimpleGlazing_Constructors) {
  // construct from scratch
  Model model;
  SimpleGlazing glazing(model);
  EXPECT_EQ(static_cast<unsigned>(1), model.objects().size());
  SimpleGlazingVector glazings = model.getModelObjects<SimpleGlazing>();
  ASSERT_EQ(static_cast<unsigned>(1), glazings.size());
  EXPECT_TRUE(glazing == glazings[0]);
  EXPECT_TRUE(glazing.iddObject().type() == IddObjectType::OS_WindowMaterial_SimpleGlazingSystem);

  // construct by clone
  Model modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1), modelClone.objects().size());
  glazings = modelClone.getModelObjects<SimpleGlazing>();
  ASSERT_EQ(static_cast<unsigned>(1), glazings.size());
  glazing = glazings[0];
  EXPECT_FALSE(glazing.model() == model);
  EXPECT_TRUE(glazing.iddObject().type() == IddObjectType::OS_WindowMaterial_SimpleGlazingSystem);
}

TEST_F(ModelFixture, FenestrationMaterial_StandardGlazing_Constructors) {
  // construct from scratch
  Model model;
  StandardGlazing glazing(model);
  EXPECT_EQ(static_cast<unsigned>(1), model.objects().size());
  StandardGlazingVector glazings = model.getModelObjects<StandardGlazing>();
  ASSERT_EQ(static_cast<unsigned>(1), glazings.size());
  EXPECT_TRUE(glazing == glazings[0]);
  EXPECT_TRUE(glazing.iddObject().type() == IddObjectType::OS_WindowMaterial_Glazing);

  // construct by clone
  Model modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1), modelClone.objects().size());
  glazings = modelClone.getModelObjects<StandardGlazing>();
  ASSERT_EQ(static_cast<unsigned>(1), glazings.size());
  glazing = glazings[0];
  EXPECT_FALSE(glazing.model() == model);
  EXPECT_TRUE(glazing.iddObject().type() == IddObjectType::OS_WindowMaterial_Glazing);
}

TEST_F(ModelFixture, FenestrationMaterial_ThermochromicGlazing_Constructors) {
  // construct from scratch
  Model model;
  ThermochromicGlazing glazing(model);
  EXPECT_EQ(static_cast<unsigned>(1), model.objects().size());
  ThermochromicGlazingVector glazings = model.getModelObjects<ThermochromicGlazing>();
  ASSERT_EQ(static_cast<unsigned>(1), glazings.size());
  EXPECT_TRUE(glazing == glazings[0]);
  EXPECT_TRUE(glazing.iddObject().type() == IddObjectType::OS_WindowMaterial_GlazingGroup_Thermochromic);

  // construct by clone
  Model modelClone = model.clone().cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1), modelClone.objects().size());
  glazings = modelClone.getModelObjects<ThermochromicGlazing>();
  ASSERT_EQ(static_cast<unsigned>(1), glazings.size());
  glazing = glazings[0];
  EXPECT_FALSE(glazing.model() == model);
  EXPECT_TRUE(glazing.iddObject().type() == IddObjectType::OS_WindowMaterial_GlazingGroup_Thermochromic);
}

TEST_F(ModelFixture, FenestrationMaterial_ThermochromicGlazing_Attributes) {
  // construct
  Model model;
  ThermochromicGlazing glazing(model);

  StandardGlazing glazingAtTemp1(model);
  EXPECT_TRUE(glazingAtTemp1.setThickness(0.001));
  StandardGlazing glazingAtTemp2(model);
  EXPECT_TRUE(glazingAtTemp2.setThickness(0.002));
  StandardGlazing glazingAtTemp3(model);
  EXPECT_TRUE(glazingAtTemp3.setThickness(0.003));

  StringVector groupValues(2);
  groupValues[0] = "0";
  groupValues[1] = glazingAtTemp1.name().get();
  glazing.pushExtensibleGroup(groupValues);
  groupValues[0] = "20";
  groupValues[1] = glazingAtTemp2.name().get();
  glazing.pushExtensibleGroup(groupValues);
  groupValues[0] = "40";
  groupValues[1] = glazingAtTemp3.name().get();
  glazing.pushExtensibleGroup(groupValues);
}
