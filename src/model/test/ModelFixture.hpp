/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_TEST_MODELFIXTURE_HPP
#define MODEL_TEST_MODELFIXTURE_HPP

#include <gtest/gtest.h>

#include <resources.hxx>

#include "../Model.hpp"

#include "../../utilities/idd/IddField.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/FileLogSink.hpp"
#include "../../utilities/data/DataEnums.hpp"

class ModelFixture : public ::testing::Test
{
 protected:
  // initialize for each test
  virtual void SetUp() override;

  // tear down after each test
  virtual void TearDown() override;

  // initialize static members
  static void SetUpTestSuite();

  // tear down static members
  static void TearDownTestSuite();

  // test that the given iddField is a boolean Yes/No choice field
  void testBooleanIddField(const openstudio::IddField& iddField, const boost::optional<std::string>& defaultValue);

  bool testFuelTypeEquality(const std::vector<openstudio::FuelType>& expectedFuelTypes, const std::vector<openstudio::FuelType>& actualFuelTypes);

  bool testAppGFuelTypeEquality(const std::vector<openstudio::AppGFuelType>& expectedAppGFuelTypes,
                                const std::vector<openstudio::AppGFuelType>& actualAppGFuelTypes);

  // set up logging
  REGISTER_LOGGER("ModelFixture");

  // Note: storage for static variables must be defined in a separate .cpp file
  static double tol;
  static boost::optional<openstudio::FileLogSink> logFile;
};

#endif  // MODEL_TEST_MODELFIXTURE_HPP
