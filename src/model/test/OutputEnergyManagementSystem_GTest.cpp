/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../OutputEnergyManagementSystem.hpp"
#include "../OutputEnergyManagementSystem_Impl.hpp"
#include "../OutputVariable.hpp"
#include "../OutputVariable_Impl.hpp"
#include "../OutputMeter.hpp"
#include "../OutputMeter_Impl.hpp"
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

TEST_F(ModelFixture, EMSOutput_EMSOutput) {
  Model model;

  // add Output:EnergyManagementSystem

  auto var = model.getUniqueModelObject<OutputEnergyManagementSystem>();
  bool defaulted = var.isActuatorAvailabilityDictionaryReportingDefaulted();
  EXPECT_EQ(true, defaulted);
  defaulted = var.isInternalVariableAvailabilityDictionaryReportingDefaulted();
  EXPECT_EQ(true, defaulted);
  defaulted = var.isEMSRuntimeLanguageDebugOutputLevelDefaulted();
  EXPECT_EQ(true, defaulted);
  var.setActuatorAvailabilityDictionaryReporting("NotByUniqueKeyNames");
  EXPECT_EQ("NotByUniqueKeyNames", var.actuatorAvailabilityDictionaryReporting());
  var.setInternalVariableAvailabilityDictionaryReporting("Verbose");
  EXPECT_EQ("Verbose", var.internalVariableAvailabilityDictionaryReporting());
  var.setEMSRuntimeLanguageDebugOutputLevel("Verbose");
  EXPECT_EQ("Verbose", var.eMSRuntimeLanguageDebugOutputLevel());
}
