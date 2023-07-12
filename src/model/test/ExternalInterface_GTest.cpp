/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ExternalInterface.hpp"
#include "../ExternalInterface_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, ExternalInterface) {
  Model model;

  auto externalinterface = model.getUniqueModelObject<ExternalInterface>();
  EXPECT_EQ("PtolemyServer", externalinterface.nameofExternalInterface());
  EXPECT_FALSE(externalinterface.setNameofExternalInterface("bad value"));
  EXPECT_TRUE(externalinterface.setNameofExternalInterface("FunctionalMockupUnitImport"));
  EXPECT_EQ("FunctionalMockupUnitImport", externalinterface.nameofExternalInterface());
}
