/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#include <analysis/test/AnalysisFixture.hpp>

#include <analysis/RubyPerturbation.hpp>

#include <ruleset/OSArgument.hpp>

#include <utilities/bcl/BCLMeasure.hpp>

#include <utilities/core/Containers.hpp>
#include <utilities/core/FileReference.hpp>

#include <OpenStudio.hxx>
#include <resources.hxx>

#include <boost/filesystem.hpp>

using namespace openstudio;
using namespace openstudio::ruleset;
using namespace openstudio::analysis;

TEST_F(AnalysisFixture, RubyPerturbation_FreeFormScript) {
  // construct
  openstudio::path scriptPath = toPath("myCrazyScript.rb");
  RubyPerturbation perturbation(scriptPath,
                                FileReferenceType::OSM,
                                FileReferenceType::OSM);
  EXPECT_EQ(FileReferenceType(FileReferenceType::OSM),perturbation.inputFileType());
  EXPECT_EQ(FileReferenceType(FileReferenceType::OSM),perturbation.outputFileType());
  EXPECT_FALSE(perturbation.usesBCLMeasure());
  EXPECT_ANY_THROW(perturbation.measure());
  EXPECT_ANY_THROW(perturbation.measureDirectory());
  EXPECT_ANY_THROW(perturbation.measureUUID());
  EXPECT_ANY_THROW(perturbation.measureVersionUUID());
  FileReference scriptReference = perturbation.perturbationScript();
  EXPECT_EQ(scriptPath,scriptReference.path().filename());
  EXPECT_EQ(boost::filesystem::system_complete(scriptPath),scriptReference.path());
  EXPECT_EQ(FileReferenceType(FileReferenceType::RB),scriptReference.fileType());
  EXPECT_TRUE(perturbation.arguments().empty());
  EXPECT_FALSE(perturbation.isUserScript());
  EXPECT_FALSE(perturbation.hasIncompleteArguments());

  // add arguments the old way
  perturbation.addArgument("verbose");
  perturbation.addArgument("numRandomChanges","10");

  // verify status of arguments
  EXPECT_EQ(2u,perturbation.arguments().size());
  EXPECT_TRUE(perturbation.hasIncompleteArguments()); // name-only argument has no value
  OSArgumentVector incompleteArgs = perturbation.incompleteArguments();
  ASSERT_EQ(1u,incompleteArgs.size());
  EXPECT_EQ("verbose",incompleteArgs[0].name());
}

TEST_F(AnalysisFixture, RubyPerturbation_FreeStandingUserScript) {
  // construct
  openstudio::path scriptPath = toPath("myUserScript.rb");
  RubyPerturbation perturbation(scriptPath,
                                FileReferenceType::IDF,
                                FileReferenceType::IDF,
                                true);
  EXPECT_EQ(FileReferenceType(FileReferenceType::IDF),perturbation.inputFileType());
  EXPECT_EQ(FileReferenceType(FileReferenceType::IDF),perturbation.outputFileType());
  EXPECT_FALSE(perturbation.usesBCLMeasure());
  EXPECT_ANY_THROW(perturbation.measure());
  EXPECT_ANY_THROW(perturbation.measureDirectory());
  EXPECT_ANY_THROW(perturbation.measureUUID());
  EXPECT_ANY_THROW(perturbation.measureVersionUUID());
  FileReference scriptReference = perturbation.perturbationScript();
  EXPECT_EQ(scriptPath,scriptReference.path().filename());
  EXPECT_EQ(boost::filesystem::system_complete(scriptPath),scriptReference.path());
  EXPECT_EQ(FileReferenceType(FileReferenceType::RB),scriptReference.fileType());
  EXPECT_TRUE(perturbation.arguments().empty());
  EXPECT_TRUE(perturbation.isUserScript());
  EXPECT_FALSE(perturbation.hasIncompleteArguments());

  // add arguments to fill in
  OSArgument numBoilers = OSArgument::makeIntegerArgument("numBoilers");
  OSArgument numChillers = OSArgument::makeIntegerArgument("numChillers");
  StringVector choices;
  choices.push_back("parallel");
  choices.push_back("series");
  OSArgument topology = OSArgument::makeChoiceArgument("topology",choices);
  EXPECT_TRUE(topology.setDefaultValue("parallel"));
  perturbation.setArgument(numBoilers);
  perturbation.setArgument(numChillers);
  perturbation.setArgument(topology);

  // verify that arguments are incomplete
  EXPECT_EQ(3u,perturbation.arguments().size());
  EXPECT_EQ(2u,perturbation.incompleteArguments().size());
  EXPECT_TRUE(perturbation.hasIncompleteArguments());

  // set to different script and verify that arguments cleared
  scriptPath = toPath("myOtherUserScript.rb");
  perturbation.setPerturbationScript(scriptPath,
                                     FileReferenceType::OSM,
                                     FileReferenceType::OSM,
                                     true);
  EXPECT_EQ(FileReferenceType(FileReferenceType::OSM),perturbation.inputFileType());
  EXPECT_EQ(FileReferenceType(FileReferenceType::OSM),perturbation.outputFileType());
  scriptReference = perturbation.perturbationScript();
  EXPECT_EQ(scriptPath,scriptReference.path().filename());
  EXPECT_TRUE(perturbation.arguments().empty());
  EXPECT_TRUE(perturbation.isUserScript());
  EXPECT_FALSE(perturbation.hasIncompleteArguments());
}

TEST_F(AnalysisFixture, RubyPerturbation_BCLMeasure) {
  // construct
  openstudio::path measuresPath = resourcesPath() / toPath("/utilities/BCL/Measures");
  openstudio::path dir = measuresPath / toPath("SetWindowToWallRatioByFacade");
  BCLMeasure measure = BCLMeasure::load(dir).get();
  RubyPerturbation perturbation(measure);
  EXPECT_TRUE(perturbation.usesBCLMeasure());
  ASSERT_TRUE(perturbation.measure());
  EXPECT_TRUE(perturbation.measure().get() == measure);
  EXPECT_EQ(dir,perturbation.measureDirectory());
  EXPECT_TRUE(measure.uuid() == perturbation.measureUUID());
  EXPECT_TRUE(measure.versionUUID() == perturbation.measureVersionUUID());
  EXPECT_ANY_THROW(perturbation.perturbationScript());
  // isUserScript value is ignored in this case
  EXPECT_TRUE(perturbation.arguments().empty());
  EXPECT_FALSE(perturbation.hasIncompleteArguments());

  // add arguments to fill in
  OSArgumentVector args;
  args.push_back(OSArgument::makeDoubleArgument("wwr"));
  args.push_back(OSArgument::makeDoubleArgument("sillHeight"));
  perturbation.setArguments(args);

  // verify that arguments are incomplete
  EXPECT_EQ(2u,perturbation.incompleteArguments().size());

  // fill in argument values
  OSArgument arg = OSArgument::makeDoubleArgument("wwr");
  EXPECT_TRUE(arg.setValue(0.8));
  perturbation.setArgument(arg);
  EXPECT_EQ(2u,perturbation.arguments().size());
  EXPECT_EQ(1u,perturbation.incompleteArguments().size());
  EXPECT_TRUE(perturbation.hasIncompleteArguments());
  arg = OSArgument::makeDoubleArgument("sillHeight");
  arg.setValue("0.1");
  perturbation.setArgument(arg);
  EXPECT_EQ(2u,perturbation.arguments().size());
  EXPECT_EQ(0u,perturbation.incompleteArguments().size());
  EXPECT_FALSE(perturbation.hasIncompleteArguments());

  // update measure
  openstudio::path tempDir = measuresPath / toPath(toString(createUUID()));
  {
    BCLMeasure newVersion = measure.clone(tempDir).get();
    newVersion.setDescription("Window to wall ratio by wwr and offset.");
    newVersion.save();
    EXPECT_TRUE(newVersion.uuid() == measure.uuid());
    EXPECT_FALSE(newVersion.versionUUID() == measure.versionUUID());
    args.pop_back();
    args.push_back(OSArgument::makeDoubleArgument("offset"));
    perturbation.updateMeasure(newVersion,args);
    EXPECT_TRUE(perturbation.usesBCLMeasure());
    ASSERT_TRUE(perturbation.measure());
    EXPECT_TRUE(perturbation.measure().get() == newVersion);
    EXPECT_EQ(tempDir,perturbation.measureDirectory());
    EXPECT_TRUE(newVersion.uuid() == perturbation.measureUUID());
    EXPECT_TRUE(newVersion.versionUUID() == perturbation.measureVersionUUID());
    EXPECT_ANY_THROW(perturbation.perturbationScript());

    // verify that arguments updated correctly, values retained
    EXPECT_EQ(2u,perturbation.arguments().size());
    ASSERT_EQ(1u,perturbation.incompleteArguments().size());
    EXPECT_TRUE(perturbation.hasIncompleteArguments());
    EXPECT_EQ("offset",perturbation.incompleteArguments()[0].name());
    EXPECT_EQ("wwr",perturbation.arguments()[0].name());
    EXPECT_EQ(0.8,perturbation.arguments()[0].valueAsDouble());

    // set measure and verify that arguments cleared
    perturbation.setMeasure(newVersion); // always goes through setting motions, even if same
    EXPECT_TRUE(perturbation.usesBCLMeasure());
    ASSERT_TRUE(perturbation.measure());
    EXPECT_TRUE(perturbation.measure().get() == newVersion);
    EXPECT_EQ(tempDir,perturbation.measureDirectory());
    EXPECT_TRUE(newVersion.uuid() == perturbation.measureUUID());
    EXPECT_TRUE(newVersion.versionUUID() == perturbation.measureVersionUUID());
    EXPECT_ANY_THROW(perturbation.perturbationScript());
    // isUserScript value is ignored in this case
    EXPECT_TRUE(perturbation.arguments().empty());
    EXPECT_FALSE(perturbation.hasIncompleteArguments());
  }
  boost::filesystem::remove_all(tempDir);
}
