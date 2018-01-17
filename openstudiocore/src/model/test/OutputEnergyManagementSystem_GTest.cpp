/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

TEST_F(ModelFixture, EMSOutput_EMSOutput)
{
  Model model;

  // add Output:EnergyManagementSystem

  OutputEnergyManagementSystem var = model.getUniqueModelObject<OutputEnergyManagementSystem>();
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

