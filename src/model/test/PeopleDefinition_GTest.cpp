/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../PeopleDefinition.hpp"
#include "../PeopleDefinition_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, PeopleDefinition_Clone) {
  Model library;
  Model model;

  PeopleDefinition definition(library);  // A ResourceObject

  EXPECT_EQ(1u, library.modelObjects().size());

  definition.clone(library);
  EXPECT_EQ(2u, library.modelObjects().size());

  definition.clone(model);
  EXPECT_EQ(1u, model.modelObjects().size());

  definition.clone(model);
  EXPECT_EQ(2u, model.modelObjects().size());
}
