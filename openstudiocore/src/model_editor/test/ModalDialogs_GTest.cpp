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

#include "ModelEditorFixture.hpp"

#include "../ModalDialogs.hpp"
#include "../TestButton.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/Lights.hpp"
#include "../model/Lights_Impl.hpp"
#include "../model/LightsDefinition.hpp"
#include "../model/LightsDefinition_Impl.hpp"

#include <utilities/idd/OS_Lights_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <QObject>

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelEditorFixture, ModalDialogs_EmptyModel_Cancel)
{
  Model model;
  
  std::vector<IddObjectType> typesToDisplay;
  typesToDisplay.push_back(Space::iddObjectType());

  std::shared_ptr<ModelObjectSelectorDialog> modelObjectSelectorDialog(new ModelObjectSelectorDialog(typesToDisplay, model));

  ModelObjectSelectorDialogWatcher watcher(modelObjectSelectorDialog);
  EXPECT_FALSE(watcher.selectedModelObject());

  std::shared_ptr<TestButton> button(new TestButton);

  QObject::connect(button.get(), &TestButton::clicked, modelObjectSelectorDialog.get(), &ModelObjectSelectorDialog::onPushButtonCancel);

  button->doClick();

  EXPECT_FALSE(watcher.selectedModelObject());
}


TEST_F(ModelEditorFixture, ModalDialogs_EmptyModel_Ok)
{
  Model model;
  
  std::vector<IddObjectType> typesToDisplay;
  typesToDisplay.push_back(Space::iddObjectType());

  std::shared_ptr<ModelObjectSelectorDialog> modelObjectSelectorDialog(new ModelObjectSelectorDialog(typesToDisplay, model));

  ModelObjectSelectorDialogWatcher watcher(modelObjectSelectorDialog);
  EXPECT_FALSE(watcher.selectedModelObject());

  std::shared_ptr<TestButton> button(new TestButton);

  QObject::connect(button.get(), &TestButton::clicked, modelObjectSelectorDialog.get(), &ModelObjectSelectorDialog::onPushButtonOK);

  button->doClick();

  EXPECT_FALSE(watcher.selectedModelObject());
}


TEST_F(ModelEditorFixture, ModalDialogs_Cancel)
{
  Model model;
  Space space1(model);
  Space space2(model);
  
  std::vector<IddObjectType> typesToDisplay;
  typesToDisplay.push_back(Space::iddObjectType());

  std::shared_ptr<ModelObjectSelectorDialog> modelObjectSelectorDialog(new ModelObjectSelectorDialog(typesToDisplay, model));

  ModelObjectSelectorDialogWatcher watcher(modelObjectSelectorDialog);
  ASSERT_TRUE(watcher.selectedModelObject());
  EXPECT_EQ(space1.handle(), watcher.selectedModelObject()->handle());

  std::shared_ptr<TestButton> button(new TestButton);

  QObject::connect(button.get(), &TestButton::clicked, modelObjectSelectorDialog.get(), &ModelObjectSelectorDialog::onPushButtonCancel);
  
  button->doClick();

  EXPECT_FALSE(watcher.selectedModelObject());
}


TEST_F(ModelEditorFixture, ModalDialogs_Ok)
{
  Model model;
  Space space1(model);
  Space space2(model);

  std::vector<IddObjectType> typesToDisplay;
  typesToDisplay.push_back(Space::iddObjectType());

  std::shared_ptr<ModelObjectSelectorDialog> modelObjectSelectorDialog(new ModelObjectSelectorDialog(typesToDisplay, model));

  ModelObjectSelectorDialogWatcher watcher(modelObjectSelectorDialog);
  ASSERT_TRUE(watcher.selectedModelObject());
  EXPECT_EQ(space1.handle(), watcher.selectedModelObject()->handle());

  std::shared_ptr<TestButton> button(new TestButton);

  QObject::connect(button.get(), &TestButton::clicked, modelObjectSelectorDialog.get(), &ModelObjectSelectorDialog::onPushButtonOK);

  button->doClick();

  ASSERT_TRUE(watcher.selectedModelObject());
  EXPECT_EQ(space1.handle(), watcher.selectedModelObject()->handle());
}


TEST_F(ModelEditorFixture, ModalDialogs_EnsureSpaceLoadDefinition_EmptyModel)
{
  Model model;
  EXPECT_TRUE(model.addObject(IdfObject(IddObjectType::OS_Lights)));

  ASSERT_EQ(1u, model.getModelObjects<Lights>().size());
  Lights light = model.getModelObjects<Lights>()[0];

  // does not have definition
  EXPECT_FALSE(light.getTarget(OS_LightsFields::LightsDefinitionName));
  EXPECT_THROW(light.definition(), openstudio::Exception);

  ensureSpaceLoadDefinition(light);

  ASSERT_TRUE(light.getTarget(OS_LightsFields::LightsDefinitionName));
  ASSERT_NO_THROW(light.definition());
  ASSERT_NO_THROW(light.lightsDefinition());
}

/* THIS IS A BLOCKING TEST
TEST_F(ModelEditorFixture, ModalDialogs_EnsureSpaceLoadDefinition)
{
  Model model;
  LightsDefinition definition1(model);
  LightsDefinition definition2(model);
  EXPECT_TRUE(model.addObject(IdfObject(IddObjectType::OS_Lights)));

  ASSERT_EQ(1u, model.getModelObjects<Lights>().size());
  Lights light = model.getModelObjects<Lights>()[0];

  // does not have definition
  EXPECT_FALSE(light.getTarget(OS_LightsFields::LightsDefinitionName));
  EXPECT_THROW(light.definition(), openstudio::Exception);

  ensureSpaceLoadDefinition(light);

  ASSERT_TRUE(light.getTarget(OS_LightsFields::LightsDefinitionName));
  ASSERT_NO_THROW(light.definition());
  ASSERT_NO_THROW(light.lightsDefinition());
}
*/

TEST_F(ModelEditorFixture, ModalDialogs_EnsureSpaceLoadDefinition_WithDefinition)
{
  Model model;
  LightsDefinition definition1(model);
  LightsDefinition definition2(model);
  Lights light(definition1);

  ensureSpaceLoadDefinition(light);

  ASSERT_TRUE(light.getTarget(OS_LightsFields::LightsDefinitionName));
  ASSERT_NO_THROW(light.definition());
  ASSERT_NO_THROW(light.lightsDefinition());
}
