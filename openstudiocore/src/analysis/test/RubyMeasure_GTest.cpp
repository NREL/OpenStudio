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

#include <gtest/gtest.h>
#include "AnalysisFixture.hpp"

#include "../RubyMeasure.hpp"

#include "../../ruleset/OSArgument.hpp"

#include "../../utilities/bcl/BCLMeasure.hpp"

#include "../../utilities/core/Containers.hpp"
#include "../../utilities/core/FileReference.hpp"

#include <OpenStudio.hxx>
#include <resources.hxx>

#include <boost/filesystem.hpp>

using namespace openstudio;
using namespace openstudio::ruleset;
using namespace openstudio::analysis;

TEST_F(AnalysisFixture, RubyMeasure_FreeFormScript) {
  // construct
  openstudio::path scriptPath = toPath("myCrazyScript.rb");
  RubyMeasure measure(scriptPath,
                      FileReferenceType::OSM,
                      FileReferenceType::OSM);
  EXPECT_EQ(FileReferenceType(FileReferenceType::OSM),measure.inputFileType());
  EXPECT_EQ(FileReferenceType(FileReferenceType::OSM),measure.outputFileType());
  EXPECT_FALSE(measure.usesBCLMeasure());
  EXPECT_ANY_THROW(measure.measure());
  EXPECT_ANY_THROW(measure.measureDirectory());
  EXPECT_ANY_THROW(measure.measureUUID());
  EXPECT_ANY_THROW(measure.measureVersionUUID());
  FileReference scriptReference = measure.perturbationScript();
  EXPECT_EQ(scriptPath,scriptReference.path().filename());
  EXPECT_EQ(boost::filesystem::system_complete(scriptPath),scriptReference.path());
  EXPECT_EQ(FileReferenceType(FileReferenceType::RB),scriptReference.fileType());
  EXPECT_TRUE(measure.arguments().empty());
  EXPECT_FALSE(measure.isUserScript());
  EXPECT_FALSE(measure.hasIncompleteArguments());

  // add arguments the old way
  measure.addArgument("verbose");
  measure.addArgument("numRandomChanges","10");

  // verify status of arguments
  EXPECT_EQ(2u,measure.arguments().size());
  EXPECT_TRUE(measure.hasIncompleteArguments()); // name-only argument has no value
  OSArgumentVector incompleteArgs = measure.incompleteArguments();
  ASSERT_EQ(1u,incompleteArgs.size());
  EXPECT_EQ("verbose",incompleteArgs[0].name());
}

TEST_F(AnalysisFixture, RubyMeasure_FreeStandingUserScript) {
  // construct
  openstudio::path scriptPath = toPath("myUserScript.rb");
  RubyMeasure measure(scriptPath,
                                FileReferenceType::IDF,
                                FileReferenceType::IDF,
                                true);
  EXPECT_EQ(FileReferenceType(FileReferenceType::IDF),measure.inputFileType());
  EXPECT_EQ(FileReferenceType(FileReferenceType::IDF),measure.outputFileType());
  EXPECT_FALSE(measure.usesBCLMeasure());
  EXPECT_ANY_THROW(measure.measure());
  EXPECT_ANY_THROW(measure.measureDirectory());
  EXPECT_ANY_THROW(measure.measureUUID());
  EXPECT_ANY_THROW(measure.measureVersionUUID());
  FileReference scriptReference = measure.perturbationScript();
  EXPECT_EQ(scriptPath,scriptReference.path().filename());
  EXPECT_EQ(boost::filesystem::system_complete(scriptPath),scriptReference.path());
  EXPECT_EQ(FileReferenceType(FileReferenceType::RB),scriptReference.fileType());
  EXPECT_TRUE(measure.arguments().empty());
  EXPECT_TRUE(measure.isUserScript());
  EXPECT_FALSE(measure.hasIncompleteArguments());

  // add arguments to fill in
  OSArgument numBoilers = OSArgument::makeIntegerArgument("numBoilers");
  OSArgument numChillers = OSArgument::makeIntegerArgument("numChillers");
  StringVector choices;
  choices.push_back("parallel");
  choices.push_back("series");
  OSArgument topology = OSArgument::makeChoiceArgument("topology",choices);
  EXPECT_TRUE(topology.setDefaultValue("parallel"));
  measure.setArgument(numBoilers);
  measure.setArgument(numChillers);
  measure.setArgument(topology);

  // verify that arguments are incomplete
  EXPECT_EQ(3u,measure.arguments().size());
  EXPECT_EQ(2u,measure.incompleteArguments().size());
  EXPECT_TRUE(measure.hasIncompleteArguments());

  // set to different script and verify that arguments cleared
  scriptPath = toPath("myOtherUserScript.rb");
  measure.setPerturbationScript(scriptPath,
                                FileReferenceType::OSM,
                                FileReferenceType::OSM,
                                true);
  EXPECT_EQ(FileReferenceType(FileReferenceType::OSM),measure.inputFileType());
  EXPECT_EQ(FileReferenceType(FileReferenceType::OSM),measure.outputFileType());
  scriptReference = measure.perturbationScript();
  EXPECT_EQ(scriptPath,scriptReference.path().filename());
  EXPECT_TRUE(measure.arguments().empty());
  EXPECT_TRUE(measure.isUserScript());
  EXPECT_FALSE(measure.hasIncompleteArguments());
}

TEST_F(AnalysisFixture, RubyMeasure_BCLMeasure) {
  // construct
  openstudio::path measuresPath = resourcesPath() / toPath("/utilities/BCL/Measures");
  openstudio::path dir = measuresPath / toPath("SetWindowToWallRatioByFacade");
  BCLMeasure bclMeasure = BCLMeasure::load(dir).get();
  RubyMeasure measure(bclMeasure);
  EXPECT_TRUE(measure.usesBCLMeasure());
  ASSERT_TRUE(measure.bclMeasure());
  EXPECT_TRUE(measure.bclMeasure().get() == bclMeasure);
  EXPECT_EQ(dir,measure.bclMeasureDirectory());
  EXPECT_TRUE(bclMeasure.uuid() == measure.bclMeasureUUID());
  EXPECT_TRUE(bclMeasure.versionUUID() == measure.bclMeasureVersionUUID());
  EXPECT_ANY_THROW(measure.perturbationScript());
  // isUserScript value is ignored in this case
  EXPECT_TRUE(measure.arguments().empty());
  EXPECT_FALSE(measure.hasIncompleteArguments());

  // add arguments to fill in
  OSArgumentVector args;
  args.push_back(OSArgument::makeDoubleArgument("wwr"));
  args.push_back(OSArgument::makeDoubleArgument("sillHeight"));
  measure.setArguments(args);

  // verify that arguments are incomplete
  EXPECT_EQ(2u,measure.incompleteArguments().size());

  // fill in argument values
  OSArgument arg = OSArgument::makeDoubleArgument("wwr");
  EXPECT_TRUE(arg.setValue(0.8));
  measure.setArgument(arg);
  EXPECT_EQ(2u,measure.arguments().size());
  EXPECT_EQ(1u,measure.incompleteArguments().size());
  EXPECT_TRUE(measure.hasIncompleteArguments());
  arg = OSArgument::makeDoubleArgument("sillHeight");
  arg.setValue("0.1");
  measure.setArgument(arg);
  EXPECT_EQ(2u,measure.arguments().size());
  EXPECT_EQ(0u,measure.incompleteArguments().size());
  EXPECT_FALSE(measure.hasIncompleteArguments());

  // update measure
  openstudio::path tempDir = measuresPath / toPath(toString(createUUID()));
  {
    BCLMeasure newVersion = bclMeasure.clone(tempDir).get();
    newVersion.setDescription("Window to wall ratio by wwr and offset.");
    newVersion.save();
    EXPECT_TRUE(newVersion.uuid() == bclMeasure.uuid());
    EXPECT_FALSE(newVersion.versionUUID() == bclMeasure.versionUUID());
    args.pop_back();
    args.push_back(OSArgument::makeDoubleArgument("offset"));
    measure.updateMeasure(newVersion,args);
    EXPECT_TRUE(measure.usesBCLMeasure());
    ASSERT_TRUE(measure.bclMeasure());
    EXPECT_TRUE(measure.bclMeasure().get() == newVersion);
    EXPECT_EQ(tempDir,measure.bclMeasureDirectory());
    EXPECT_TRUE(newVersion.uuid() == measure.bclMeasureUUID());
    EXPECT_TRUE(newVersion.versionUUID() == measure.bclMeasureVersionUUID());
    EXPECT_ANY_THROW(measure.perturbationScript());

    // verify that arguments updated correctly, values retained
    EXPECT_EQ(2u,measure.arguments().size());
    ASSERT_EQ(1u,measure.incompleteArguments().size());
    EXPECT_TRUE(measure.hasIncompleteArguments());
    EXPECT_EQ("offset",measure.incompleteArguments()[0].name());
    EXPECT_EQ("wwr",measure.arguments()[0].name());
    EXPECT_EQ(0.8,measure.arguments()[0].valueAsDouble());

    // set measure and verify that arguments cleared
    measure.setMeasure(newVersion); // always goes through setting motions, even if same
    EXPECT_TRUE(measure.usesBCLMeasure());
    ASSERT_TRUE(measure.bclMeasure());
    EXPECT_TRUE(measure.bclMeasure().get() == newVersion);
    EXPECT_EQ(tempDir,measure.bclMeasureDirectory());
    EXPECT_TRUE(newVersion.uuid() == measure.bclMeasureUUID());
    EXPECT_TRUE(newVersion.versionUUID() == measure.bclMeasureVersionUUID());
    EXPECT_ANY_THROW(measure.perturbationScript());
    // isUserScript value is ignored in this case
    EXPECT_TRUE(measure.arguments().empty());
    EXPECT_FALSE(measure.hasIncompleteArguments());
  }
  boost::filesystem::remove_all(tempDir);
}
