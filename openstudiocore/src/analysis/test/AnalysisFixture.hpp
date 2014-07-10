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
  virtual void SetUp();

  /// tear down after each test
  virtual void TearDown();

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
