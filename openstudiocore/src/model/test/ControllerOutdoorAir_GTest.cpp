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
/*
#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../ControllerOutdoorAir.hpp"

#include <utilities/idd/OS_Controller_OutdoorAir_FieldEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,ControllerOutdoorAir_IddKeys) {
  Model myModel;
  ControllerOutdoorAir controller(myModel);
  IddObject obj = controller.iddObject();
  OptionalIddField f = obj.getField(Controller_OutdoorAirFields::EconomizerControlType);
  ASSERT_TRUE(f);
  EXPECT_TRUE(f->properties().type == IddFieldType::ChoiceType);
  IddKeyVector keys = f->keys();
  EXPECT_EQ(static_cast<unsigned>(8),keys.size());
  LOG(Info,"Keys for field " << Controller_OutdoorAirFields::EconomizerControlType
      << ", " << f->name() << ", of IddObject " << obj.name() << ":");
  unsigned i = 0;
  for (IddKey& key : keys) {
    LOG(Info,"  Key " << i << ": " << key.name());
    ++i;
  }
}
*/
