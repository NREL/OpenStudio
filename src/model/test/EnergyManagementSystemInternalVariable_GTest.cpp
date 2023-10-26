/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../EnergyManagementSystemInternalVariable.hpp"
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

TEST_F(ModelFixture, EMSInternalVariable_EMSInternalVariable) {
  Model model;

  // add internal variable
  EnergyManagementSystemInternalVariable var(model, "TestName");
  var.setInternalDataIndexKeyName("TestName");
  EXPECT_EQ("TestName", var.internalDataIndexKeyName());
  var.resetInternalDataIndexKeyName();
  EXPECT_EQ("", var.internalDataIndexKeyName());
  //var.setInternalDataType("TestName");
  EXPECT_EQ("TestName", var.internalDataType());
}
