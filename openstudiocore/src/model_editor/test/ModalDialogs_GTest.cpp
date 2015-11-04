/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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
