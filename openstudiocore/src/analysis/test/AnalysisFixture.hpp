/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef ANALYSIS_TEST_ANALYSISFIXTURE_HPP
#define ANALYSIS_TEST_ANALYSISFIXTURE_HPP

#include <gtest/gtest.h>

#include "../../model/Model.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/FileLogSink.hpp"

namespace openstudio {
namespace analysis {
  class Analysis;
}
}

class AnalysisFixture : public ::testing::Test {
 protected:
  /// initialize for each test
  virtual void SetUp() override;

  /// tear down after each test
  virtual void TearDown() override;

  /// initialize static members
  static void SetUpTestCase();

  /// tear down static members
  static void TearDownTestCase();

  REGISTER_LOGGER("AnalysisFixture");

  // static variables
  static boost::optional<openstudio::FileLogSink> logFile;

  enum AnalysisState { PreRun, PostRun };

  // Test case with one MeasureGroup and two RubyContinuousVariables that use
  // the same measure. In the PostRun state, a complete DataPoint is included.
  static openstudio::analysis::Analysis analysis1(AnalysisState state);

  // Test case with two RubyContinousVariables attached to two different 
  // measures, with or without a simulation workflow.
  static openstudio::analysis::Analysis analysis2(bool simulate);
};

#endif // ANALYSIS_TEST_ANALYSISFIXTURE_HPP
