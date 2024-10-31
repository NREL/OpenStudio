/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "IddFixture.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/Controller_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../IddField.hpp"
#include "../IddKey.hpp"

using namespace openstudio;

TEST_F(IddFixture, IddKey_Getters) {
  OptionalIddObject optObject = IddFactory::instance().getObject(IddObjectType::Controller_OutdoorAir);
  ASSERT_TRUE(optObject);
  IddObject obj = *optObject;
  OptionalIddField f = obj.getField(Controller_OutdoorAirFields::EconomizerControlType);
  ASSERT_TRUE(f);
  EXPECT_TRUE(f->properties().type == IddFieldType::ChoiceType);
  IddKeyVector keys = f->keys();
  EXPECT_EQ(static_cast<unsigned>(8), keys.size());
  LOG(Info, "Keys for field " << Controller_OutdoorAirFields::EconomizerControlType << ", " << f->name() << ", of IddObject " << obj.name() << ":");
  unsigned i = 0;
  for (const IddKey& key : keys) {
    LOG(Info, "  Key " << i << ": " << key.name());
    ++i;
  }
}
