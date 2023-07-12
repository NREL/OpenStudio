/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../EnergyManagementSystemGlobalVariable.hpp"
#include "../Model_Impl.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idf/ValidityReport.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/data/TimeSeries.hpp"
#include "../../utilities/core/Compare.hpp"
#include "../../utilities/core/Optional.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, EMSGlobalVariable_EMSGlobalVariable) {
  Model model;

  // add global variable
  unsigned n1 = model.objects().size();
  EnergyManagementSystemGlobalVariable var(model, "glob_var");
  EXPECT_EQ("glob_var", var.nameString());
  unsigned n2 = model.objects().size();
  EXPECT_EQ(n1 + 1, n2);

  //should crash and not create any new objects
  n1 = model.objects().size();
  EXPECT_ANY_THROW(EnergyManagementSystemGlobalVariable var(model, "glob var"));
  n2 = model.objects().size();
  EXPECT_EQ(n1, n2);
}
